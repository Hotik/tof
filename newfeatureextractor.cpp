#include "newfeatureextractor.h"

using namespace cv;
using namespace std;

NewFeatureExtractor::NewFeatureExtractor()
{

}

NewFeatureExtractor::NewFeatureExtractor(std::shared_ptr<RecognitionSettings> rs) : settings(rs) {}

NewFeatureExtractor::~NewFeatureExtractor()
{}

std::shared_ptr<PoseFeatures> NewFeatureExtractor::extractFeatures(Mat frame)
{

    if (!is_empty(frame))
    {
        shared_ptr<PoseFeatures> pose_features(new PoseFeatures);
        array<double, 2> area = getHumanArea(frame);

        pose_features.get()->setInBed(is_human_in_bed(frame));
        pose_features.get()->setAreaInBed(area[0]);
        pose_features.get()->setAreaNotInBed(area[1]);
        if (area[0])
            pose_features.get()->setRelArea(area[1]/area[0]);
        else
            pose_features.get()->setRelArea(area[1]);
        pose_features.get()->setBedIntensity(getBedIntensityHistogram(frame));
        pose_features.get()->setFloorIntensity(getFLoorIntensityHistogram(frame));
        pose_features.get()->setHumanLength(getHumanLength(frame));

        return pose_features;
    }
    else
        return nullptr;
}



bool NewFeatureExtractor::is_in_bed(double x, double y)
{
    array<Point2f, 4> bed_corners = settings.get()->getBedCorners();
    return x <= max(bed_corners[0].x , bed_corners[1].x) && x >= min(bed_corners[2].x , bed_corners[3].x) &&
            y <= max(bed_corners[1].y, bed_corners[2].y) && y >= min(bed_corners[0].y, bed_corners[3].y);

}


bool NewFeatureExtractor::is_in_bed(Point2f point)
{
    double x = point.x;
    double y = point.y;
    array<Point2f, 4> bed_corners = settings.get()->getBedCorners();
    return x <= max(bed_corners[0].x , bed_corners[1].x) && x >= min(bed_corners[2].x , bed_corners[3].x) &&
             y <= max(bed_corners[1].y, bed_corners[2].y) && y >= min(bed_corners[0].y, bed_corners[3].y);
}

bool NewFeatureExtractor::is_empty(Mat img)
{
    vector<vector<Point>> contours = getHumanContours(img);
    double tmp_area;
    double human_area = 0;
    for (auto contour : contours)
    {
        tmp_area = contourArea(contour);
        if (tmp_area <= settings.get()->getMaxNoiseArea() * settings.get()->getWidth() * settings.get()->getHeight())
                continue;
        human_area += tmp_area;
    }

    if (human_area >= settings.get()->getMinHumanArea() * settings.get()->getWidth() * settings.get()->getHeight())
        return false;
    else
        return true;
}

vector<vector<Point>> NewFeatureExtractor::getHumanContours(Mat img)
{
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            if (img.at<uint16_t>(i, j) == MAX_COLOR)
                img.at<uint16_t>(i, j) = 0;
    img.convertTo(img, CV_8U, 1 / settings.get()->getConvertScale());
    Mat mask;

    threshold(img, mask, settings.get()->getHumContThresholdScale(), 255, CV_THRESH_BINARY);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    return contours;
}

bool NewFeatureExtractor::is_human_in_bed(Mat img)
{
    vector<Point> max_contour = getHumanMaxContour(getHumanContours(img));
    Moments m = moments(max_contour, false);
    double xc = m.m10 / m.m00;
    double yc = m.m01 / m.m00;
    return is_in_bed(xc, yc);
}

array<double, 2> NewFeatureExtractor::getHumanArea(Mat img)
{
    double area_in_bed = 0;
    double area_not_in_bed = 0;
    for( int i = 0; i < img.rows; i++ )
           for(int j = 0; j < img.cols; j++ )
           {
               if (img.at<uint16_t>(i, j) != MAX_COLOR)
               {
                   if (is_in_bed(j + 1, i + 1))
                       area_in_bed++;
                   else
                       area_not_in_bed++;
               }
           }
    array<double, 2> area {{area_in_bed, area_not_in_bed}};
    return area;
}

vector<Point> NewFeatureExtractor::getHumanMaxContour(std::vector<std::vector<Point>> &&contours)
{
    double max_area = 0;
    int max_area_ind = 0;
    int ind = 0;
    for (auto contour : contours)
    {
        double area = contourArea(contour);
        if (max_area < area)
        {
            max_area_ind = ind;
            max_area = area;
        }
        ind++;
    }

    vector<Point> max_contour = contours.at(max_area_ind);
    return max_contour;
}

double NewFeatureExtractor::getHumanLength(Mat img)
{

    vector<Point> max_contour = getHumanMaxContour(getHumanContours(img));
    Rect brect = boundingRect(max_contour);
    return max(brect.width, brect.height);
}

vector<double> NewFeatureExtractor::getBedIntensityHistogram(Mat img)
{
    for( int i = 0; i < img.rows; i++ )
        for(int j = 0; j < img.cols; j++ )
            if (!is_in_bed(j,i))
                img.at<uint16_t>(i,j) = 65535;


    int bins = settings.get()->getBedHistBins();
    int histSize[] = {bins};
    const float* ranges[] = {settings.get()->getBedHistRanges().get()};
    cv::Mat hist;
    int channels[] = {0};
    cv::calcHist(&img, 1, channels, Mat(), hist, 1, histSize, ranges, false, false);

    vector<double> intensity(bins);
    for(int i = 0; i < bins; i++)
        intensity[i] = hist.at<float>(i);

    float sum = 0;
    for (int i = 0; i < bins; i++)
        sum += intensity[i];

    for (int i = 0; i < bins; i++)
        intensity[i] /= sum;

    return intensity;
}


vector<double> NewFeatureExtractor::getFLoorIntensityHistogram(Mat img)
{
    int bins = settings.get()->getFloorHistBins();
    int histSize[] = {bins};
    const float* ranges[] = {settings.get()->getFloorHistRanges().get()};
    cv::Mat hist;
    int channels[] = {0};
    cv::calcHist(&img, 1, channels, Mat(), hist, 1, histSize, ranges, false, false);

    vector<double> intensity(bins);
    for(int i = 0; i < bins; i++)
        intensity[i] = hist.at<float>(i);

    float sum = 0;
    for (int i = 0; i < bins; i++)
        sum += intensity[i];

    for (int i = 0; i < bins; i++)
        intensity[i] /= sum;

    return intensity;
}

#include "newfeatureextractor.h"

using namespace cv;
using namespace std;

NewFeatureExtractor::NewFeatureExtractor()
{

}

NewFeatureExtractor::~NewFeatureExtractor()
{}

std::shared_ptr<PoseFeatures> NewFeatureExtractor::extractFeatures(Mat frame)
{
    shared_ptr<PoseFeatures> pose_features(new PoseFeatures);

    changeIntensity(frame, getFrameAngle());



    return pose_features;
}

Mat NewFeatureExtractor::imageTransformation(Mat img, double depthCur, double offsetCur, double depthNew, double offsetNew)
{
    if (depthCur != depthNew || offsetCur != offsetNew)
    {
        double tmp = (offsetNew - offsetCur)*MAX_COLOR / depthNew;
        for( int i = 0; i < img.rows; i++ )
               for( int j = 0; j < img.cols; j++ )
               {
                   img.at<uint16_t>(i, j) = uint16_t((double(img.at<uint16_t>(i, j)) * depthCur + (offsetCur - offsetNew)*MAX_COLOR)/depthNew);
                   if (img.at<uint16_t>(i,j) < tmp + 200)
                       img.at<uint16_t>(i,j) = 1;
               }
    }
    return img;
}
void NewFeatureExtractor::bedRecognition(Mat img)
{
    img.convertTo(img, CV_8U, COLOR_SCALE);
    Mat mask;
    threshold(img, mask, getBedRecThresholdScale(), 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

    vector<std::vector<Point>> contours;
    findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);


    int biggestContourIdx = -1;
    float biggestContourArea = 0;

    float ctArea = 0;
    for (auto i = 0; i < contours.size(); i++)
    {
        ctArea = contourArea(contours[i]);
        if (ctArea > biggestContourArea)
        {
              biggestContourArea = ctArea;
              biggestContourIdx = i;
        }
    }

    if(biggestContourIdx < 0)
    {
        LOG(ERROR) << "Can't find bed";
        throw FeatureExtractorException("Can't find bed");
    }

    RotatedRect boundingBox = minAreaRect(contours[biggestContourIdx]);

    Point2f corners[4];
    boundingBox.points(corners);
    this->bed_corners[0] = corners[0];
    this->bed_corners[1] = corners[1];
    this->bed_corners[2] = corners[2];
    this->bed_corners[3] = corners[3];

    sort(this->bed_corners.begin(), this->bed_corners.end(), [](const Point2f &a, const Point2f &b) {return a.x > b.x;});
    if (this->bed_corners[0].y > this->bed_corners[1].y)
        swap(this->bed_corners[0], this->bed_corners[1]);
    if (this->bed_corners[3].y > this->bed_corners[2].y)
        swap(this->bed_corners[3], this->bed_corners[2]);
}


bool NewFeatureExtractor::is_in_bed(double x, double y)
{
    return x <= max(this->bed_corners[0].x , this->bed_corners[1].x) && x >= min(this->bed_corners[2].x , this->bed_corners[3].x) &&
            y <= max(this->bed_corners[1].y, this->bed_corners[2].y) && y >= min(this->bed_corners[0].y, this->bed_corners[3].y);

}


bool NewFeatureExtractor::is_in_bed(Point2f point)
{
    double x = point.x;
    double y = point.y;
    return x <= max(this->bed_corners[0].x , this->bed_corners[1].x) && x >= min(this->bed_corners[2].x , this->bed_corners[3].x) &&
        y <= max(this->bed_corners[1].y, this->bed_corners[2].y) && y >= min(this->bed_corners[0].y, this->bed_corners[3].y);
}

bool NewFeatureExtractor::is_empty(Mat img)
{
    vector<vector<Point>> contours = getHumanContours(img);
    double tmp_area;
    double human_area = 0;
    for (auto contour : contours)
    {
        tmp_area = contourArea(contour);
        if (tmp_area <= this->max_noise_area * this->width * this->height)
                continue;
        human_area += tmp_area;
    }

    if (human_area >= this->min_human_area * this->width * this->height)
        return false;
    else
        return true;
}

vector<vector<Point>> NewFeatureExtractor::getHumanContours(Mat img)
{
    Mat dif = getEmptyImage() - img;
    Mat tmp = dif.clone();
    medianBlur(dif, tmp, getMedianBlurScale());
    tmp.convertTo(tmp, CV_8U, COLOR_SCALE);
    Mat mask;
    threshold(tmp, mask, getHumContThresholdScale(), 255, CV_THRESH_BINARY);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    return contours;
}

Mat NewFeatureExtractor::changeIntensity(Mat img, double degree)
{
    double angle = degree * M_PI/180;
    double xc  = this->width / 2;
    double yc = this->height / 2;
    double r = this->width / 2 / tan(angle/2);

    for( int i = 0; i < img.rows; i++ )
    {
        double dy = yc - i - 1;
           for( int j = 0; j < img.cols; j++ )
           {
               double dx = xc - j - 1;
               double dxy = sqrt(dx*dx + dy*dy);
               double alpha = atan(dxy / r);
               img.at<uint16_t>(i, j) = uint16_t(double(img.at<uint16_t>(i, j)) * cos(alpha) * cos(alpha));
           }
    }
    return img;
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

    Mat dif = getEmptyImage() - img;
    Mat tmp = dif.clone();
    medianBlur(dif, tmp, getMedianBlurScale());
    tmp.convertTo(tmp, CV_8U, COLOR_SCALE);
    Mat mask;
    threshold(tmp, mask, getHumContThresholdScale(), 255, CV_THRESH_BINARY);


    double area_in_bed = 0;
    double area_not_in_bed = 0;
    for( int i = 0; i < mask.rows; i++ )
           for(int j = 0; j < mask.cols; j++ )
           {
               if (mask.at<uchar>(i, j))
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

vector<float> NewFeatureExtractor::getBedIntensityHistogram(Mat img)
{
    Mat img_tmp = img.clone();
    img_tmp.convertTo(img_tmp, CV_8U, COLOR_SCALE);

    Mat dif = getEmptyImage() - img;
    Mat tmp = dif.clone();
    medianBlur(dif, tmp, getMedianBlurScale());
    tmp.convertTo(tmp, CV_8U, COLOR_SCALE);

    Mat mask;
    threshold(tmp, mask, getHumContThresholdScale(), 255, CV_THRESH_BINARY);
    tmp.convertTo(tmp, CV_16U, 256);

    Mat hist_img = tmp.clone();
    for( int i = 0; i < mask.rows; i++ )
        for(int j = 0; j < mask.cols; j++ )
            if (mask.at<uchar>(i, j) && is_in_bed(j,i))
                hist_img.at<uint16_t>(i, j) = tmp.at<uint16_t>(i, j);
            else
                hist_img.at<uint16_t>(i,j) = 65535;

    int bins = BED_INTENSITY_SIZE;
    int histSize[] = {bins};


    // Set ranges for histogram bins
   // float r1[] = {0.1, 15.625, 31.25, 62.5, 97.65, 250};
   // float r1[] = {0, 4000, 8000, 16000, 25000, 65000};
    const float* ranges[] = {getBedHistRanges().get()};

    cv::Mat hist;
    int channels[] = {0};
    cv::calcHist(&hist_img, 1, channels, Mat(), hist, 1, histSize, ranges, false, false);

    vector<float> intensity(bins);
    for(int i = 0; i < bins; i++)
        intensity[i] = hist.at<float>(i);

    float sum = 0;
    for (int i = 0; i < bins; i++)
        sum += intensity[i];

    for (int i = 0; i < bins; i++)
        intensity[i] /= sum;

    return intensity;
}


vector<float> NewFeatureExtractor::getFLoorIntensityHistogram(Mat img)
{
    Mat img_tmp = img.clone();
    img_tmp.convertTo(img_tmp, CV_8U, COLOR_SCALE);

    Mat dif = getEmptyImage() - img;
    Mat tmp = dif.clone();
    medianBlur(dif, tmp, getMedianBlurScale());
    tmp.convertTo(tmp, CV_8U, COLOR_SCALE);

    Mat mask;
    threshold(tmp, mask, getHumContThresholdScale(), 255, CV_THRESH_BINARY);
    tmp.convertTo(tmp, CV_16U, 256);

    Mat hist_img = tmp.clone();
    for( int i = 0; i < mask.rows; i++ )
        for(int j = 0; j < mask.cols; j++ )
            if (mask.at<uchar>(i, j))
                hist_img.at<uint16_t>(i, j) = img_tmp.at<uint16_t>(i, j);
            else
                hist_img.at<uint16_t>(i,j) = 65535;

    int bins = FLOOR_INTENSITY_SIZE;
    int histSize[] = {bins};

    // Set ranges for histogram bins
   //float r1[] = {0.1, 8000, 16000, 25000, 65000};
   // float *r1 = getFloorHistRanges().get();
    const float* ranges[] = {getFloorHistRanges().get()};

    cv::Mat hist;
    int channels[] = {0};
    cv::calcHist(&hist_img, 1, channels, cv::Mat(), hist, 1, histSize, ranges, false, false);

    vector<float> intensity(bins);
    for(int i = 0; i < bins; i++)
        intensity[i] = hist.at<float>(i);

    float sum = 0;
    for (int i = 0; i < bins; i++)
        sum += intensity[i];

    for (int i = 0; i < bins; i++)
        intensity[i] /= sum;

    return intensity;
}

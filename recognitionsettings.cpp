#include "recognitionsettings.h"
#include <easylogging++.h>

using namespace std;
using namespace cv;

RecognitionSettings::RecognitionSettings()
{

}

RecognitionSettings::~RecognitionSettings()
{

}

void RecognitionSettings::bedRecognition(Mat img)
{
    img.convertTo(img, CV_8U, 1/this->getConvertScale());
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
       // throw FeatureExtractorException("Can't find bed");
        return;
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

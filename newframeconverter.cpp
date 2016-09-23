#include <newframeconverter.h>

using namespace std;
using namespace cv;

NewFrameConverter::NewFrameConverter()
{

}

NewFrameConverter::~NewFrameConverter()
{

}

Mat NewFrameConverter::changeIntensity(Mat img, double degree)
{
    double angle = degree * M_PI/180;
    double xc  = getWidth() / 2;
    double yc = getHeight() / 2;
    double r = getWidth() / 2 / tan(angle/2);

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

Mat NewFrameConverter::imageTransformation(Mat img, double depthCur, double offsetCur, double depthNew, double offsetNew)
{
    if (depthCur != depthNew || offsetCur != offsetNew)
    {
        double tmp = (offsetCur - offsetNew)* getMaxIntensityValue() / depthNew;
        for( int i = 0; i < img.rows; i++ )
               for( int j = 0; j < img.cols; j++ )
               {
                   img.at<uint16_t>(i, j) = uint16_t((double(img.at<uint16_t>(i, j)) * depthCur + (offsetCur - offsetNew)*getMaxIntensityValue())/depthNew);
                   if (img.at<uint16_t>(i,j) == tmp)
                       img.at<uint16_t>(i,j) = 0;
               }
    }
    return img;
}

Mat  NewFrameConverter::convert(Mat img, double depthCur, double offsetCur, double depthNew, double offsetNew)
{
    img = changeIntensity(img, getFrameAngle());
    img = imageTransformation(img, depthCur, offsetCur, depthNew, offsetNew);
    return img;
}

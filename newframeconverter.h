#ifndef NEWFRAMECONVERTER_H
#define NEWFRAMECONVERTER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

class NewFrameConverter
{
public:
    NewFrameConverter();
    ~NewFrameConverter();
    inline void setFrameAngle(double angle) {
        this->frame_angle = angle;
    }

    inline double getFrameAngle() const {
        return frame_angle;
    }

    inline double getMaxIntensityValue() const {
        return max_intensity_value;
    }

    inline void setMaxIntensityValue(double value)
    {
        this->max_intensity_value = value;
    }

    inline void setWidth(int width)
    {
        this->width = width;
    }

    inline int getWidth() const {
        return width;
    }

    inline void setHeight(int height) {
        this->height = height;
    }

    inline int getHeight() const {
        return height;
    }

    cv::Mat convert(cv::Mat img, double depthCur, double offsetCur, double depthNew, double offsetNew);
private:
    cv::Mat imageTransformation(cv::Mat img, double depthCur, double offsetCur, double depthNew, double offsetNew);
    cv::Mat changeIntensity(cv::Mat img, double degree);
    double frame_angle;
    double max_intensity_value;
    double width;
    double height;
};

#endif // NEWFRAMECONVERTER_H

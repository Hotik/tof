#ifndef BODYLOCATOR_H
#define BODYLOCATOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <memory>

#include "RecognitionSettings.h"


class BodyLocator
{
protected:
    std::shared_ptr<RecognitionSettings> settings;

public:
    BodyLocator();
    virtual ~BodyLocator();

    virtual cv::Mat locateBody(cv::Mat frame, cv::Mat base) = 0;
};

#endif // BODYLOCATOR_H

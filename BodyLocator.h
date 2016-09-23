#ifndef BODYLOCATOR_H
#define BODYLOCATOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <memory>

#include "RecognitionSettings.h"

using namespace std;
using namespace cv;

class BodyLocator
{
protected:
    shared_ptr<RecognitionSettings> settings;

public:
    BodyLocator();
    virtual ~BodyLocator();

    virtual Mat locateBody(Mat frame, Mat base) = 0;
};

#endif // BODYLOCATOR_H

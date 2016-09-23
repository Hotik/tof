#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <memory>

#include "PoseFeatures.h"
#include "RecognitionSettings.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;

class FeatureExtractor
{
protected:
    shared_ptr<RecognitionSettings> settings;

    
public:
//    FeatureExtractor(shared_ptr<RecognitionSettings> settings, shared_ptr<Display> display,
//                     shared_ptr<NewFrameConverter> converter);
    virtual ~FeatureExtractor();
    FeatureExtractor() {}
    
    virtual shared_ptr<PoseFeatures> extractFeatures(cv::Mat frame) = 0;
};

#endif // FEATUREEXTRACTOR_H

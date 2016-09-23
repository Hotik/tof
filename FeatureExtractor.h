#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <memory>

#include "PoseFeatures.h"
#include "RecognitionSettings.h"
#include "displays/Display.h"
#include "converters/FrameConverter.h"
#include "feature-extraction/body/BodyLocator.h"
#include "feature-extraction/heat-trace/HeatTraceFilter.h"
#include "feature-extraction/face/FaceDetector.h"
#include "feature-extraction/morphology/MorphologyAnalyzer.h"

using namespace std;

class FeatureExtractor
{
protected:
    shared_ptr<RecognitionSettings> settings;
    shared_ptr<Display> display;
    shared_ptr<FrameConverter> converter;
    shared_ptr<BodyLocator> locator;
    shared_ptr<HeatTraceFilter> filter;
    shared_ptr<MorphologyAnalyzer> analyzer;
    
public:
    FeatureExtractor(shared_ptr<RecognitionSettings> settings, shared_ptr<Display> display,
                     shared_ptr<FrameConverter> converter);
    virtual ~FeatureExtractor();
    FeatureExtractor() {}
    
    virtual shared_ptr<PoseFeatures> extractFeatures(Mat frame) = 0;
};

#endif // FEATUREEXTRACTOR_H

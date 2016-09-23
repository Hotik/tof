#include "FeatureExtractor.h"

FeatureExtractor::FeatureExtractor(shared_ptr<RecognitionSettings> settings, shared_ptr<Display> display,
                                   shared_ptr<FrameConverter> converter)
{
    this->settings = settings;
    this->display = display;
    this->converter = converter;
}

FeatureExtractor::~FeatureExtractor()
{

}

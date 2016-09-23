#ifndef NEWBODYLOCATOR_H
#define NEWBODYLOCATOR_H

#include <BodyLocator.h>
#include <RecognitionSettings.h>


class NewBodyLocator : public BodyLocator
{
public:
    MedianNoiseBodyLocator(shared_ptr<MedianNoiseBodyLocatorSettings> settings);
    NewBodyLocator(std::shared_ptr<RecognitionSettings> settings);
    NewBodyLocator();
    ~NewBodyLocator();

    virtual Mat locateBody(Mat frame, Mat base);
};

#endif // NEWBODYLOCATOR_H

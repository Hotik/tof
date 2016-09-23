#ifndef NEWBODYLOCATOR_H
#define NEWBODYLOCATOR_H

#include <BodyLocator.h>
#include <RecognitionSettings.h>


class NewBodyLocator : public BodyLocator
{
public:
    NewBodyLocator(std::shared_ptr<RecognitionSettings> settings);
    NewBodyLocator();
    ~NewBodyLocator();

    virtual cv::Mat locateBody(cv::Mat frame, cv::Mat base);
};

#endif // NEWBODYLOCATOR_H

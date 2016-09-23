#include "newbodylocator.h"

NewBodyLocator::NewBodyLocator()
{

}

NewBodyLocator::~NewBodyLocator()
{

}

NewBodyLocator::NewBodyLocator(std::shared_ptr<RecognitionSettings> settings)
{
    this->settings = settings;
}

NewBodyLocator::locateBody(Mat frame, Mat base)
{
    Mat res  = frame.clone();
    Mat dif = base - frame;
    Mat tmp = dif.clone();
    medianBlur(dif, tmp, settings.get()->getMedianBlurScale());
    tmp.convertTo(tmp, CV_8U, settings.get()->getConvertScale());

    Mat mask;
    treshold(tmp, mask, settings.get()->getHumContThresholdScale(), 255, CV_THRESH_BINARY);

    for( int i = 0; i < mask.rows; i++ )
        for(int j = 0; j < mask.cols; j++ )
            if (mask.at<uchar>(i, j))
                res.at<uint16_t>(i, j) = dif.at<uint16_t>(i, j);
            else
                res.at<uint16_t>(i,j) = 65535;
    return res;
}

#include "newbodylocator.h"

using namespace cv;
using namespace std;

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

Mat NewBodyLocator::locateBody(Mat frame, Mat base)
{

    Mat dif = base - frame;

    Mat tmp;
    medianBlur(dif, tmp, settings.get()->getMedianBlurScale());
    tmp.convertTo(tmp, CV_8U, 1 / settings.get()->getConvertScale());

    Mat mask;
    threshold(tmp, mask, settings.get()->getHumContThresholdScale(), 255, CV_THRESH_BINARY);

    Mat res = dif.clone();

    for( int i = 0; i < mask.rows; i++ )
        for(int j = 0; j < mask.cols; j++ )
            if (!mask.at<uchar>(i, j))
                res.at<uint16_t>(i,j) = 65535;
    return res;
}

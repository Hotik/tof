#include "frame.h"

using namespace std;
using namespace cv;

Frame::Frame()
{
    this->image_type = CV_LOAD_IMAGE_GRAYSCALE;
    this->width = TOF_WIDTH;
    this->height = TOF_HEIGHT;
}


Frame::Frame(const Frame &src)
{
    this->width = src.width;
    this->height = src.height;
    this->filename = src.filename;
    this->image = src.image;
    this->true_pose = src.true_pose;
    this->predicted_pose = src.predicted_pose;
    this->image_type = src.image_type;
}

Frame::Frame(string name)
{
    this->image_type = CV_LOAD_IMAGE_GRAYSCALE;
    this->width = TOF_WIDTH;
    this->height = TOF_HEIGHT;
    this->filename = name;

}

Frame::~Frame() {
}

void Frame::load()
{
    Mat tmp = imread(this->filename, this->image_type);
    tmp.convertTo(this->image, CV_16U, 256);
    this->width = this->image.cols;
    this->height = this->image.rows;
}

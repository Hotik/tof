#ifndef NEWFEATUREEXTRACTOR_H
#define NEWFEATUREEXTRACTOR_H

#include <FeatureExtractor.h>
#include <array>
#include <ssuexception.h>
#include <log.h>
#include <math.h>
#include "posefeatures.h"
#include <memory.h>
#include <recognitionsettings.h>

#define _USE_MATH_DEFINES

const double MAX_COLOR = 655535;
const double COLOR_SCALE = 0.00390625;
#define  BED_INTENSITY_SIZE  5
#define FLOOR_INTENSITY_SIZE 4

class FeatureExtractorException : public SsuException
{
public:
    FeatureExtractorException(const std::string& reason):SsuException(reason){}
};

class NewFeatureExtractor : public FeatureExtractor
{
public:
    NewFeatureExtractor();
    NewFeatureExtractor(std::shared_ptr<RecognitionSettings> rs);
    virtual ~NewFeatureExtractor();

    virtual shared_ptr<PoseFeatures> extractFeatures(cv::Mat frame);


    std::vector<std::vector<cv::Point>> getHumanContours(cv::Mat img);
    bool is_in_bed(double x, double y);
    bool is_in_bed(cv::Point2f point);
    bool is_empty(cv::Mat img);
    bool is_human_in_bed(cv::Mat img);
    std::array<double, 2> getHumanArea(cv::Mat img);
    double getHumanLength(cv::Mat img);
    std::vector<cv::Point> getHumanMaxContour(std::vector<std::vector<cv::Point>> &&contours);
    vector<double> getBedIntensityHistogram(cv::Mat img);
    std::vector<double> getFLoorIntensityHistogram(cv::Mat img);


private:
    std::shared_ptr<RecognitionSettings> settings;


};

#endif // NEWFEATUREEXTRACTOR_H

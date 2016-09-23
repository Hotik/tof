#ifndef FRAME_H
#define FRAME_H

#include <cstdio>
#include <pose.h>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

const std::size_t TOF_WIDTH = 640;
const std::size_t TOF_HEIGHT = 480;

class Frame
{
public:

    Frame();
    Frame(const Frame& src);
    Frame(std::string name);
    ~Frame();

    /** \brief Acess the frame's width
     */

    inline std::size_t getWidth() const {
        return width;
    }

    /** \brief Acess the frame's height
     */
    inline std::size_t getHeight() const {
        return height;
    }

    /** \brief Access the frame's true pose
     */

    inline Pose getTruePose() const {
        return true_pose;
    }

    /** \brief Acess the frame's predicted pose
     */

    inline Pose getPredictedPose() const {
        return predicted_pose;
    }

    inline void setTruePose(Pose pose) {
        this->true_pose = pose;
    }

    inline void setPredictedPose(Pose pose) {
        this->predicted_pose = pose;
    }

    inline void setFileName(std::string name) {
        this->filename = name;
   }

    /** \brief Acess the frame's file name
     */

    inline std::string getFileName() const {
        return filename;
    }

    /** \brief Acess the frame's image
     */

    inline cv::Mat getImage() const {
        return image;
    }

    inline void setImage(cv::Mat img) {
        this->image = img;
    }

    inline void setImageType(int imageType) {
        this->image_type = imageType;
    }

    inline int getImageType() const {
        return image_type;
    }

    void load();

private:
    std::size_t width;
    std::size_t height;
    Pose true_pose;
    Pose predicted_pose;
    std::string filename;
    cv::Mat image;
    int image_type;
};

#endif // FRAME_H

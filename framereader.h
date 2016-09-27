#ifndef FRAMEREADER_H
#define FRAMEREADER_H

#include <frame.h>
#include <vector>
#include <memory>
#include <dirent.h>
#include <ssuexception.h>


const std::string sit_on_str = "1_sitting_on_the_bed/";
const std::string sit_in_str = "2_sitting_in_the_bed/";
const std::string lie_in_str = "3_lying_on_the_bed/";
const std::string stand_str = "4_standing/";
const std::string sit_floor_str = "5_lying_on_the_floor/";
const std::string lie_floor_str = "6_sitting_on_the_floor/";
const std::string empty_str = "7_empty/";
const std::string not_alone_str = "8_not_alone/";
const std::size_t isFile = 0x8;




class FrameReaderException : public SsuException
{
public:
    FrameReaderException(const std::string& reason):SsuException(reason){}
};


class FrameReader
{
public:
    FrameReader();
    FrameReader(const FrameReader& src);
    ~FrameReader();

    inline void setDirectoryPath(std::string path) {
       this->dir_path = path;
    }

    inline std::string getDirectoryPath() const {
        return dir_path;
    }

    inline void setFrameNumber(std::size_t num) {
        this->frame_number = num;
    }

    inline std::size_t getFrameNumber() const {
        return frame_number;
    }

    void load_frames(bool load_empty_flag = false);
    void load_frames_from_dir(std::string path);

    inline std::vector<std::shared_ptr<Frame>> getFrameArray() const {
        return frames_array;
    }

    inline const std::vector<int> &get_frames_classes() const {
        return frames_classes;
    }

private:
    std::string dir_path;
    std::size_t frame_number;
    std::vector<std::shared_ptr<Frame>> frames_array;
    std::vector<int> frames_classes;
};

#endif // FRAMEREADER_H

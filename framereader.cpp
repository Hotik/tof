#include "framereader.h"
#include "log.h"
#include <iostream>

using namespace std;

FrameReader::FrameReader()
{

}

FrameReader::FrameReader(const FrameReader &src)
{
    this->dir_path = src.dir_path;
    this->frames_array = src.frames_array;
    this->frame_number = src.frame_number;
}


FrameReader::~FrameReader()
{

}

void FrameReader::load_frames_from_dir(string path)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path.c_str());
    if (!dir) {
        LOG(ERROR) << "Can't find " + path;
        throw FrameReaderException( "Can't find " + path);
    }
    else
        while ((entry = readdir(dir)) != NULL)
            if (entry->d_type == isFile)
            {
                shared_ptr<Frame> ptr(new Frame(path + entry->d_name));
                ptr.get()->load();
                this->frames_array.push_back(ptr);
            }

}

void FrameReader::load_frames(bool load_empty_flag)
{
    load_frames_from_dir(this->dir_path + sit_on_str);
    load_frames_from_dir(this->dir_path + sit_in_str);
    load_frames_from_dir(this->dir_path + lie_in_str);
    load_frames_from_dir(this->dir_path + stand_str);
    load_frames_from_dir(this->dir_path + lie_floor_str);
    load_frames_from_dir(this->dir_path + sit_floor_str);
    if (load_empty_flag)
        load_frames_from_dir(this->dir_path + empty_str);
    load_frames_from_dir(this->dir_path + not_alone_str);
}

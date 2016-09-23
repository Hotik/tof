#ifndef POSEFEATURES_H
#define POSEFEATURES_H

#include <vector>

class PoseFeatures
{
public:
    PoseFeatures();

    inline void setInBed(bool in_bed) {
        this->is_in_bed = in_bed;
    }

    inline bool getInBed() const {
        return is_in_bed;
    }

    inline void setAreaInBed(double area) {
        this->area_in_bed = area;
    }

    inline double getAreaInBed() const {
        return area_in_bed;
    }

    inline void setAreaNotInBed(double area) {
        this->area_not_in_bed = area;
    }

    inline double getAreaNotInBed() const {
        return area_not_in_bed;
    }

    inline void setRelArea(double area) {
        this->rel_area = area;
    }

    inline double getRelArea() const {
        return rel_area;
    }

    inline void setBedIntensity(std::vector<float> &arr) {
        this->bed_intensity = arr;
    }

    inline const std::vector<float>& getBedIntensity() const {
        return bed_intensity;
    }

    inline void setFloorIntensity(std::vector<float> &arr) {
        this->floor_intensity = arr;
    }

    inline const std::vector<float>& getFloorIntensity() const {
        return floor_intensity;
    }

    inline void setHumanLength(double length) {
        this->human_length = length;
    }

    inline double getHumanLength() const {
        return human_length;
    }

    inline void setDelta(double d) {
        this->delta = d;
    }

    inline double getDelta() const {
        return delta;
    }

private:
    bool is_in_bed;
    double area_in_bed;
    double area_not_in_bed;
    double rel_area;
    std::vector<float> bed_intensity;
    std::vector<float> floor_intensity;
    double human_length;
    double delta;
};

#endif // POSEFEATURES_H

#pragma once
#include <exception>
#define TROPHY_POINT_BRNZ 15
#define TROPHY_POINT_SILV 30
#define TROPHY_POINT_GOLD 90
#define TROPHY_POINT_PLAT_V1 180
#define TROPHY_POINT_PLAT_V2 300
#define LEVEL_MIN 1
#define LEVEL_MAX_V1 99
#define LEVEL_MAX_V2 999
#define LEVEL_SEGMENT_LENGTH_V1 1
#define LEVEL_SEGMENT_LENGTH_V2 100
#define LEVEL_SEGMENTS_V1 26
#define LEVEL_SEGMENTS_V2 10
#define MASK_BRNZ 0x1
#define MASK_SILV 0x2
#define MASK_GOLD 0x4
#define MASK_PLAT 0x8
#define MASK_FULL 0xf
const unsigned int LEVEL_UPGRADE_POINTS_V1[LEVEL_SEGMENTS_V1] = {200, 400, 600, 1200, 1600, 2000, 2000, 2000, 2000, 2000, 2000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 10000};
const unsigned int LEVEL_UPGRADE_POINTS_V2[LEVEL_SEGMENTS_V2] = {60, 90, 450, 900, 1350, 1800, 2250, 2700, 3150, 3600};

namespace psn {
class invalid_level_exception : public std::exception {
    virtual const char* what() const throw() {
        return "invalid level value";
    }
};

class invalid_level_percentage_exception : public std::exception {
    virtual const char* what() const throw() {
        return "invalid level percentage value";
    }
};

class invalid_points_exception : public std::exception {
    virtual const char* what() const throw() {
        return "invalid points value";
    }
};

class profile {
  private:
    unsigned int bronze = 0;
    unsigned int silver = 0;
    unsigned int gold = 0;
    unsigned int platinum = 0;
    unsigned int points_v1 = 0;
    unsigned int points_v2 = 0;
    unsigned int level_v1 = LEVEL_MIN;
    unsigned int level_v2 = LEVEL_MIN;
    double level_percentage_v1 = 0;
    double level_percentage_v2 = 0;
    void update_points();
    void update_level();

  public:
    profile(unsigned int bronze = 0, unsigned int silver = 0, unsigned int gold = 0, unsigned int platinum = 0);
    void set_trophies(unsigned char mask, unsigned int bronze, unsigned int silver, unsigned int gold, unsigned int platinum);
    unsigned int get_points_v1();
    unsigned int get_points_v2();
    unsigned int get_level_v1(double& percentage);
    unsigned int get_level_v2(double& percentage);
};

void validate_level(unsigned int& level, double& percentage);
unsigned int level_to_points_v1(unsigned int level, double percentage = 0);
unsigned int level_to_points_v2(unsigned int level, double percentage = 0);
unsigned int points_to_level_v1(unsigned int points, double* percentage);
unsigned int points_to_level_v2(unsigned int points, double* percentage);
}; // namespace psn

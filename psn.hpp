#pragma once
#define TROPHY_POINT_BRNZ 15
#define TROPHY_POINT_SILV 30
#define TROPHY_POINT_GOLD 90
#define TROPHY_POINT_PLAT 300
#define LEVEL_MIN 1
#define LEVEL_MAX 999
#define LEVEL_SEGMENT_LENGTH 100
#define LEVEL_SEGMENTS 10
#define MASK_BRNZ 0x1
#define MASK_SILV 0x2
#define MASK_GOLD 0x4
#define MASK_PLAT 0x8
#define MASK_FULL 0xf
const unsigned int LEVEL_UPGRADE_POINTS[LEVEL_SEGMENTS] = {60, 90, 450, 900, 1350, 1800, 2250, 2700, 3150, 3600};

namespace psn {
class profile {
  private:
    unsigned int bronze = 0;
    unsigned int silver = 0;
    unsigned int gold = 0;
    unsigned int platinum = 0;
    unsigned int points = 0;
    unsigned int level = LEVEL_MIN;
    double level_percentage = 0;
    void update_points();
    void update_level();

  public:
    profile(unsigned int bronze = 0, unsigned int silver = 0, unsigned int gold = 0, unsigned int platinum = 0);
    void set_trophies(unsigned char mask, unsigned int bronze, unsigned int silver, unsigned int gold, unsigned int platinum);
    unsigned int get_points();
    unsigned int get_level(double& percentage);
};

unsigned int level_to_points(unsigned int level);
unsigned int points_to_level(unsigned int points, double* percentage);
}; // namespace psn

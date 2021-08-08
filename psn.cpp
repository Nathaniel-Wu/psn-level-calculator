#include "psn.hpp"
#include <vector>

static unsigned int level_segment_points[LEVEL_SEGMENTS] = {0};
static std::vector<bool> level_segment_points_set(LEVEL_SEGMENTS, false);
static inline unsigned int level_segment_points_accelerated(unsigned int level_segment) { // points accumulated until the end of the level segment, or the begining of the next level segment
    if (level_segment_points_set[level_segment])
        return level_segment_points[level_segment];
    unsigned int points;
    if (level_segment == 0)
        points = ((level_segment + 1) * LEVEL_SEGMENT_LENGTH - LEVEL_MIN) * LEVEL_UPGRADE_POINTS[level_segment];
    else
        points = level_segment_points_accelerated(level_segment - 1) + LEVEL_SEGMENT_LENGTH * LEVEL_UPGRADE_POINTS[level_segment];
    level_segment_points[level_segment] = points;
    level_segment_points_set[level_segment] = true;
    return points;
}

static unsigned int level_points[LEVEL_MAX - LEVEL_MIN + 1] = {0};
static std::vector<bool> level_points_set(LEVEL_MAX - LEVEL_MIN + 1, false);
unsigned int psn::level_to_points(unsigned int level) { // points at the begining of the level
    unsigned int level_offset = level - LEVEL_MIN;
    if (level_points_set[level_offset])
        return level_points[level_offset];
    unsigned int level_segment = level / LEVEL_SEGMENT_LENGTH;
    unsigned int points;
    if (level_segment == 0)
        points = level_offset * LEVEL_UPGRADE_POINTS[level_segment];
    else
        points = level_segment_points_accelerated(level_segment - 1) + (level - (level_segment - 1) * LEVEL_SEGMENT_LENGTH) * LEVEL_UPGRADE_POINTS[level_segment];
    level_points[level_offset] = points;
    level_points_set[level_offset] = true;
    return points;
}

unsigned int psn::points_to_level(unsigned points, double* percentage) { // double* percentage saves an optional return value
    unsigned int level_segment;
    unsigned int last_level_segment_points;
    for (level_segment = LEVEL_SEGMENTS - 1; level_segment >= 1; level_segment--)
        if (points >= (last_level_segment_points = level_segment_points_accelerated(level_segment - 1)))
            break;
    unsigned int points_remainder;
    unsigned int level;
    if (level_segment == 0) {
        points_remainder = points % LEVEL_UPGRADE_POINTS[level_segment];
        level = points / LEVEL_UPGRADE_POINTS[level_segment] + LEVEL_MIN;
    } else {
        points_remainder = (points - last_level_segment_points) % LEVEL_UPGRADE_POINTS[level_segment];
        level = (points - last_level_segment_points) / LEVEL_UPGRADE_POINTS[level_segment] + (level_segment == 0 ? LEVEL_MIN : level_segment * LEVEL_SEGMENT_LENGTH);
    }
    if (percentage)
        *percentage = 100 * (double)points_remainder / LEVEL_UPGRADE_POINTS[level_segment];
    level_points[level - LEVEL_MIN] = points - points_remainder;
    level_points_set[level - LEVEL_MIN] = true;
    return level;
}

psn::profile::profile(unsigned int bronze, unsigned int silver, unsigned int gold, unsigned int platinum) {
    this->bronze = bronze;
    this->silver = silver;
    this->gold = gold;
    this->platinum = platinum;
    update_points();
    update_level();
}

void psn::profile::update_points() {
    points = bronze * TROPHY_POINT_BRNZ + silver * TROPHY_POINT_SILV + gold * TROPHY_POINT_GOLD + platinum * TROPHY_POINT_PLAT;
}

void psn::profile::update_level() {
    level = psn::points_to_level(points, &level_percentage);
}

void psn::profile::set_trophies(unsigned char mask, unsigned int bronze, unsigned int silver, unsigned int gold, unsigned int platinum) {
    if (mask & MASK_BRNZ)
        this->bronze = bronze;
    if (mask & MASK_SILV)
        this->silver = silver;
    if (mask & MASK_GOLD)
        this->gold = gold;
    if (mask & MASK_PLAT)
        this->platinum = platinum;
    update_points();
    update_level();
}

unsigned int psn::profile::get_points() { return points; }

unsigned int psn::profile::get_level(double& percentage) {
    percentage = level_percentage;
    return level;
}

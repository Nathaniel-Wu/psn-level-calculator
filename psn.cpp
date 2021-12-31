#include "psn.hpp"
#include <cmath>
#include <vector>

static unsigned int level_segment_points_v2[LEVEL_SEGMENTS_V2] = {0};
static std::vector<bool> level_segment_points_set_v2(LEVEL_SEGMENTS_V2, false);
static inline unsigned int level_segment_points_accelerated_v2(unsigned int level_segment) { // points accumulated until the end of the level segment, or the begining of the next level segment
    if (level_segment_points_set_v2[level_segment])
        return level_segment_points_v2[level_segment];
    unsigned int points;
    if (level_segment == 0)
        points = ((level_segment + 1) * LEVEL_SEGMENT_LENGTH_V2 - LEVEL_MIN) * LEVEL_UPGRADE_POINTS_V2[level_segment];
    else
        points = level_segment_points_accelerated_v2(level_segment - 1) + LEVEL_SEGMENT_LENGTH_V2 * LEVEL_UPGRADE_POINTS_V2[level_segment];
    level_segment_points_v2[level_segment] = points;
    level_segment_points_set_v2[level_segment] = true;
    return points;
}

void psn::validate_level(unsigned int& level, double& percentage) {
    if (level < 1)
        throw psn::invalid_level_exception();
    if (percentage < 0 || percentage > 100)
        throw psn::invalid_level_percentage_exception();
    if (percentage == 100) {
        level++;
        percentage = 0;
    }
}

static unsigned int level_points_v1[LEVEL_MAX_V1 - LEVEL_MIN + 1] = {0};
static std::vector<bool> level_points_set_v1(LEVEL_MAX_V1 - LEVEL_MIN + 1, false);
static const unsigned int last_irregular_level_v1 = LEVEL_MIN + LEVEL_SEGMENTS_V1 - 1;
static inline unsigned int level_start_to_points_v1(unsigned int level) {
    unsigned int level_offset = level - LEVEL_MIN;
    if (level <= LEVEL_MAX_V1 && level_points_set_v1[level_offset])
        return level_points_v1[level_offset];
    unsigned int points = 0;
    if (level <= last_irregular_level_v1) {
        if (level <= LEVEL_MIN)
            points = 0;
        else
            points = level_start_to_points_v1(level - 1) + LEVEL_UPGRADE_POINTS_V1[level_offset - 1];
    } else
        points = level_start_to_points_v1(last_irregular_level_v1) + (level - last_irregular_level_v1) * LEVEL_UPGRADE_POINTS_V1[LEVEL_SEGMENTS_V1 - 1];
    if (level <= LEVEL_MAX_V1) {
        level_points_v1[level_offset] = points;
        level_points_set_v1[level_offset] = true;
    }
    return points;
}
unsigned int psn::level_to_points_v1(unsigned int level, double percentage) {
    psn::validate_level(level, percentage);
    unsigned int points = level_start_to_points_v1(level);
    if (percentage > 0)
        points += round(percentage * ((double)(level_start_to_points_v1(level + 1) - points)) / 100.f);
    return points;
}

static unsigned int level_points_v2[LEVEL_MAX_V2 - LEVEL_MIN + 1] = {0};
static std::vector<bool> level_points_set_v2(LEVEL_MAX_V2 - LEVEL_MIN + 1, false);
static inline unsigned int level_start_to_points_v2(unsigned int level) { // points at the begining of the level
    unsigned int level_offset = level - LEVEL_MIN;
    if (level <= LEVEL_MAX_V2 && level_points_set_v2[level_offset])
        return level_points_v2[level_offset];
    unsigned int level_segment = level / LEVEL_SEGMENT_LENGTH_V2;
    if (level_segment >= LEVEL_SEGMENTS_V2)
        level_segment = LEVEL_SEGMENTS_V2 - 1;
    unsigned int points;
    if (level_segment == 0)
        points = level_offset * LEVEL_UPGRADE_POINTS_V2[level_segment];
    else
        points = level_segment_points_accelerated_v2(level_segment - 1) + (level - level_segment * LEVEL_SEGMENT_LENGTH_V2) * LEVEL_UPGRADE_POINTS_V2[level_segment];
    if (level <= LEVEL_MAX_V2) {
        level_points_v2[level_offset] = points;
        level_points_set_v2[level_offset] = true;
    }
    return points;
}
unsigned int psn::level_to_points_v2(unsigned int level, double percentage) {
    psn::validate_level(level, percentage);
    unsigned int points = level_start_to_points_v2(level);
    if (percentage > 0)
        points += round(percentage * ((double)(level_start_to_points_v2(level + 1) - points)) / 100.f);
    return points;
}

unsigned int psn::points_to_level_v1(unsigned points, double* percentage) { // double* percentage saves an optional return value
    unsigned int level;
    unsigned int last_level_points;
    for (level = last_irregular_level_v1; level >= LEVEL_MIN; level--)
        if (points >= (last_level_points = level_start_to_points_v1(level)))
            break;
    unsigned int points_remainder;
    if (level == last_irregular_level_v1) {
        level = level + (points - last_level_points) / LEVEL_UPGRADE_POINTS_V1[LEVEL_SEGMENTS_V1 - 1];
        points_remainder = (points - last_level_points) % LEVEL_UPGRADE_POINTS_V1[LEVEL_SEGMENTS_V1 - 1];
        if (!level_points_set_v1[level - LEVEL_MIN] && level <= LEVEL_MAX_V1) {
            level_points_v1[level - LEVEL_MIN] = points - points_remainder;
            level_points_set_v1[level - LEVEL_MIN] = true;
        }
        if (percentage)
            *percentage = 100 * (double)points_remainder / LEVEL_UPGRADE_POINTS_V1[LEVEL_SEGMENTS_V1 - 1];
    } else {
        points_remainder = points - last_level_points;
        if (percentage)
            *percentage = 100 * (double)points_remainder / LEVEL_UPGRADE_POINTS_V1[level - LEVEL_MIN];
    }
    return level;
}
unsigned int psn::points_to_level_v2(unsigned points, double* percentage) { // double* percentage saves an optional return value
    unsigned int level_segment;
    unsigned int last_level_segment_points;
    for (level_segment = LEVEL_SEGMENTS_V2 - 1; level_segment >= 1; level_segment--)
        if (points >= (last_level_segment_points = level_segment_points_accelerated_v2(level_segment - 1)))
            break;
    unsigned int points_remainder;
    unsigned int level;
    if (level_segment == 0) {
        points_remainder = points % LEVEL_UPGRADE_POINTS_V2[level_segment];
        level = points / LEVEL_UPGRADE_POINTS_V2[level_segment] + LEVEL_MIN;
    } else {
        points_remainder = (points - last_level_segment_points) % LEVEL_UPGRADE_POINTS_V2[level_segment];
        level = (points - last_level_segment_points) / LEVEL_UPGRADE_POINTS_V2[level_segment] + (level_segment == 0 ? LEVEL_MIN : level_segment * LEVEL_SEGMENT_LENGTH_V2);
    }
    if (percentage)
        *percentage = 100 * (double)points_remainder / LEVEL_UPGRADE_POINTS_V2[level_segment];
    if (level <= LEVEL_MAX_V2) {
        level_points_v2[level - LEVEL_MIN] = points - points_remainder;
        level_points_set_v2[level - LEVEL_MIN] = true;
    }
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
    points_v1 = bronze * TROPHY_POINT_BRNZ + silver * TROPHY_POINT_SILV + gold * TROPHY_POINT_GOLD + platinum * TROPHY_POINT_PLAT_V1;
    points_v2 = points_v1 + platinum * (TROPHY_POINT_PLAT_V2 - TROPHY_POINT_PLAT_V1);
}

void psn::profile::update_level() {
    level_v1 = psn::points_to_level_v1(points_v1, &level_percentage_v1);
    level_v2 = psn::points_to_level_v2(points_v2, &level_percentage_v2);
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

unsigned int psn::profile::get_points_v1() { return points_v1; }

unsigned int psn::profile::get_points_v2() { return points_v2; }

unsigned int psn::profile::get_level_v1(double& percentage) {
    percentage = level_percentage_v1;
    return level_v1;
}

unsigned int psn::profile::get_level_v2(double& percentage) {
    percentage = level_percentage_v2;
    return level_v2;
}

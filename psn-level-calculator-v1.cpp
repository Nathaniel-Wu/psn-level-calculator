#include "psn.hpp"
#include <cstdio>
#include <cstdlib>

static inline void display_profile(psn::profile& p) {
    double percentage;
    unsigned int level = p.get_level_v1(percentage);
    printf("PSN level %u (%.2f%%), points: %u\n", level, percentage, p.get_points_v1());
    printf("PSN level %u starts at %u points\n", level, psn::level_to_points_v1(level));
}

int main(int argc, char* argv[]) {
    if (argc == 5) {
        psn::profile p(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
        display_profile(p);
    } else if (argc == 1) {
        unsigned int bronze, silver, gold, platinum;
        psn::profile p;
        while (!feof(stdin)) {
            scanf("%u %u %u %u%*[^0-9]", &bronze, &silver, &gold, &platinum);
            p.set_trophies(MASK_FULL, bronze, silver, gold, platinum);
            display_profile(p);
        }
    } else
        exit(1);
    return 0;
}

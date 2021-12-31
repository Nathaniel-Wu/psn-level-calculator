#include "psn.hpp"
#include <cstdio>
#include <cstdlib>
#include <exception>

int main(int argc, char* argv[]) {
    try {
        unsigned int level;
        if (argc == 2 || argc == 3) {
            level = atoi(argv[1]);
            if (argc == 3) {
                double percentage = atof(argv[2]);
                psn::validate_level(level, percentage);
                unsigned int points = psn::level_to_points_v1(level, percentage);
                printf("PSN level %u (%.2f%%) is %u points\n", level, percentage, points);
            } else
                printf("PSN level %u starts at %u points\n", level, psn::level_to_points_v1(level));
        } else {
            fprintf(stderr, "invalid arguments\n");
            exit(1);
        }
    } catch (std::exception& e) {
        fprintf(stderr, "%s\n", e.what());
        exit(1);
    }
    return 0;
}

#include "psn.hpp"
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc == 2) {
        unsigned int points = atoi(argv[1]);
        double percentage;
        unsigned int level = psn::points_to_level_v1(points, &percentage);
        printf("PSN level %u (%.2f%%)\n", level, percentage);
    } else
        exit(1);
    return 0;
}

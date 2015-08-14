#include <random>
#include <ctime>
#include "Rand.h"

std::mt19937 mt(time(NULL));

int rnd::randomize(int min, int max)
{
    std::uniform_int_distribution<int> dist_int(min, max);
    return dist_int(mt);
}

int rnd::randomize(int max)
{
    std::uniform_int_distribution<int> dist_int(0, max);
    return dist_int(mt);
}

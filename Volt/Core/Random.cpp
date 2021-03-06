#include "Volt/Core/Random.h"
#include <time.h>
#include <stdlib.h>

namespace Volt {

void Random::Seed () {
    srand(static_cast<unsigned int>(time(NULL)));
}

void Random::Seed (unsigned int seed) {
    srand(seed);
}

int Random::Range (int start, int end) {
    return start + (rand() % (end - start));
}

float Random::Percent () {
    return ((float) rand()) / (float) RAND_MAX;
}

float Random::RangeFloat (float start, float end) {
    return ((((float) rand()) / (float) RAND_MAX) * (end - start)) + start;
}

bool Random::CoinFlip () {
    return Percent() >= 0.5;
}

}

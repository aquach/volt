#pragma once

namespace Volt {

class Random {
public:
    static void Seed ();
    static void Seed (unsigned int seed);
    static int Range (int start, int end);
    static float Percent ();
    static bool CoinFlip ();
    static float RangeFloat (float start, float end);
};

}

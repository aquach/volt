#include "Math.h"

namespace Volt {
    
float RoundToNearest (float number, float divisor) {
    number += divisor / 2;
    return number - fmodf(number, divisor);
}

}

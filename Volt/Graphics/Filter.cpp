#include "Volt/Graphics/Filter.h"

namespace Volt {

Filter::Filter (int bottomLayer, int topLayer)
    : m_bottomLayer(bottomLayer),
      m_topLayer(topLayer) {
    CHECK_GE(bottomLayer, topLayer);
}

}

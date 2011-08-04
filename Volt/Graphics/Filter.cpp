#include "Volt/Graphics/Filter.h"

namespace Volt {

Filter::Filter (const string& name, int bottomLayer, int topLayer)
    : m_name(name),
      m_bottomLayer(bottomLayer),
      m_topLayer(topLayer) {
    CHECK_GE(bottomLayer, topLayer);
}

}

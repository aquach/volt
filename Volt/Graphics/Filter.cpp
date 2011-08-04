#include "Volt/Graphics/Filter.h"

namespace Volt {

Filter::Filter (const string& name, int bottomLayer, int topLayer)
    : m_name(name),
      m_enabled(true) {
    m_bottomLayer = MAX(bottomLayer, topLayer);
    m_topLayer = MIN(bottomLayer, topLayer);
}

}

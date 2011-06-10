#include "Layer.h"
#include "Graphic.h"

namespace Volt {

void Layer::Render () {
    for (list<Graphic*>::iterator i = m_graphics.begin(); i != m_graphics.end();
         i++) {
        (*i)->Render();
    }
}

}

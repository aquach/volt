#pragma once

#include "Volt/Core/Core.h"

namespace Volt {

/** A filter, in the form of a shader, to be applied to the screen. Filters
 *  allow people to add multi-pass full-screen effects using shaders. Filters
 *  specify a layer range, and will get calls when those layer is rendered
 *  so that the filter can do things like change a FBO or render something
 *  special. */
class Filter {
public:
    Filter (int bottomLayer, int topLayer);
    virtual ~Filter () { }

    /* Called just before any entities in the bottom layer or higher are
     * rendered. */
    virtual void OnBottomLayer () = 0;
    /* Called just after any entities in the top layer or lower are
     * rendered. */
    virtual void OnTopLayer () = 0;

    int bottomLayer () const { return m_bottomLayer; }
    int topLayer () const { return m_topLayer; }

private:
    friend class Scene;
    int m_bottomLayer;
    int m_topLayer;

    DISALLOW_COPY_AND_ASSIGN(Filter);
};

}

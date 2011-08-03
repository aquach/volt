#pragma once

#include "Volt/Core/Core.h"

namespace Volt {
    
/**
 *  Manages the global progression of time by providing access to dt().
 */
class AppTime {
public:
    AppTime () : m_dt(0) { }
    
    float dt () const { return m_dt; }
    void SetDt (float dt) { m_dt = dt; }
    
    static void Register (AppTime* time) { instance = time; }
    static AppTime* Instance () { return instance; }

private:
    float m_dt;

    static AppTime* instance;

    DISALLOW_COPY_AND_ASSIGN(AppTime);
};

#define G_Time Volt::AppTime::Instance()

}

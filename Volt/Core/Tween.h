#pragma once

#include <vector>

using namespace std;

namespace Volt {

template <typename T> class Tween {
public:
    void Update (float dt);
    T value () const;

    static Tween<T> None (T start, T end, float duration);
    static Tween<T> Linear (T start, T end, float duration);
    static Tween<T> QuadraticIn (T start, T end, float duration);
    static Tween<T> QuadraticOut (T start, T end, float duration);
    static Tween<T> QuadraticInOut (T start, T end, float duration);
    static Tween<T> SinIn (T start, T end, float duration);
    static Tween<T> SinOut (T start, T end, float duration);
    static Tween<T> SinInOut (T start, T end, float duration);

private:
    friend class CompositeTween;

    enum EaseType {
        EASE_NONE,
        EASE_LINEAR,
        EASE_INOUTSIN,
        EASE_INSIN,
        EASE_OUTSIN,
        EASE_INQUAD,
        EASE_OUTQUAD,
        EASE_INOUTQUAD
    };

    Tween ()
        : m_t(0),
          m_duration(1),
          m_type(EASE_LINEAR) {
    }

    T m_value;
    T m_start;
    T m_end;
    float m_t;
    float m_duration;
    EaseType m_type;
};

template <typename T> class CompositeTween {
public:
    CompositeTween () :
        m_t(0),
        m_currentTween(0) {
    }
    ~CompositeTween ();

    void Update (float dt);
    void AddTween (Tween<T> tween);
    T value ();

private:
    vector<Tween<T> > m_tweens;
    int m_currentTween;
};

}

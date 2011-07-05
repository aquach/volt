#include "Tween.h"
#include "Math.h"

namespace Volt {

template<typename T> void Tween<T>::Update (float dt) {
    m_t += dt;

    float t = m_t / m_duration;
    if (t >= 1.0f)
        t = 1.0f;

    T dv = m_end - m_start;

    switch (m_type) {
        case EASE_NONE:
            m_value = m_start;
        break;
        case EASE_LINEAR:
            m_value = m_start + dv * t;
        break;
        case EASE_INQUAD:
            m_value = m_start + dv * (t * t);
        break;
        case EASE_OUTQUAD:
            m_value = m_start + dv * (2 * t - t * t);
        break;
        case EASE_INOUTQUAD:
            if (t < 0.5f)
                m_value = m_start + dv * (t * t * 0.5f);
            else
                m_value = m_start - dv * (0.5f * (t * (t - 2) - 1));
        break;
        case EASE_INSIN:
            m_value = m_start + dv * cos(t * PI / 2);
        break;
        case EASE_OUTSIN:
            m_value = m_start + dv * sin(t * PI / 2);
        break;
        case EASE_INOUTSIN:
            m_value = m_start + dv * (0.5f * (cos(t * PI) - 1));
        break;
        default:
        break;
    }
}

template <typename T> T Tween<T>::value () const {
    return m_value;
}

template <typename T> Tween<T> Tween<T>::Linear (T start, T end,
                                                 float duration) {
    return Tween(EASE_LINEAR, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::QuadraticIn (T start, T end,
                                                      float duration) {
    return Tween(EASE_INQUAD, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::QuadraticOut (T start, T end,
                                                       float duration) {
    return Tween(EASE_OUTQUAD, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::QuadraticInOut (T start, T end,
                                                         float duration) {
    return Tween(EASE_INOUTQUAD, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::SinIn (T start, T end,
                                                float duration) {
    return Tween(EASE_INSIN, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::SinOut (T start, T end,
                                                 float duration) {
    return Tween(EASE_OUTSIN, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::SinInOut (T start, T end,
                                                   float duration) {
    return Tween(EASE_INOUTSIN, start, end, duration);
}

template <typename T> void CompositeTween<T>::Update (float dt) {
    m_t += dt;
    while (m_t >= m_tweens[m_currentTween].m_duration &&
           m_currentTween < m_tweens.size() - 1) {
        m_currentTween++;
        m_t -= m_tweens[m_currentTween].m_duration;
    }
    m_tweens[m_currentTween].Update(m_t);
}

template <typename T> void CompositeTween<T>::AddTween (Tween<T> tween) {
    m_tweens.push_back(tween);
}

template <typename T> T CompositeTween<T>::value () {
    return m_tweens[m_currentTween].value();
}

}

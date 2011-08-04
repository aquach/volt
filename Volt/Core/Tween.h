#pragma once

#include <vector>
#include "Logging.h"
#include "Math.h"

using namespace std;

namespace Volt {

template <typename T> class Tween {
public:
    void Update (float dt);
    void SetTime (float time);
    T value () const { return m_value; }
    bool finished () const { return m_t >= m_duration; }
    float duration () const { return m_duration; }
    float time () const { return m_t; }

    static Tween<T> NoTween (T start, T end, float duration);
    static Tween<T> Linear (T start, T end, float duration);
    static Tween<T> QuadraticIn (T start, T end, float duration);
    static Tween<T> QuadraticOut (T start, T end, float duration);
    static Tween<T> QuadraticInOut (T start, T end, float duration);
    static Tween<T> SinIn (T start, T end, float duration);
    static Tween<T> SinOut (T start, T end, float duration);
    static Tween<T> SinInOut (T start, T end, float duration);

private:

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

    Tween (EaseType type, T start, T end, float duration)
        : m_t(0),
          m_type(type),
          m_start(start),
          m_end(end),
          m_duration(duration) {
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
    CompositeTween () : m_currentTween(0) {
    }
    ~CompositeTween () { }

    void Update (float dt);
    void SetTime (float time);
    void AddTween (Tween<T> tween);
    T value ();

private:
    vector<Tween<T> > m_tweens;
    int m_currentTween;
};


template<typename T> void Tween<T>::Update (float dt) {
    SetTime(m_t + dt);
}

template<typename T> void Tween<T>::SetTime (float time) {
    m_t = time;
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
            t *= 2;
            if (t < 1.0f) {
                m_value = m_start + dv * (t * t * 0.5f);
            } else {
                t--;
                m_value = m_start - dv * (0.5f * (t * (t - 2) - 1));
            }
        break;
        case EASE_INSIN:
            m_value = m_start + dv * (1 - cos(t * PI / 2));
        break;
        case EASE_OUTSIN:
            m_value = m_start + dv * sin(t * PI / 2);
        break;
        case EASE_INOUTSIN:
            m_value = m_start + dv * (-0.5f * (cos(t * PI) - 1));
        break;
        default:
        break;
    }
}

template <typename T> Tween<T> Tween<T>::NoTween (T start, T end,
                                                 float duration) {
    return Tween<T>(EASE_NONE, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::Linear (T start, T end,
                                                 float duration) {
    return Tween<T>(EASE_LINEAR, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::QuadraticIn (T start, T end,
                                                      float duration) {
    return Tween<T>(EASE_INQUAD, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::QuadraticOut (T start, T end,
                                                       float duration) {
    return Tween<T>(EASE_OUTQUAD, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::QuadraticInOut (T start, T end,
                                                         float duration) {
    return Tween<T>(EASE_INOUTQUAD, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::SinIn (T start, T end,
                                                float duration) {
    return Tween<T>(EASE_INSIN, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::SinOut (T start, T end,
                                                 float duration) {
    return Tween<T>(EASE_OUTSIN, start, end, duration);
}

template <typename T> Tween<T> Tween<T>::SinInOut (T start, T end,
                                                   float duration) {
    return Tween<T>(EASE_INOUTSIN, start, end, duration);
}

template <typename T> void CompositeTween<T>::SetTime (float time) {
    if (m_tweens.size() == 0)
        return;

    uint i = 0;
    for (; i < m_tweens.size(); i++) {
        Tween<T>* tween = &m_tweens[i];
        float amount = MIN(tween->duration(), time);
        tween->SetTime(amount);
        time -= amount;
        if (time <= 0) {
            m_currentTween = i;
            break;
        }
    }
    CHECK_GE(time, 0);
    i++;
    for (; i < m_tweens.size(); i++)
        m_tweens[i].SetTime(0);

    m_tweens[m_tweens.size() - 1].Update(time);
    if (time > 0)
        m_currentTween = m_tweens.size() - 1;
}

template <typename T> void CompositeTween<T>::Update (float dt) {
    m_tweens[m_currentTween].Update(dt);
    while (
        m_tweens[m_currentTween].time() >= m_tweens[m_currentTween].duration()
        && m_currentTween < (int)m_tweens.size() - 1) {
        dt -= m_tweens[m_currentTween].time() -
              m_tweens[m_currentTween].duration();
        m_currentTween++;
        m_tweens[m_currentTween].Update(dt);
    }
}

template <typename T> void CompositeTween<T>::AddTween (Tween<T> tween) {
    m_tweens.push_back(tween);
}

template <typename T> T CompositeTween<T>::value () {
    return m_tweens[m_currentTween].value();
}

}

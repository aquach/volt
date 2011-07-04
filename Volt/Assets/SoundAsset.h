#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/Asset.h"

namespace Volt {

const int NUM_BUFFERS = 2;

class DataItem;
class SoundManager;
class SingleSound;
class MultipleSound;

/* A sound file. */
class SoundAsset : public Asset {
public:
    enum SoundType {
        SOUND_SINGLE,  // For music: at most one instance can be playing.
        SOUND_MULTIPLE // For sound effects: multiple instances can be playing.
    };
    enum SoundTransition {
        TRANSITION_NONE,
        TRANSITION_FADE
    };

    SoundAsset ();
    ~SoundAsset ();

    bool Load (const DataItem& item, SoundType type, float volume = 1.0);

    virtual void Reload ();
    virtual void Unload ();

    /** Plays the sound file. Returns true if the music is playing or was
     *  succesfully started. If a SINGLE sound, does nothing if the sound
     *  is playing. If a MULTIPLE sound, starts playing the sound again. */
    void Play ();
    void PlayAt (Vector2 source, float volume);

    /** These only work on SINGLE sounds. */
    bool IsPlaying ();
    void Stop ();
    void Pause ();
    bool IsPaused ();

    void Update ();

private:
    SingleSound* m_single;
    MultipleSound* m_multiple;
    float m_volume;
    SoundType m_type;
};

}

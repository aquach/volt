#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/AssetManager.h"

namespace Volt {

class Entity;

class SpriteAnimation {
public:
    SpriteAnimation (Entity* e, DataAssetRef animationData);
    SpriteAnimation (Entity* e, const string& animationDataPath);
    ~SpriteAnimation () { }

    bool completed () const;
    void Update ();
    void Render ();
    void PlayTrack (const string& trackName);
    const Json::Value& frameUserData ();
    void ReloadSprites ();

    void SetXFlip (bool flip) {
        m_transform.scale.x = abs(m_transform.scale.x) * (flip ? 1 : -1);
    }

private:
    struct AnimationFrame {
        int frameLength;
        // Additional data stored in the animation file about each frame,
        // such as attach points for weapons.
        Json::Value userData;
        TextureAssetRef texture;
    };
    struct AnimationTrack {
        float t;
        float duration;
        bool loops;
        bool seesaws;
        string nextTrack;
        // Array of indices into frames, where each element corresponds
        // to a unit of time. So a frame with frame length 2 would have
        // two entries in frameIndices. frameIndices essentially expands the
        // duration and the seesaws into a complete array for easy access
        // in rendering.
        vector<int> frameIndices;
        vector<AnimationFrame> frames;
    };

    void Load (DataAssetRef animationData);
    AnimationFrame* currentFrame ();

    typedef map<string, AnimationTrack> Tracks;
    Tracks m_tracks;
    Entity* m_entity;
    AnimationTrack* m_currentTrack;
    Transform m_transform;
    DataAssetRef m_animationData;

    DISALLOW_COPY_AND_ASSIGN(SpriteAnimation);
};

}

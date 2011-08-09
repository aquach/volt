#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/AssetManager.h"

namespace Volt {

class Entity;

class SpriteAnimation {
public:
    SpriteAnimation (Entity* e, DataAssetRef animationData);
    ~SpriteAnimation () { }

    void Update ();
    void Render ();
    void PlayTrack (const string& trackName);

    void ReloadSprites ();

private:
    struct AnimationTrack {
        float t;
        float duration;
        bool loops;
        vector<TextureAssetRef> frames;
    };

    void Load (DataAssetRef animationData);

    typedef map<string, AnimationTrack> Tracks;
    Tracks m_tracks;
    Entity* m_entity;
    AnimationTrack* m_currentTrack;
    Transform m_transform;
    DataAssetRef m_animationData;

    DISALLOW_COPY_AND_ASSIGN(SpriteAnimation);
};

}

#include "Volt/Graphics/SpriteAnimation.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Game/Entity.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/OpenGL.h"

namespace Volt {

SpriteAnimation::SpriteAnimation (Entity* e, DataAssetRef animationData)
    : m_entity(e),
      m_currentTrack(NULL) {

    Load(animationData);
}

void SpriteAnimation::Load (DataAssetRef animationData) {
    m_animationData = animationData;

    const Json::Value& node = animationData->data();
    m_transform.Load(node["transform"]);
    for (uint i = 0; i < node["tracks"].size(); i++) {
        const Json::Value& trackNode = node["tracks"][i];
        string name = trackNode["name"].asString();

        AnimationTrack& track = m_tracks[name];
        track.t = 0;
        track.duration = trackNode["duration"].asDouble();
        track.loops = trackNode["loops"].asBool();
        for (uint j = 0; j < trackNode["frames"].size(); j++) {
            string path = trackNode["frames"][j].asString();
            track.frames.push_back(G_AssetManager->GetTexture(path));
        }
    }

    TextureAssetRef refTexture = m_tracks.begin()->second.frames[0];
    if (m_transform.scale.x == 0) {
        m_transform.scale.x = (float)refTexture->width() / refTexture->height()
            * m_transform.scale.y;
    }
    if (m_transform.scale.y == 0) {
        m_transform.scale.y = (float)refTexture->height() / refTexture->width()
            * m_transform.scale.x;
    }

    string startTrack = node.get("startTrack", "").asString();
    if (startTrack.size() > 0)
        PlayTrack(startTrack);
    else
        PlayTrack(m_tracks.begin()->first);
}

void SpriteAnimation::Update () {
    CHECK_NOTNULL(m_currentTrack);
    m_currentTrack->t += G_Time->dt();
}

void SpriteAnimation::PlayTrack (const string& trackName) {
    Tracks::iterator iter = m_tracks.find(trackName);
    CHECK(iter != m_tracks.end()) << "Could not find animation track "
        << trackName;
    m_currentTrack = &iter->second;
    m_currentTrack->t = 0;
}

void SpriteAnimation::Render () {
    CHECK_NOTNULL(m_currentTrack);
    CHECK_GE(m_currentTrack->frames.size(), 0);

    glPushMatrix();
    Graphics::TransformMatrix(m_entity->transform());
    Graphics::TransformMatrix(m_transform);
    Graphics::SetBlend(Graphics::BLEND_ALPHA);
    Graphics::SetColor(Color::white);

    float percent = m_currentTrack->t / m_currentTrack->duration;
    if (percent >= 1.0f) {
        if (m_currentTrack->loops) {
            percent = fmodf(m_currentTrack->t, m_currentTrack->duration);
        } else {
            percent = 0.999f;
        }
    }
    int frame = (int)(percent * m_currentTrack->frames.size());

    Graphics::BindTexture(m_currentTrack->frames[frame]);
    Graphics::RenderQuad(1, 1);
    Graphics::BindTexture(NULL);
    Graphics::SetBlend(Graphics::BLEND_NONE);
    glPopMatrix();
}

void SpriteAnimation::ReloadSprites () {
    FOR_(Tracks::iterator, iter, m_tracks) {
        for (uint i = 0; i < iter->second.frames.size(); i++)
            iter->second.frames[i]->Reload();
    }
    m_animationData->Reload();
    Load(m_animationData);
}

}

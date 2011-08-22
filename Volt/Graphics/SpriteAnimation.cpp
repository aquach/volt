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

SpriteAnimation::SpriteAnimation (Entity* e, const string& animationDataPath)
    : m_entity(e),
      m_currentTrack(NULL) {
    Load(G_AssetManager->GetData(animationDataPath));
}

const Json::Value& SpriteAnimation::frameUserData () {
    return currentFrame()->userData;
}

void SpriteAnimation::Load (DataAssetRef animationData) {
    m_animationData = animationData;
    m_tracks.clear();

    const Json::Value& node = animationData->data();
    m_transform.Load(node["transform"]);
    for (uint i = 0; i < node["tracks"].size(); i++) {
        const Json::Value& trackNode = node["tracks"][i];
        string name = trackNode["name"].asString();

        AnimationTrack& track = m_tracks[name];
        track.t = 0;
        track.duration = trackNode["duration"].asDouble();
        track.loops = trackNode.get("loops", false).asBool();
        track.seesaws = trackNode.get("seesaws", false).asBool();
        track.nextTrack = trackNode.get("nextTrack", "").asString();
        if (track.nextTrack.size() > 0 && track.loops) {
            LOG(WARNING) << "Next track set for track '" << name << "', but "
                         << "track loops.";
        }

        for (uint frameIndex = 0; frameIndex < trackNode["frames"].size();
            frameIndex++) {
            const Json::Value& frameNode = trackNode["frames"][frameIndex];
            AnimationFrame frame;
            string path;
            if (frameNode.size() == 0) {
                // frameNode is a string indicating texture.
                path = frameNode.asString();
                frame.frameLength = 1;
            } else {
                // Read additional information.
                path = frameNode["texture"].asString();
                frame.frameLength = frameNode.get("length", 1).asInt();
                frame.userData = frameNode;
            }
            frame.texture = G_AssetManager->GetTexture(path);
            track.frames.push_back(frame);
            for (int count = 0; count < frame.frameLength; count++)
                track.frameIndices.push_back(frameIndex);
        }

        if (track.seesaws) {
            // Concatenate all frames backwards.
            for (int i = (int)track.frameIndices.size() - 1; i >= 0; i--) {
                track.frameIndices.push_back(track.frameIndices[i]);
            }
        }
    }

    AnimationFrame& frame = m_tracks.begin()->second.frames[0];
    TextureAssetRef refTexture = frame.texture;
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

    // Transition to the next track if time is up.
    if (m_currentTrack->t >= m_currentTrack->duration &&
        !m_currentTrack->loops && m_currentTrack->nextTrack.size() > 0) {
        PlayTrack(m_currentTrack->nextTrack);
    }
}

void SpriteAnimation::PlayTrack (const string& trackName) {
    Tracks::iterator iter = m_tracks.find(trackName);
    CHECK(iter != m_tracks.end()) << "Could not find animation track "
        << trackName;
    // Do nothing if track is already playing and it's a looping track.
    if (&iter->second == m_currentTrack && m_currentTrack->loops)
        return;
    m_currentTrack = &iter->second;
    m_currentTrack->t = 0;
}

SpriteAnimation::AnimationFrame* SpriteAnimation::currentFrame () {
    CHECK_NOTNULL(m_currentTrack);
    CHECK_GE(m_currentTrack->frames.size(), 0);

    float percent = m_currentTrack->t / m_currentTrack->duration;
    if (percent >= 1.0f) {
        if (m_currentTrack->loops) {
            float loopT = fmodf(m_currentTrack->t, m_currentTrack->duration);
            percent = loopT / m_currentTrack->duration;
        } else {
            percent = 0.999f;
        }
    }
    int index = (int)(percent * m_currentTrack->frameIndices.size());
    return &m_currentTrack->frames[m_currentTrack->frameIndices[index]];
}

bool SpriteAnimation::completed () const {
    float percent = m_currentTrack->t / m_currentTrack->duration;
    return !m_currentTrack->loops && percent >= 1.0f;
}

void SpriteAnimation::Render () {
    glPushMatrix();
    Graphics::TransformMatrix(m_entity->transform());
    Graphics::TransformMatrix(m_transform);
    Graphics::SetBlend(Graphics::BLEND_ALPHA);
    Graphics::SetColor(Color::white);
    // For transparent parts of the picture, don't draw into depth.
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    AnimationFrame* renderFrame = currentFrame();

    Graphics::BindTexture(renderFrame->texture);
    Graphics::RenderQuad(1, 1);
    Graphics::BindTexture(NULL);
    glDisable(GL_ALPHA_TEST);
    Graphics::SetBlend(Graphics::BLEND_NONE);
    glPopMatrix();
}

void SpriteAnimation::ReloadSprites () {
    FOR_(Tracks::iterator, iter, m_tracks) {
        for (uint i = 0; i < iter->second.frames.size(); i++)
            iter->second.frames[i].texture->Reload();
    }
    m_animationData->Reload();
    Load(m_animationData);
}

}

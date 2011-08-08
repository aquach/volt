#include "Volt/Graphics/SpriteAnimation.h"

SpriteAnimation::SpriteAnimation (Entity* e, DataAssetRef animationData)
    : m_entity(e),
      m_currentTrack(NULL) {

}

void SpriteAnimation::Update () {

}

void SpriteAnimation::PlayTrack (const string& trackName) {
    Tracks::iterator iter = m_tracks.find(id);
    CHECK(iter != m_tracks.end()) << "Could not find animation track "
        << trackName;
    m_currentTrack = iter->second;
    m_currentTrack->t = 0;
}

void SpriteAnimation::Render () {
    CHECK_NOTNULL(m_currentTrack);
    CHECK_GE(m_currentTrack->frame.size(), 0);

    glPushMatrix();
    Graphics::TransformMatrix(m_entity->transform());
    float percent = t / duration;
    if (percent > 1.0f) {
        if (m_currentTrack->loops) {
            percent = fmodf(t, duration);
        } else {
            percent = 1.0f;
        }
    }
    int frame = (int)(percent * (m_currentTrack->frames.size() - 1));
    Graphics::BindTexture(m_currentTrack->frames[frame]);
    //Graphics::RenderQuad()
    // TODO: Size of animation? Interaction between entity's game transform
    // and actual animation transform?
    Graphics::BindTexture(NULL);
    glPopMatrix();
}

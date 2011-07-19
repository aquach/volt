#include "Game/Game/GamePerfHook.h"
#include "Volt/Game/Entity.h"

GamePerfHook::GamePerfHook ()
    : m_renderTotal(0),
      m_updateTotal(0) {
}

GamePerfHook::~GamePerfHook () {
    LOG(INFO) << "== UPDATE PERFORMANCE ==";
    FOR_(Times::iterator, i, m_updateTimes) {
        int percent = (int)((float)i->second / m_updateTotal * 100 + 0.5f);
        int microsecs = (int)((float)i->second / m_counts[i->first]);
        LOG(INFO) << i->first << ": " << microsecs << " microsecs/entity"
                  << " (" << percent << "% of all update time)";
    }

    LOG(INFO) << "== RENDER PERFORMANCE ==";
    FOR_(Times::iterator, i, m_renderTimes) {
        int percent = (int)((float)i->second / m_renderTotal * 100 + 0.5f);
        int microsecs = (int)((float)i->second / m_counts[i->first]);
        LOG(INFO) << i->first << ": " << microsecs << " microsecs/entity"
                  << " (" << percent << "% of all render time)";
    }
}

void GamePerfHook::OnEntityRenderStart (Volt::Entity* entity) {
    m_usecs = Volt::GetMicroseconds();
}

void GamePerfHook::OnEntityRenderEnd (Volt::Entity* entity) {
    long elapsed = Volt::GetMicroseconds() - m_usecs;
    if (elapsed < 0)
        return;
    vector<string> tags;
    entity->GetTags(&tags);

    string key;
    for (uint i = 0; i < tags.size(); i++) {
        key += tags[i];
        if (i != tags.size() - 1)
            key += ", ";
    }
    m_renderTimes[key] += elapsed;
    m_counts[key]++;
    m_renderTotal += elapsed;
}

void GamePerfHook::OnEntityUpdateStart (Volt::Entity* entity) {
    m_usecs = Volt::GetMicroseconds();
}

void GamePerfHook::OnEntityUpdateEnd (Volt::Entity* entity) {
    long elapsed = Volt::GetMicroseconds() - m_usecs;
    if (elapsed < 0)
        return;
    vector<string> tags;
    entity->GetTags(&tags);

    string key;
    for (uint i = 0; i < tags.size(); i++) {
        key += tags[i];
        if (i != tags.size() - 1)
            key += ", ";
    }
    m_updateTimes[key] += elapsed;
    m_counts[key]++;
    m_updateTotal += elapsed;
}

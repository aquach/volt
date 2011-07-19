#include "Game/Game/GamePerfHook.h"
#include "Volt/Game/Entity.h"

GamePerfHook::GamePerfHook ()
    : m_renderTotal(0),
      m_updateTotal(0) {
}

GamePerfHook::~GamePerfHook () {
    LOG(PERF) << "== UPDATE PERFORMANCE ==";
    FOR_(Times::iterator, i, m_updateTimes) {
        int percent = (int)((float)i->second / m_updateTotal * 100 + 0.5f);
        int microsecs = (int)((float)i->second / m_counts[i->first]);
        LOG(PERF) << i->first << ": " << microsecs << " usecs/entity"
                  << " (" << percent << "% of all update time)";
    }

    LOG(PERF) << "== RENDER PERFORMANCE ==";
    FOR_(Times::iterator, i, m_renderTimes) {
        int percent = (int)((float)i->second / m_renderTotal * 100 + 0.5f);
        int microsecs = (int)((float)i->second / m_counts[i->first]);
        LOG(PERF) << i->first << ": avg " << microsecs << " usecs/entity"
                  << " (" << percent << "%) "
                  << "max " << m_maxRenderTimes[i->first] << " usecs";
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

    m_maxRenderTimes[key] = MAX(m_maxRenderTimes[key], elapsed);
    
    LOG(INFO) << key << " took " << elapsed << " microsecs";
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

void GamePerfHook::OnRenderStart () {
    m_renderTime = Volt::GetMicroseconds();
}

void GamePerfHook::OnRenderEnd () {
    long elapsed = Volt::GetMicroseconds() - m_renderTime;
    if (elapsed < 0)
        return;
    LOG(INFO) << "Render: " << elapsed / 1000 << " ms";
}

void GamePerfHook::OnUpdateStart () {
    m_updateTime = Volt::GetMicroseconds();
}

void GamePerfHook::OnUpdateEnd () {
    long elapsed = Volt::GetMicroseconds() - m_updateTime;
    if (elapsed < 0)
        return;
    LOG(INFO) << "Update: " << elapsed / 1000 << " ms";
}


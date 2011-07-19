#include "Game/Game/GamePerfHook.h"
#include "Volt/Game/Entity.h"

GamePerfHook::GamePerfHook ()
    : m_total(0) {
}

GamePerfHook::~GamePerfHook () {
    LOG(INFO) << "== PERFORMANCE ==";
    FOR_(Times::iterator, i, m_times) {
        int percent = (int)((float)i->second / m_total * 100 + 0.5f);
        int microsecs = (int)((float)i->second / m_counts[i->first]);
        LOG(INFO) << i->first << ": " << microsecs << " microsecs/entity"
                  << " (" << percent << "% of all render time)";
    }
    LOG(INFO) << "=================";
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
    m_times[key] += elapsed;
    m_counts[key]++;
    m_total += elapsed;
}

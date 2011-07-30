#include "Volt/Assets/ScriptAsset.h"
#include <fstream>
#include "Volt/Assets/AssetManager.h"
#include "Volt/Assets/DataSource.h"


namespace Volt {

ScriptAsset::ScriptAsset () {
}

ScriptAsset::~ScriptAsset () {
    Unload();
}

bool ScriptAsset::Load (const DataItem& item) {
    m_path = item.path;

    m_script.assign(item.data, item.size);
    return true;
}

void ScriptAsset::Reload () {
    Unload();
    DataItem item;
    m_manager->ReloadPath(this, &item);
    Load(item);
}

void ScriptAsset::Unload () {
    m_script.clear();
}

}

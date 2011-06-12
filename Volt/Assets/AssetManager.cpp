#include "AssetManager.h"
#include "DataSource.h"
#include "TextureAsset.h"

namespace Volt {

AssetManager::AssetManager (DataSource* source)
    : m_dataSource(source) {
}

AssetManager::~AssetManager () {
    delete m_dataSource;
    for (Assets::iterator i = m_assets.begin(); i != m_assets.end(); i++)
        delete i->second;
}

void AssetManager::ReloadAll () {
    for (Assets::iterator i = m_assets.begin(); i != m_assets.end(); i++)
        i->second->Reload();
}

void AssetManager::ReloadPath (Asset* asset, DataItem* item) {
    m_dataSource->LoadDataItem(asset->path(), item);
}

AssetRef<TextureAsset> AssetManager::RequestTexture (
    const string& path, TextureAsset::FilterType type, bool repeatX,
    bool repeatY) {

    Assets::iterator iter = m_assets.find(path);
    if (iter != m_assets.end()) {
        Asset* asset = iter->second;
        TextureAsset* tex = dynamic_cast<TextureAsset*>(asset);
        if (tex == NULL) {
            LOG(WARNING) << "Requested path " << path << " was not a texture.";
            return NULL;
        }
        return AssetRef<TextureAsset>(tex);
    }

    DataItem item;
    if (!m_dataSource->LoadDataItem(path, &item)) {
        LOG(WARNING) << "Failed to load data item " << path;
        return NULL;
    }

    TextureAsset* texture = new TextureAsset();
    texture->m_manager = this;
    if (!texture->Load(item, type, repeatX, repeatY)) {
        LOG(WARNING) << "Failed to load texture " << path;
        delete texture;
        return NULL;
    }

    m_assets[path] = texture;

    return AssetRef<TextureAsset>(texture);
}

void AssetManager::GarbageCollect () {
    for (Assets::iterator i = m_assets.begin(); i != m_assets.end();) {
        if (i->second->referenceCount() == 0) {
            LOG(INFO) << "Reclaiming " << i->second->path();
            Assets::iterator del = i;
            i++;
            delete del->second;
            m_assets.erase(del);
        } else {
            i++;
        }
    }
}

}

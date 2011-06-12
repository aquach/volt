#include "AssetManager.h"
#include "DataSource.h"
#include "TextureAsset.h"

namespace Volt {

AssetManager::AssetManager (const DataSource* source)
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

void AssetManager::ReloadPath (const Asset* asset, DataItem* item) {
    m_dataSource->LoadDataItem(asset->path(), item);
}

AssetRef<FontAsset> AssetManager::RequestFont (
    const string& path, float size, int textureWidth, int textureHeight) {

    AssetRef<FontAsset> asset = GetAssetByName<FontAsset>(path);
    if (asset.HasAsset())
        return asset;

    DataItem item;
    if (!m_dataSource->LoadDataItem(path, &item)) {
        LOG(WARNING) << "Failed to load data item " << path;
        return NULL;
    }

    FontAsset* font = new FontAsset(textureWidth, textureHeight);
    font->m_manager = this;
    if (!font->Load(item, size)) {
        LOG(WARNING) << "Failed to load font " << path;
        delete font;
        return NULL;
    }

    m_assets[path] = font;

    return AssetRef<FontAsset>(font);
}

template <class T> AssetRef<T> AssetManager::GetAssetByName (
    const string& path) {
    Assets::iterator iter = m_assets.find(path);
    if (iter != m_assets.end()) {
        Asset* asset = iter->second;
        T* specialAsset = dynamic_cast<T*>(asset);
        if (specialAsset == NULL) {
            LOG(WARNING) << "Requested path " << path << " was not "
                         << "the type expected.";
            return NULL;
        }
        return AssetRef<T>(specialAsset);
    }
    return NULL;
}

AssetRef<TextureAsset> AssetManager::RequestTexture (
    const string& path, TextureAsset::FilterType type, bool repeatX,
    bool repeatY) {

    AssetRef<TextureAsset> asset = GetAssetByName<TextureAsset>(path);
    if (asset.HasAsset())
        return asset;

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

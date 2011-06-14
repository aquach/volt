#include "AssetManager.h"
#include "DataSource.h"
#include "TextureAsset.h"

namespace Volt {

AssetManager* AssetManager::instance = NULL;

AssetManager::AssetManager (const DataSource* source)
    : m_dataSource(source) {
}

AssetManager::~AssetManager () {
    delete m_dataSource;
    for (Assets::iterator i = m_assets.begin(); i != m_assets.end(); i++) {
        DCHECK_EQ(i->second->referenceCount(), 0);
        delete i->second;
    }
}

void AssetManager::ReloadAll () {
    for (Assets::iterator i = m_assets.begin(); i != m_assets.end(); i++)
        i->second->Reload();
}

void AssetManager::ReloadPath (const Asset* asset, DataItem* item) {
    m_dataSource->LoadDataItem(asset->path(), item);
}

FontAssetRef AssetManager::GetFont (
    const string& path, float size, int textureWidth, int textureHeight) {

    FontAssetRef asset = GetAssetByKey<FontAsset>(path);
    if (asset.HasAsset())
        return asset;

    DataItem item;
    if (!m_dataSource->LoadDataItem(path, &item)) {
        LOG(ERROR) << "Failed to load data item " << path;
        return NULL;
    }

    FontAsset* font = new FontAsset(textureWidth, textureHeight);
    font->m_manager = this;
    if (!font->Load(item, size)) {
        LOG(ERROR) << "Failed to load font " << path;
        delete font;
        return NULL;
    }

    m_assets[font->assetKey()] = font;

    return FontAssetRef(font);
}

template <class T> AssetRef<T> AssetManager::GetAssetByKey (
    const string& path) {
    Assets::iterator iter = m_assets.find(path);
    if (iter != m_assets.end()) {
        Asset* asset = iter->second;
        T* specialAsset = dynamic_cast<T*>(asset);
        if (specialAsset == NULL) {
            LOG(ERROR) << "Requested path " << path << " was not "
                       << "the type expected.";
            return NULL;
        }
        return AssetRef<T>(specialAsset);
    }
    return NULL;
}

TextureAssetRef AssetManager::GetTexture (
    const string& path, TextureAsset::FilterType type, bool repeatX,
    bool repeatY) {

    TextureAssetRef asset = GetAssetByKey<TextureAsset>(path);
    if (asset.HasAsset())
        return asset;

    DataItem item;
    if (!m_dataSource->LoadDataItem(path, &item)) {
        LOG(ERROR) << "Failed to load data item " << path;
        return NULL;
    }

    TextureAsset* texture = new TextureAsset();
    texture->m_manager = this;
    if (!texture->Load(item, type, repeatX, repeatY)) {
        LOG(ERROR) << "Failed to load texture " << path;
        delete texture;
        return NULL;
    }

    m_assets[texture->assetKey()] = texture;

    return TextureAssetRef(texture);
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

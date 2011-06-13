#pragma once

#include "Core/Core.h"
#include "AssetRef.h"
#include "FontAsset.h"
#include "TextureAsset.h"

namespace Volt {

class DataSource;

typedef AssetRef<TextureAsset> TextureAssetRef;
typedef AssetRef<FontAsset> FontAssetRef;

/* Manages all assets, including asset loading/reloading. */
class AssetManager {
public:
    AssetManager (const DataSource* source);
    ~AssetManager ();

    TextureAssetRef GetTexture (
        const string& path,
        TextureAsset::FilterType type = TextureAsset::e_filterTypeLinear,
        bool repeatX = true, bool repeatY = true);
    FontAssetRef GetFont (
        const string& path, float size, int textureWidth = 1024,
        int textureHeight = 1024);

    void GarbageCollect ();

    void ReloadAll ();
    void ReloadPath (const Asset* asset, DataItem* item);

private:
    template <class T> AssetRef<T> GetAssetByName (const string& path);

    const DataSource* m_dataSource;
    typedef map<string, Asset*> Assets;
    Assets m_assets;

    DISALLOW_COPY_AND_ASSIGN(AssetManager);
};

}

#pragma once

#include "Core/Core.h"
#include "AssetRef.h"
#include "DataAsset.h"
#include "FontAsset.h"
#include "ShaderAsset.h"
#include "SoundAsset.h"
#include "TextureAsset.h"

namespace Volt {

class DataSource;

typedef AssetRef<DataAsset> DataAssetRef;
typedef AssetRef<FontAsset> FontAssetRef;
typedef AssetRef<ShaderAsset> ShaderAssetRef;
typedef AssetRef<SoundAsset> SoundAssetRef;
typedef AssetRef<TextureAsset> TextureAssetRef;

/* Manages all assets, including asset loading/reloading. */
class AssetManager {
public:
    AssetManager (const DataSource* source);
    ~AssetManager ();

    TextureAssetRef GetTexture (
        const string& path,
        TextureAsset::FilterType type = TextureAsset::FILTER_LINEAR,
        bool repeatX = true, bool repeatY = true);
    FontAssetRef GetFont (
        const string& path, float size, int textureWidth = 1024,
        int textureHeight = 1024);
    ShaderAssetRef GetShader (const string& path,
                              ShaderAsset::ShaderType type);
    SoundAssetRef GetSound (
        const string& path,
        SoundAsset::SoundType type = SoundAsset::SOUND_SINGLE,
        float globalVolume = 1.0);
    DataAssetRef GetData (const string& path);

    void GarbageCollect ();

    void ReloadAll ();
    void ReloadPath (const Asset* asset, DataItem* item);

    static void Register (AssetManager* manager) {
        instance = manager;
    }
    static AssetManager* Instance () { return instance; }

private:
    template <class T> AssetRef<T> GetAssetByKey (const string& path);

    const DataSource* m_dataSource;
    typedef map<string, Asset*> Assets;
    Assets m_assets;

    static AssetManager* instance;

    DISALLOW_COPY_AND_ASSIGN(AssetManager);
};

#define G_AssetManager AssetManager::Instance()

}

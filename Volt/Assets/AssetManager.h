#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/AssetRef.h"
#include "Volt/Assets/DataAsset.h"
#include "Volt/Assets/FontAsset.h"
#include "Volt/Assets/ShaderAsset.h"
#include "Volt/Assets/SoundAsset.h"
#include "Volt/Assets/TextureAsset.h"

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
    void ReloadShaders ();
    void ReloadPath (const Asset* asset, DataItem* item);

    string sourcePath() const;

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

#define G_AssetManager Volt::AssetManager::Instance()

}

#include "ShaderAsset.h"
#include "AssetManager.h"
#include "DataSource.h"
#include <json/json.h>
#include <fstream>

namespace Volt {

DataAsset::DataAsset ()
    : m_root(NULL) {
}

DataAsset::~DataAsset () {
    Unload();
}

bool DataAsset::Load (const DataItem& item) {
    m_path = item.path;

    Json::Reader reader;
    bool success = reader.parse(item.data, item.data + item.size, m_root);
    if (!success) {
        LOG(ERROR) << "Failed to parse " << item.path << ":\n" << reader.getFormatedErrorMessages();
    }
    return success;
}

void DataAsset::Reload () {
    Unload();
    DataItem item;
    m_manager->ReloadPath(this, &item);
    Load(item);
}

void DataAsset::Unload () {
    m_root.clear();
}

void DataAsset::Save (const Json::Value& root, string filename) {
    string path = G_AssetManager->sourcePath() + "/" + filename;
    ofstream out(path.c_str());
    if (!out.is_open()) {
        LOG(ERROR) << "Failed to open file " << path;
    }

    out << root;
    out.close();
}

}

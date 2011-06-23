#include "ShaderAsset.h"
#include "AssetManager.h"
#include "DataSource.h"
#include <json/json.h>

namespace Volt {

DataAsset::DataAsset ()
    : m_root(NULL) {
}

DataAsset::~DataAsset () {
    Unload();
}

bool DataAsset::Load (const DataItem& item) {
    m_path = item.path;

    m_root = new Json::Value;
    Json::Reader reader;
    return reader.parse(item.data, item.data + item.size, *m_root);
}

void DataAsset::Reload () {
    Unload();
    DataItem item;
    m_manager->ReloadPath(this, &item);
    Load(item);
}

void DataAsset::Unload () {
    delete m_root;
    m_root = NULL;
}

}

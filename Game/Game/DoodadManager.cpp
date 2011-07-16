#include "Game/Game/DoodadManager.h"
#include "Game/Entities/Game/Doodad.h"
#include "Volt/Game/Scene.h"

void DoodadBrush::Load (const Json::Value& node) {
    CHECK(node.isMember("id"));
    CHECK(node.isMember("texture"));
    CHECK(node.isMember("size"));
    CHECK(node.isMember("textureCoords"));
    CHECK(node["textureCoords"].size() == 4);

    name = node.get("name", "").asString();
    id = node["id"].asInt();
    texture = node["texture"].asString();
    for (int i = 0; i < 4; i++) {
        textureCoords[i].Load(node["textureCoords"][i]);
    }
    size.Load(node["size"]);
}

void DoodadBrush::Save (Json::Value& node) const {
    node["id"] = id;
    node["name"] = name;
    node["texture"] = texture;
    for (int i = 0; i < 4; i++) {
        Json::Value node;
        textureCoords[i].Save(node);
        node["textureCoords"].append(node);
    }
    size.Save(node["size"]);
}

float DoodadBrush::width () const {
    if (size.x != 0)
        return size.x;
    return size.y / Volt::G_AssetManager->GetTexture(texture)->height() *
                    Volt::G_AssetManager->GetTexture(texture)->width();
}

float DoodadBrush::height () const {
    if (size.y != 0)
        return size.y;
    return size.x / Volt::G_AssetManager->GetTexture(texture)->width() *
                    Volt::G_AssetManager->GetTexture(texture)->height();
}

DoodadManager* DoodadManager::instance = NULL;

DoodadManager::~DoodadManager () {
    UnloadBrushes();
}

void DoodadManager::LoadDoodadBrushes (Volt::DataAssetRef brushes) {
    m_brushDataRef = brushes;

    const Json::Value& node = brushes->data();
    for (uint i = 0; i < node.size(); i++) {
        DoodadBrush* brush = new DoodadBrush;
        brush->Load(node[i]);
        CHECK(m_brushes.find(brush->id) == m_brushes.end())
            << "Existing brush with id " << brush->id;
        m_brushes[brush->id] = brush;
    }
    LOG(INFO) << "Loaded " << m_brushes.size() << " brushes.";
}

void DoodadManager::UnloadBrushes () {
    FOR_(Brushes::iterator, i, m_brushes) {
        delete i->second;
    }
    m_brushes.clear();
}

void DoodadManager::ReloadBrushes () {
    UnloadBrushes();
    m_brushDataRef->Reload();
    LoadDoodadBrushes(m_brushDataRef);

    // Reload all doodad's brushes. TODO: Somewhat inefficient.
    vector<Volt::Entity*> entities;
    m_scene->GetEntities(&entities);
    for (uint i = 0; i < entities.size(); i++) {
        if (Doodad* d = dynamic_cast<Doodad*>(entities[i])) {
            d->SetBrush(GetDoodadBrush(d->brushId()));
        }
    }
}

DoodadBrush* DoodadManager::GetDoodadBrush (int id) {
    Brushes::iterator iter = m_brushes.find(id);
    CHECK(iter != m_brushes.end()) << "Could not find brush with id " << id;
    return iter->second;
}

void DoodadManager::GetDoodadBrushes (vector<DoodadBrush*>* brushes) {
    FOR_(Brushes::iterator, i, m_brushes) {
        brushes->push_back(i->second);
    }
}

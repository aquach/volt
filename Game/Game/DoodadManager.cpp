#include "Game/Game/DoodadManager.h"

void DoodadBrush::Load (const Json::Value& node) {
	CHECK(node.isMember("id"));
	CHECK(node.isMember("texture"));
	CHECK(node.isMember("size"));
	CHECK(node.isMember("textureCoords"));
	CHECK(node["textureCoords"].size() == 4);

	name = node.get("name", "").asString();
	id = node["id"].asInt();
    texture = Volt::G_AssetManager->GetTexture(node["texture"].asString());
	for (int i = 0; i < 4; i++) {
		textureCoords[i].Load(node["textureCoords"][i]);
	}
	size.Load(node["size"]);
}

void DoodadBrush::Save (Json::Value& node) const {
	node["id"] = id;
	node["name"] = name;
	node["texture"] = texture->path();
	for (int i = 0; i < 4; i++) {
		Json::Value node;
		textureCoords[i].Save(node);
		node["textureCoords"].append(node);
	}
	size.Save(node["size"]);
}

DoodadManager* DoodadManager::instance = NULL;

DoodadManager::~DoodadManager () {
    FOR_(Brushes::iterator, i, m_brushes) {
        delete i->second;
    }
    m_brushes.clear();
}

void DoodadManager::LoadDoodadBrushes (Volt::DataAssetRef brushes) {
	const Json::Value& node = brushes->data();
	for (uint i = 0; i < node.size(); i++) {
		DoodadBrush* brush = new DoodadBrush;
		brush->Load(node[i]);
		m_brushes[brush->id] = brush;
	}
	LOG(INFO) << "Loaded " << m_brushes.size() << " brushes.";
}

DoodadBrush* DoodadManager::GetDoodadBrush (int id) {
    Brushes::iterator iter = m_brushes.find(id);
	CHECK(iter != m_brushes.end());
	return iter->second;
}

void DoodadManager::GetDoodadBrushes (vector<DoodadBrush*>* brushes) {
    FOR_(Brushes::iterator, i, m_brushes) {
        brushes->push_back(i->second);
    }
}

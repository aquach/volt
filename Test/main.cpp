#include "Volt/Core/Core.h"
#include <GL/gl.h>
#include <SDL/SDL.h>
#include "Volt/Assets/AssetManager.h"
#include "Volt/Assets/DirectoryDataSource.h"
#include "Volt/Game/Entity.h"
#include "Volt/Game/Game.h"
#include "Volt/Game/Scene.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/GUI/Label.h"

using namespace Volt;

AssetRef<TextureAsset> test2 (AssetRef<TextureAsset> asset) {
	LOG(INFO) << "in test 2";
	LOG(INFO) << asset->referenceCount();
	return asset;
}

class TestEntity : public Entity {
public:
	TestEntity () {
		LOG(INFO) << "LOADING";
		font = G_AssetManager->GetFont("test.ttf", 80);
		font2 = G_AssetManager->GetFont("test.ttf", 40);
		LOG(INFO) << "LOADED";
		t = 0;
	}
	~TestEntity () { LOG(INFO) << "DESTRUCT"; }

	virtual void Render () {
		Graphics::SetBlend(Graphics::BLEND_ALPHA);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		Graphics::RenderText(font, "IM A COMPUTER", 100, 100);
		Graphics::RenderTextCentered(font2, "IM A COMPUTER SMALL",
									 Graphics::width() / 2,
									 Graphics::height() / 2);

		Graphics::SetBlend(Graphics::BLEND_NONE);
		glColor4f(1.0f, 1.0f, 0.5f, 1.0f);
		glLineWidth(5.0);
		Graphics::RenderLineRect(100, 100, 200, 200 + sin(t) * 50);
	}

	virtual void Update () {
		t += G_Game->dt();
	}

	float t;
	FontAssetRef font;
	FontAssetRef font2;
};

class TestScene : public Scene {
public:
	TestScene () {
		font = G_AssetManager->GetFont("test.ttf", 20);
	}

	virtual void OnBegin () {
		entity = new TestEntity;
		Add(entity);

		label = new Label(font, 50, 50);
		label->SetColor(Color::RGB(240, 100, 230));
		label->SetText("FPS");
		Add(label);
	}

	virtual void Update () {
		Scene::Update();
		char buffer[512];
		sprintf(buffer, "FPS: %.1f", G_Game->fps());
		label->SetText(buffer);
	}

	virtual void OnKeyEvent (SDL_KeyboardEvent e) {
		if (e.type == SDL_KEYUP) {
			if (e.keysym.sym == SDLK_UP) {
				entity->t = 0;
			}
		}
	}
	TestEntity* entity;
	Label* label;
	FontAssetRef font;
};

int main (int argc, char** argv) {
	google::InitGoogleLogging(argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);
	FLAGS_logtostderr = true;

	string exeDir = GetExecutableDirectory(argv[0]);
	LOG(INFO) << "Executable directory: " << exeDir;

	Game* game = new Game("Test Game",
						  new DirectoryDataSource(exeDir + "../TestData"));
	TestScene* scene = new TestScene;
	game->SetScene(scene);
	game->Run();

	delete game;

	//test(assetManager);
	/*
	AssetRef<FontAsset> asset = assetManager->RequestFont(
		"test.ttf", 20);
	float width = asset->GetTextWidth("AHH");
	LOG(INFO) << width;

	assetManager->GarbageCollect();
	*/

	return 0;
}

#include "Volt/Core/Core.h"
#include <GL/gl.h>
#include <SDL/SDL.h>
#include "Volt/Assets/AssetManager.h"
#include "Volt/Assets/Hash.h"
#include "Volt/Assets/DirectoryDataSource.h"
#include "Volt/Assets/PackDataSource.h"
#include "Volt/Game/Entity.h"
#include "Volt/Game/Game.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Game/Scene.h"
#include "Volt/Graphics/Filter.h"
#include "Volt/Graphics/GpuProgram.h"
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
		font = G_AssetManager->GetFont("Inconsolata.ttf", 80);
		font2 = G_AssetManager->GetFont("test.ttf", 40);
		LOG(INFO) << "LOADED";
		t = 0;
	}
	~TestEntity () { LOG(INFO) << "DESTRUCT"; }

	virtual void Render () {
		return;
		Graphics::SetBlend(Graphics::BLEND_ALPHA);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		Graphics::RenderText(font, "IM A COMPUTER", 100, 100);
		Graphics::RenderText(font2, "IM A COMPUTER SMALL",
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

class PhysicsEntity : public Entity {
public:
	PhysicsEntity (float x, float y) {
		m_transform.position.x = x;
		m_transform.position.y = y;
		m_transform.rotation = 30;

		b2BodyDef def;
		def.type = b2_dynamicBody;
		def.position.Set(m_transform.position.x, m_transform.position.y);
		def.angle = 30 * c_deg2rad;
		def.angularVelocity = (Random::Percent() - 0.5) * 5;
		Vector2 dir = Vector2(500, 500) - m_transform.position;
		dir.Normalize();
		def.linearVelocity.Set(dir.x * 50, dir.y * 50);

		def.userData = this;
		m_body = G_PhysicsManager->world()->CreateBody(&def);

		b2PolygonShape shape;
		shape.SetAsBox(50, 50);
		m_body->CreateFixture(&shape, 1);
	}
	~PhysicsEntity () { }

	virtual void Render () {
		Graphics::SetBlend(Graphics::BLEND_NONE);
		glLineWidth(2.0f);
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		glPushMatrix();
		Graphics::TransformMatrix(m_transform);
		Graphics::RenderLineRect(0, 0, 100, 100);
		glPopMatrix();
	}

	virtual void Update () {
		UpdatePhysics();
	}
};

class TestScene : public Scene {
public:
	TestScene () {
		font = G_AssetManager->GetFont("Inconsolata.ttf", 20);
		font2 = G_AssetManager->GetFont("Inconsolata.ttf", 50);
	}

	virtual void OnBegin () {
		sound = G_AssetManager->GetSound("bgm.ogg");
		sound2 = G_AssetManager->GetSound("thunder.ogg", SoundAsset::SOUND_MULTIPLE);
		sound->Play();

		entity = new TestEntity;
		Add(entity);

		Add(new PhysicsEntity(40, 40));
		Add(new PhysicsEntity(450, 40));
		Add(new PhysicsEntity(40, 450));

		label = new Label(font2, 400, 400);
		label->SetColor(Color::RGB(240, 100, 230));
		label->SetText("FPS");
		Add(label);

		Label* label2 = new Label(font2, 300, 300);
		label2->SetColor(Color::RGB(200, 0, 0));
		label2->SetText("ON TOP");
		Add(label2, -1);

		Label* label3 = new Label(font2, 400, 200);
		label3->SetAnchorX(Label::ANCHOR_LEFT);
		label3->SetAnchorY(Label::ANCHOR_CENTER);
		label3->SetColor(Color::RGB(200, 0, 0));
		label3->SetText("LABEL ANCHOR!!!");
		Add(label3, -1);

        /*
		GpuProgram* program = new GpuProgram;
		program->Attach(G_AssetManager->GetShader("standard.vert", ShaderAsset::SHADER_VERTEX));
		program->Attach(G_AssetManager->GetShader("test.frag", ShaderAsset::SHADER_FRAGMENT));
		Filter* filter = new Filter(program);
		//AddFilter(filter, -2);

		{
			GpuProgram* program = new GpuProgram;
			program->Attach(G_AssetManager->GetShader("standard.vert", ShaderAsset::SHADER_VERTEX));
			program->Attach(G_AssetManager->GetShader("test2.frag", ShaderAsset::SHADER_FRAGMENT));
			Filter* filter = new Filter(program);
			//AddFilter(filter, -1);
		}
        */
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
	SoundAssetRef sound;
	SoundAssetRef sound2;
	TestEntity* entity;
	Label* label;
	FontAssetRef font;
	FontAssetRef font2;
};

int main (int argc, char** argv) {
	string exeDir = GetExecutableDirectory(argv[0]);
	LOG(INFO) << "Executable directory: " << exeDir;


	//PackDataSource* pack = new PackDataSource(exeDir + "../out.leopak");

	/*
	DirectoryDataSource* s = new DirectoryDataSource(exeDir + "../TestData");
	s->WriteToPackFile("out.leopak");
	return 0;
	* */


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

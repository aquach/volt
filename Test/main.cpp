#include "Volt/Core/Core.h"
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <json/json.h>
#include "Volt/Assets/AssetManager.h"
#include "Volt/Assets/Hash.h"
#include "Volt/Assets/DirectoryDataSource.h"
#include "Volt/Assets/PackDataSource.h"
#include "Volt/Core/Tween.h"
#include "Volt/Game/Entity.h"
#include "Volt/Game/FSM.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Game/Game.h"
#include "Volt/Game/PhysicsManager.h"
#include "Volt/Game/Scene.h"
#include "Volt/Graphics/Filter.h"
#include "Volt/Graphics/GpuProgram.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Gui/Label.h"

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
		t += G_Time->dt();
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

		float angularVelocity = (Random::Percent() - 0.5) * 5;
		Vector2 dir = Vector2(10, 8) - m_transform.position;
		dir.Normalize();

		CreateBody(b2_dynamicBody);
		m_body->SetLinearVelocity(b2Vec2(dir.x, dir.y));
		m_body->SetAngularVelocity(angularVelocity);

		b2PolygonShape shape;
		shape.SetAsBox(0.5, 0.5);
		m_body->CreateFixture(&shape, 1);
	}
	~PhysicsEntity () { }

	virtual void Render () {
		Graphics::SetBlend(Graphics::BLEND_NONE);
		glLineWidth(2.0f);
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		glPushMatrix();
		Graphics::TransformMatrix(m_transform);
		Graphics::RenderLineRect(0, 0, 1, 1);
		glPopMatrix();
	}

	virtual void Update () {
		UpdatePhysics();
	}
};

class TweenEntity : public Entity {
public:
    TweenEntity(Tween<float> x, float y)
        : m_x(x), m_y(y) {
        m_color = Color::Random();
    }

    virtual void Update () {
        m_x.Update(G_Time->dt());
        m_transform.position.Set(m_x.value(), m_y);
    }

    virtual void Render () {
        Graphics::SetBlend(Graphics::BLEND_NONE);
        Graphics::SetColor(m_color);
		glPushMatrix();
		Graphics::TransformMatrix(m_transform);
		Graphics::RenderQuad(1, 1);
		glPopMatrix();
    }
private:
    Tween<float> m_x;
    float m_y;
    Color m_color;
};

class TestScene : public Scene {
public:
	TestScene () {
		font = G_AssetManager->GetFont("Inconsolata.ttf", 20);
		font2 = G_AssetManager->GetFont("Inconsolata.ttf", 50);

        DataAssetRef data = G_AssetManager->GetData("test.json");
        const Json::Value& root = data->data();
        const Json::Value v = root["derek"];
        LOG(INFO) << v;
        for (uint i = 0; i < v.size(); i++)
            LOG(INFO) << "Item: " << v[i].asInt();
        int blargh = root.get("blargh", 1).asInt();
        LOG(INFO) << "blargh: " << blargh;

		camera()->transform.scale.Set(50, 50);
		camera()->transform.position.Set(0, 0);
	}

	virtual void OnBegin () {
		sound = G_AssetManager->GetSound("bgm.ogg");
		sound2 = G_AssetManager->GetSound("thunder.ogg", SoundAsset::SOUND_MULTIPLE);
		sound->Play();

		entity = new TestEntity;
		Add(entity);

		Add(new PhysicsEntity(5, 0));
		Add(new PhysicsEntity(0, 5));
		Add(new PhysicsEntity(0, 0));

        //Add(new TweenEntity(Tween<float>::Linear(-7, 10, 4), -4));
        //Add(new TweenEntity(Tween<float>::QuadraticIn(-7, 10, 4), -2));
        //Add(new TweenEntity(Tween<float>::QuadraticOut(-7, 10, 4), 0));
        Add(new TweenEntity(Tween<float>::QuadraticInOut(-7, 10, 4), 2));
        //Add(new TweenEntity(Tween<float>::SinIn(-7, 10, 4), 4));
        //Add(new TweenEntity(Tween<float>::SinOut(-7, 10, 4), 6));
        //Add(new TweenEntity(Tween<float>::SinInOut(-7, 10, 4), -5));

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
		//char buffer[512];
		//sprintf(buffer, "FPS: %.1f", G_Time->fps());
		//label->SetText(buffer);
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

class TestState : public FSMState {
public:
    virtual void Update () {
		LOG(INFO) << m_stateName << " UPDATING";
		TransitionTo("BLARGH");
	}

    virtual void OnEnter () {
		LOG(INFO) << m_stateName << " ENTERING";
	}

    virtual void OnExit () {
		LOG(INFO) << m_stateName << " EXITING";
	}
};

int main (int argc, char** argv) {
	string exeDir = GetExecutableDirectory(argv[0]);
	LOG(INFO) << "Executable directory: " << exeDir;

	FSM* m = new FSM;
	m->AddState(new TestState, "F");
	m->AddState(new TestState, "BLARGH");
	m->TransitionTo("F");
	m->Update();
	m->Update();
	m->Update();
	m->Update();

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

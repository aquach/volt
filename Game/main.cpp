#include "Volt/Assets/DirectoryDataSource.h"
#include "Volt/Assets/PackDataSource.h"
#include "Volt/Game/Game.h"
#include "Game/Core/Core.h"
#include "Game/Scenes/GameScene.h"
#include "Game/Scenes/TitleScene.h"

int main (int argc, char** argv) {
	string exeDir = Volt::GetExecutableDirectory(argv[0]);
	LOG(INFO) << "Executable directory: " << exeDir;

	Volt::Game* game = new Volt::Game("Game",
		new Volt::DirectoryDataSource(exeDir + "../GameData"));
	GameScene* scene = new GameScene;
	game->SetScene(scene);
	game->Run();

	delete game;

	return 0;
}

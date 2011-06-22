#include "Volt/Assets/DirectoryDataSource.h"
#include "Volt/Assets/PackDataSource.h"
#include "Volt/Game/Game.h"
#include "Core/Core.h"
#include "Scenes/TitleScene.h"

int main (int argc, char** argv) {
	string exeDir = Volt::GetExecutableDirectory(argv[0]);
	LOG(INFO) << "Executable directory: " << exeDir;

	Volt::Game* game = new Volt::Game("Game",
		new Volt::DirectoryDataSource(exeDir + "../GameData"));
	TitleScene* scene = new TitleScene;
	game->SetScene(scene);
	game->Run();

	delete game;

	return 0;
}

#include "Volt/Core/Core.h"
#include <gflags/gflags.h>
#include "Volt/Game/Game.h"

int main (int argc, char** argv) {
	google::InitGoogleLogging(argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, true);
	FLAGS_logtostderr = true;

	Volt::Game* game = new Volt::Game("Test Game");
	game->Run();

	return 0;
}

#include "Game/Core/Core.h"
#include "Volt/Assets/AssetManager.h"

namespace Python {

void Initialize (int argc, char** argv);
void Terminate ();

void RunGameScriptFile (string scriptPath);
void RunGameScript (string script);
}

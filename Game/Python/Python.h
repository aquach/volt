#include "Volt/Assets/AssetManager.h"

namespace Python {

void Initialize (int argc, char** argv);
void Terminate ();

void ExecuteGameScriptFile (Volt::ScriptAssetRef script);
void ExecuteGameScript (string script);
}

#include "Game/Core/Core.h"
#include "Volt/Assets/AssetManager.h"

namespace Python {

void Initialize (int argc, char** argv);
void Terminate ();

void WaitForScripts ();
void RunGameScriptFile (const string& scriptPath);
//void RunGameScript (const string& script);

// Access to the GIL of Python. Lock to protect calls that might go into Python.
int Lock ();
void Unlock (int lock);
}

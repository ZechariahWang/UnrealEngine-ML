// Copyright Epic Games, Inc. All Rights Reserved.

//   1. Regenerate IDE/IntelliSense project files (run after adding/moving/renaming files or editing *.Build.cs)
//   & "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" `
//     -projectfiles -VSCode -game -rocket -progress `
//     -project="C:\Users\zecha\OneDrive\Documents\Unreal Projects\MyProject2\MyProject2.uproject"

//   2. Build the editor target (runs UHT → generates *.generated.h, compiles, links the module DLL)
//   & "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" `
//     MyProject2Editor Win64 Development -WaitMutex `
//     -Project="C:\Users\zecha\OneDrive\Documents\Unreal Projects\MyProject2\MyProject2.uproject"

// git lfs install

#include "MyProject2.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, MyProject2, "MyProject2" );

// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject2GameMode.h"
#include "MyProject2Character.h"
#include "MyProject2PlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMyProject2GameMode::AMyProject2GameMode()
{
	// Spawn the Blueprint character (which carries the mesh + input assets) by default.
	// Fall back to the raw C++ class if the Blueprint can't be found.
	DefaultPawnClass = AMyProject2Character::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP_MyProject2Character"));
	if (PlayerPawnBPClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AMyProject2PlayerController::StaticClass();
}

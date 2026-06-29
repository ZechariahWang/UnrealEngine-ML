// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject2GameMode.h"
#include "MyProject2Character.h"
#include "MyProject2PlayerController.h"

AMyProject2GameMode::AMyProject2GameMode()
{
	// Spawn our character and controller by default when the game starts.
	DefaultPawnClass = AMyProject2Character::StaticClass();
	PlayerControllerClass = AMyProject2PlayerController::StaticClass();
}

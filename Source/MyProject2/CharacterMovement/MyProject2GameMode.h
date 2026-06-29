// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProject2GameMode.generated.h"

/**
 * Default game mode. Wires up the default pawn (character) and player controller
 * so that hitting Play spawns a controllable character.
 */
UCLASS(minimalapi)
class AMyProject2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyProject2GameMode();
};

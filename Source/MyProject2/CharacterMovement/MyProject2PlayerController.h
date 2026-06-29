// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyProject2PlayerController.generated.h"

/**
 * Base player controller. A central place to hook up UI, pause handling,
 * and game-wide input later. Mouse cursor is hidden by default for gameplay.
 */
UCLASS()
class MYPROJECT2_API AMyProject2PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyProject2PlayerController();
};

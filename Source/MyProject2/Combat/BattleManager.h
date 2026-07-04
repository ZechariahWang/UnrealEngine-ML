#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleManager.generated.h"

class ABotCharacter;

/**
 * Drop one of these into the level and press Play: it spawns two facing
 * lines of bots (blue vs red) centered on its own location, watches for a
 * team wipe, announces the winner, and resets the round.
 */
UCLASS()
class MYPROJECT2_API ABattleManager : public AActor
{
	GENERATED_BODY()

public:
	ABattleManager();

	/** Bots per team. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	int32 TeamSize = 5;

	/** Distance between the two team lines. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	float TeamSeparation = 3000.0f;

	/** Spacing between bots within a line. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	float LineSpacing = 300.0f;

	/** Seconds to wait after a wipe before the next round. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	float RoundResetDelay = 4.0f;

	/** Bot class to spawn. Swap in a Blueprint subclass here later if wanted. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	TSubclassOf<ABotCharacter> BotClass;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY()
	TArray<TObjectPtr<ABotCharacter>> Bots;

	int32 RoundNumber = 0;
	float ResetTimer = -1.0f;

	void StartRound();
	void DestroyAllBots();
	int32 CountAlive(int32 TeamId) const;
	void SpawnTeamLine(int32 TeamId);
};

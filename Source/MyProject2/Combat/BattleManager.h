#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleManager.generated.h"

class ABotCharacter;

UCLASS()
class MYPROJECT2_API ABattleManager : public AActor
{
	GENERATED_BODY()

public:
	ABattleManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	int32 TeamSize = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	float TeamSeparation = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	float LineSpacing = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	float RoundResetDelay = 4.0f;
	
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

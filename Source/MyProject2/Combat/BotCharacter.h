#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BotCharacter.generated.h"

class UHealthComponent;
class UGunComponent;

UCLASS()
class MYPROJECT2_API ABotCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABotCharacter();

	// 0 = blue, 1 = red. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Team)
	int32 TeamId = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	TObjectPtr<UHealthComponent> Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	TObjectPtr<UGunComponent> Gun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float AttackRange = 1500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	bool bDead = false;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	bool IsAlive() const;

private:
	UPROPERTY()
	TObjectPtr<ABotCharacter> Target;

	float RetargetTimer = 0.0f;
	float StrafeTimer = 0.0f;
	float StrafeDirection = 1.0f;

	ABotCharacter* FindNearestEnemy() const;
	bool HasLineOfSightTo(const ABotCharacter* Other) const;
	void FaceTarget(float DeltaSeconds);
	void Die();
	void ApplyTeamColor();
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BotCharacter.generated.h"

class UHealthComponent;
class UGunComponent;

/**
 * A combat bot. Team 0 = blue, team 1 = red.
 *
 * Scripted behavior (all in Tick): pick the nearest living enemy, run toward
 * it until in range with line of sight, then strafe and shoot. Dies into a
 * ragdoll. This scripted brain is the placeholder the RL policy will replace.
 */
UCLASS()
class MYPROJECT2_API ABotCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABotCharacter();

	/** 0 = blue, 1 = red. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Team)
	int32 TeamId = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	TObjectPtr<UHealthComponent> Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	TObjectPtr<UGunComponent> Gun;

	/** Preferred shooting distance. Bot advances until closer than this. */
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

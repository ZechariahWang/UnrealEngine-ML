#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunComponent.generated.h"

/**
 * Hitscan gun. FireAt() traces a ray toward the target with random cone
 * spread and applies damage if it hits an enemy bot. No aiming skill needed
 * from the shooter — this keeps the eventual RL problem tractable (the agent
 * decides WHEN to shoot, not how to line up a crosshair).
 */
UCLASS(ClassGroup = (Combat), meta = (BlueprintSpawnableComponent))
class MYPROJECT2_API UGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGunComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	float Range = 6000.0f;

	/** Cone half-angle in degrees. Bigger = less accurate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	float SpreadDegrees = 3.0f;

	/** Seconds between shots. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	float FireInterval = 0.3f;

	/** Draw a debug line for each shot so fights are easy to watch. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	bool bDrawTracers = true;

	bool CanFire() const;

	/** Fire one shot toward the target actor. Returns true if it dealt damage. */
	bool FireAt(AActor* Target);

private:
	float LastFireTime = -1000.0f;
};

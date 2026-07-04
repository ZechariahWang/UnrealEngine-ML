#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

/**
 * Simple health pool. Owners poll IsDead() — no delegates.
 */
UCLASS(ClassGroup = (Combat), meta = (BlueprintSpawnableComponent))
class MYPROJECT2_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	float Health = 100.0f;

	void ApplyDamage(float Amount);

	bool IsDead() const;

	void ResetHealth();
};

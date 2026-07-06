#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunComponent.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	float SpreadDegrees = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	float FireInterval = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	bool bDrawTracers = true;

	bool CanFire() const;
	bool FireAt(AActor* Target);

private:
	float LastFireTime = -1000.0f;
};

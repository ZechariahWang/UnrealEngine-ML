#include "GunComponent.h"
#include "BotCharacter.h"
#include "HealthComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

UGunComponent::UGunComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

bool UGunComponent::CanFire() const {
	UWorld* World = GetWorld();
	if (World == nullptr) { return false; }

	float Now = World->GetTimeSeconds();
	return Now - LastFireTime >= FireInterval;
}

bool UGunComponent::FireAt(AActor* Target) {
	AActor* Shooter = GetOwner();
	UWorld* World = GetWorld();
	if (Shooter == nullptr || Target == nullptr || World == nullptr) { return false; }
	if (!CanFire()) { return false; }

	LastFireTime = World->GetTimeSeconds();

	// Aim from roughly eye height toward the target's center.
	FVector Start = Shooter->GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
	FVector AimPoint = Target->GetActorLocation();
	FVector Direction = (AimPoint - Start).GetSafeNormal();

	// Random cone spread.
	float SpreadRadians = FMath::DegreesToRadians(SpreadDegrees);
	Direction = FMath::VRandCone(Direction, SpreadRadians);

	FVector End = Start + Direction * Range;

	// Trace on the Pawn channel: walls block it and so do character capsules.
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Shooter);
	FHitResult Hit;
	bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, QueryParams);

	FVector TracerEnd = End;
	if (bHit) {
		TracerEnd = Hit.ImpactPoint;
	}
	if (bDrawTracers) {
		DrawDebugLine(World, Start, TracerEnd, FColor::Orange, false, 0.1f, 0, 1.0f);
	}

	if (!bHit) { return false; }

	// Only bots take damage, and never teammates.
	ABotCharacter* HitBot = Cast<ABotCharacter>(Hit.GetActor());
	if (HitBot == nullptr) { return false; }

	ABotCharacter* ShooterBot = Cast<ABotCharacter>(Shooter);
	if (ShooterBot != nullptr && ShooterBot->TeamId == HitBot->TeamId) { return false; }
	if (HitBot->Health == nullptr || HitBot->Health->IsDead()) { return false; }

	HitBot->Health->ApplyDamage(Damage);
	return true;
}

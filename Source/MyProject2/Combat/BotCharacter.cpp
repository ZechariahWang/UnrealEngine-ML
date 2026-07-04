#include "BotCharacter.h"
#include "HealthComponent.h"
#include "GunComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

ABotCharacter::ABotCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Bots rotate themselves toward their target in Tick, not via movement.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Gun = CreateDefaultSubobject<UGunComponent>(TEXT("Gun"));

	// Use the Manny mannequin so bots are visible without any editor setup.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MannyMesh(
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple"));
	if (MannyMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(MannyMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> UnarmedAnim(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"));
	if (UnarmedAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(UnarmedAnim.Class);
	}
}

void ABotCharacter::BeginPlay() {
	Super::BeginPlay();
	Health->ResetHealth();
	ApplyTeamColor();
}

void ABotCharacter::ApplyTeamColor() {
	FLinearColor TeamColor = FLinearColor::Blue;
	if (TeamId == 1) {
		TeamColor = FLinearColor::Red;
	}
	// The mannequin material exposes a "Tint" vector parameter. Setting it on
	// a material that lacks the parameter is harmless, so no need to check.
	int32 MaterialCount = GetMesh()->GetNumMaterials();
	for (int32 i = 0; i < MaterialCount; i++) {
		UMaterialInstanceDynamic* DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);
		if (DynamicMaterial != nullptr) {
			DynamicMaterial->SetVectorParameterValue(TEXT("Tint"), TeamColor);
		}
	}
}

bool ABotCharacter::IsAlive() const { return !bDead; }

void ABotCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (bDead) { return; }
	if (Health->IsDead()) {
		Die();
		return;
	}

	// Re-pick the nearest enemy a couple of times per second.
	RetargetTimer = RetargetTimer - DeltaSeconds;
	bool bTargetInvalid = (Target == nullptr) || !IsValid(Target) || !Target->IsAlive();
	if (RetargetTimer <= 0.0f || bTargetInvalid) {
		Target = FindNearestEnemy();
		RetargetTimer = 0.5f;
	}
	if (Target == nullptr) { return; } // Everyone on the other team is dead.

	FaceTarget(DeltaSeconds);

	float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	bool bCanSee = HasLineOfSightTo(Target);

	if (Distance > AttackRange || !bCanSee) {
		// Advance toward the target.
		FVector ToTarget = Target->GetActorLocation() - GetActorLocation();
		ToTarget.Z = 0.0f;
		AddMovementInput(ToTarget.GetSafeNormal(), 1.0f);
	}
	else {
		// In range with line of sight: strafe and shoot.
		StrafeTimer = StrafeTimer - DeltaSeconds;
		if (StrafeTimer <= 0.0f) {
			if (FMath::RandBool()) {
				StrafeDirection = 1.0f;
			}
			else {
				StrafeDirection = -1.0f;
			}
			StrafeTimer = FMath::RandRange(1.0f, 3.0f);
		}
		AddMovementInput(GetActorRightVector(), StrafeDirection * 0.6f);

		if (Gun->CanFire()) {
			Gun->FireAt(Target);
		}
	}
}

ABotCharacter* ABotCharacter::FindNearestEnemy() const {
	ABotCharacter* Nearest = nullptr;
	float NearestDistance = 0.0f;

	UWorld* World = GetWorld();
	if (World == nullptr) { return nullptr; }

	for (TActorIterator<ABotCharacter> It(World); It; ++It) {
		ABotCharacter* Other = *It;
		if (Other == this) { continue; }
		if (Other->TeamId == TeamId) { continue; }
		if (!Other->IsAlive()) { continue; }

		float Distance = FVector::Dist(GetActorLocation(), Other->GetActorLocation());
		if (Nearest == nullptr || Distance < NearestDistance) {
			Nearest = Other;
			NearestDistance = Distance;
		}
	}
	return Nearest;
}

bool ABotCharacter::HasLineOfSightTo(const ABotCharacter* Other) const {
	UWorld* World = GetWorld();
	if (World == nullptr || Other == nullptr) { return false; }

	FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
	FVector End = Other->GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);

	// If the trace hits anything before reaching the target, sight is blocked.
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Other);
	FHitResult Hit;
	bool bBlocked = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, QueryParams);
	return !bBlocked;
}

void ABotCharacter::FaceTarget(float DeltaSeconds) {
	if (Target == nullptr) { return; }

	FVector ToTarget = Target->GetActorLocation() - GetActorLocation();
	ToTarget.Z = 0.0f;
	if (ToTarget.IsNearlyZero()) { return; }

	FRotator Desired = ToTarget.Rotation();
	FRotator Current = GetActorRotation();
	FRotator Smoothed = FMath::RInterpTo(Current, Desired, DeltaSeconds, 10.0f);
	SetActorRotation(FRotator(0.0f, Smoothed.Yaw, 0.0f));
}

void ABotCharacter::Die() {
	bDead = true;
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	SetActorTickEnabled(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

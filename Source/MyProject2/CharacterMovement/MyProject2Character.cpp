// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject2Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogMyProject2Character);

AMyProject2Character::AMyProject2Character() {
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f); 	// Size the collision capsule.
	bUseControllerRotationPitch = false; // Let the controller rotate the character only via movement, not directly.
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* Movement = GetCharacterMovement(); // Configure character movement: face the direction of travel.
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	Movement->JumpZVelocity = 700.f;
	Movement->AirControl = 0.35f;
	Movement->MaxWalkSpeed = 3000.f;
	Movement->MinAnalogWalkSpeed = 20.f;
	Movement->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); // Create a camera boom (pulls in toward the character on collision).
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); // Create the follow camera on the end of the boom.
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AMyProject2Character::NotifyControllerChanged() {
	Super::NotifyControllerChanged();

	// Add the default input mapping context for the possessing local player.
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) { 
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMyProject2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProject2Character::Move);

		// Looking
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProject2Character::Look);
	}
	else {
		UE_LOG(LogMyProject2Character, Error,
			TEXT("'%s' failed to find an Enhanced Input component. Project must use the Enhanced Input system."),
			*GetNameSafe(this));
	}
}

void AMyProject2Character::Move(const FInputActionValue& Value) {

	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr) {
		// Find out which way is forward based on the controller's yaw.
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyProject2Character::Look(const FInputActionValue& Value) {
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	UE_LOG(LogMyProject2Character, Warning, TEXT("C++ Look() called: X=%f Y=%f"), LookAxisVector.X, LookAxisVector.Y);

	if (Controller != nullptr) {
		AddControllerYawInput(-LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

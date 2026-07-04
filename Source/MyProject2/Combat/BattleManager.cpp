#include "BattleManager.h"
#include "BotCharacter.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

ABattleManager::ABattleManager() {
	PrimaryActorTick.bCanEverTick = true;
	BotClass = ABotCharacter::StaticClass();
}

void ABattleManager::BeginPlay() {
	Super::BeginPlay();
	StartRound();
}

void ABattleManager::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	// Counting down to the next round?
	if (ResetTimer > 0.0f) {
		ResetTimer = ResetTimer - DeltaSeconds;
		if (ResetTimer <= 0.0f) {
			StartRound();
		}
		return;
	}

	// Otherwise watch for a team wipe.
	int32 BlueAlive = CountAlive(0);
	int32 RedAlive = CountAlive(1);
	if (BlueAlive > 0 && RedAlive > 0) { return; }

	FString Message;
	if (BlueAlive > 0) {
		Message = FString::Printf(TEXT("Round %d: BLUE wins!"), RoundNumber);
	}
	else if (RedAlive > 0) {
		Message = FString::Printf(TEXT("Round %d: RED wins!"), RoundNumber);
	}
	else {
		Message = FString::Printf(TEXT("Round %d: mutual destruction!"), RoundNumber);
	}
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, RoundResetDelay, FColor::Yellow, Message);
	}
	ResetTimer = RoundResetDelay;
}

void ABattleManager::StartRound() {
	DestroyAllBots();
	RoundNumber = RoundNumber + 1;
	SpawnTeamLine(0);
	SpawnTeamLine(1);
	if (GEngine) {
		FString Message = FString::Printf(TEXT("Round %d: fight!"), RoundNumber);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, Message);
	}
}

void ABattleManager::SpawnTeamLine(int32 TeamId) {
	UWorld* World = GetWorld();
	if (World == nullptr) { return; }

	// Blue lines up on -X, red on +X, both facing the middle.
	float SideSign = -1.0f;
	float FacingYaw = 0.0f;
	if (TeamId == 1) {
		SideSign = 1.0f;
		FacingYaw = 180.0f;
	}

	FVector Center = GetActorLocation();
	float LineHalfLength = LineSpacing * (TeamSize - 1) * 0.5f;

	for (int32 i = 0; i < TeamSize; i++) {
		FVector SpawnLocation = Center;
		SpawnLocation.X = SpawnLocation.X + SideSign * TeamSeparation * 0.5f;
		SpawnLocation.Y = SpawnLocation.Y - LineHalfLength + LineSpacing * i;
		SpawnLocation.Z = SpawnLocation.Z + 96.0f; // capsule half height

		// Deferred spawn: TeamId must be set BEFORE BeginPlay runs, because BeginPlay applies the team color.
		FRotator SpawnRotation(0.0f, FacingYaw, 0.0f);
		FTransform SpawnTransform(SpawnRotation, SpawnLocation);
		ABotCharacter* Bot = World->SpawnActorDeferred<ABotCharacter>(
			BotClass, SpawnTransform, nullptr, nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (Bot != nullptr) {
			Bot->TeamId = TeamId;
			Bot->FinishSpawning(SpawnTransform);
			Bots.Add(Bot);
		}
	}
}

void ABattleManager::DestroyAllBots() {
	for (int32 i = 0; i < Bots.Num(); i++) {
		ABotCharacter* Bot = Bots[i];
		if (Bot != nullptr && IsValid(Bot)) {
			Bot->Destroy();
		}
	}
	Bots.Empty();
}

int32 ABattleManager::CountAlive(int32 TeamId) const {
	int32 Count = 0;
	for (int32 i = 0; i < Bots.Num(); i++) {
		ABotCharacter* Bot = Bots[i];
		if (Bot != nullptr && IsValid(Bot) && Bot->TeamId == TeamId && Bot->IsAlive()) {
			Count = Count + 1;
		}
	}
	return Count;
}

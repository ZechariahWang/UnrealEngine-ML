#include "HealthComponent.h"

UHealthComponent::UHealthComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::ApplyDamage(float Amount) {
	if (Amount <= 0.0f) { return; }

	Health = Health - Amount;
	if (Health < 0.0f) {
		Health = 0.0f;
	}
}

bool UHealthComponent::IsDead() const { return Health <= 0.0f; }
void UHealthComponent::ResetHealth() { Health = MaxHealth; }

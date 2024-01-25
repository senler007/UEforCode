#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
}

float UAttributeComponent::GetHealthPercent()
{
	return CurrentHealth / MaxHealth;
}

float UAttributeComponent::GetStaminaPercent()
{
	return 0.0f;
}

bool UAttributeComponent::IsAlive()
{
	return CurrentHealth>0.f;
}

void UAttributeComponent::AddGold(int32 NumOfGold)
{
	Gold += NumOfGold;
}

void UAttributeComponent::AddSoul(int32 NumOfSoul)
{
	Soul += NumOfSoul;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


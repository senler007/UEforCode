#pragma once

UENUM(BlueprintType)
enum class ECharacterState :uint8
{
	ECS_Unequipped UMETA(DisplayName = "UnEquipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two_Handed Weapon"),
};

UENUM(BlueprintType)
enum class EActionState :uint8
{
	EAC_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAC_HitReaction UMETA(DisplayName = "HitReaction"),
	EAC_Attacking UMETA(DisplayName = "Attacking"),
	EAC_Equipping UMETA(DisplayName = "Equipping"),
	EAC_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum EDeathPose
{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),

	EDP_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EEnemyState :uint8
{
	EES_NoState UMETA(DisPlayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisPlayName = "Engaged")
};
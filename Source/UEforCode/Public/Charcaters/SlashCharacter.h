// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterType.h"
#include "Interface/PickupInterface.h"
#include "SlashCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;
class AItems;
class UAnimMontage;
class USlashOverlay;

UCLASS()
class UEFORCODE_API ASlashCharacter : public ABaseCharacter,public IPickupInterface
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Jump() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(AItems* Item) override;
	virtual void AddSouls(class ASoul* Soul) override;
	virtual void AddGold(class ATreasure* Gold) override;
protected:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BluePrintReadWrite)
	EActionState ActionState = EActionState::EAC_Unoccupied;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	virtual void BeginPlay() override;
	
	/*Input*/
	void MoveForward(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void MoveRight(float Value);
	void EKeyPressed();
	void VKeyPressed();
	void FKeyPressed();
	virtual void Attack() override;


	/**Combat*/

	void EquipWeapon(ASword* Sword);
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	void PlayEquipMontage(FName SectionName);
	bool CanDisarm();
	bool CanArm(); 
	void DisArm();
	void Arm();
	virtual void Die_Implementation() override;

	UFUNCTION(BlueprintCallable) 
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquip();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

private:

	void InitializeSlashOverlay();
	void SetHUDHealth();

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	AItems* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY()
	USlashOverlay* SlashOverlay;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABird> BirdClass;

public:
	FORCEINLINE ECharacterState GetSlashCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};

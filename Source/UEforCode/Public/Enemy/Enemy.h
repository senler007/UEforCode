
#pragma once

#include "CoreMinimal.h"
#include "Charcaters/BaseCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class UEFORCODE_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/** <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/** </AActor> */

	/** <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */

protected:
	/** <AActor> */
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <ABaseCharacter> */
	virtual void Die_Implementation() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;
	/** </ABaseCharacter> */

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:
	/** AI Behavior */
	void CheckPatrolTarget();
	void CheckCombatTarget();

	bool InTargetRange(AActor* Target, double Radius);
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();

	void HideHealthBar();
	void ShowHealthBar();

	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	void ClearPatrolTimer();
	AActor* ChoosePatrolTarget();
	void MoveToTarget(AActor* Target);
	void SpawnDefaultWeapon();
	
	UFUNCTION()
	void PawnSeen123(APawn* SeenPawn);  //Callback for OnPawnSeen in UPawnSensingComponent

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 3.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 8.f;

	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* PawnSensing;

	/**Combat*/
	FTimerHandle AttackTimer;

	void StartAttackTimer();
	void ClearAttackTimer();

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AcceptanceRadius = 150.f;

	/** Others */

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;

	UPROPERTY(EditAnywhere)
	class UHealthBarComponent* HealthBarWidge;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASword> SwordClass;

	UPROPERTY()
	class AAIController* EnemyController;
};


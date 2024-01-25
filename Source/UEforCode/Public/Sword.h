// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "Sword.generated.h"

class UBoxComponent;
class USceneComponent;

UCLASS()
class UEFORCODE_API ASword : public AItems
{
	GENERATED_BODY()

public:
	ASword();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void DeactivateEmbers();
	void DisableSphereCollision();
	void AttachMeshtoSocket(USceneComponent* InParent, const FName& InSocketName);

	TArray<AActor*> IgnoreActors;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;

	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Sword Property")
	FVector BoxTraceExtent = FVector(5.f); //¼´ÎªFVector(5.f, 5.f, 5.f)

	UPROPERTY(EditAnywhere, Category = "Sword Property")
	bool bShowBoxDebug = false;

	UPROPERTY(VisibleAnywhere,Category = "Sword Property")
	UBoxComponent* SwordBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Sword Property")
	float Damage = 20.f;
	
public:
	FORCEINLINE UBoxComponent* GetSwordBox() const { return SwordBox; }
};

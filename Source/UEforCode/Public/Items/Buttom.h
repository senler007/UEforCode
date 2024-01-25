// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Buttom.generated.h"

DECLARE_DELEGATE_OneParam(PressButtonDelegate, bool);


UCLASS()
class UEFORCODE_API AButtom : public AActor
{
	GENERATED_BODY()
	
public:	
	AButtom();
	PressButtonDelegate delegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

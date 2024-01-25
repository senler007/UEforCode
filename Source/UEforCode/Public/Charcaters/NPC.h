// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Charcaters/BaseCharacter.h"
#include "Interface/Dialog.h"
#include "NPC.generated.h"

/**
 * 
 */
UCLASS()
class UEFORCODE_API ANPC : public ABaseCharacter,public IDialog
{
	GENERATED_BODY()
	
public:
	

protected:
	UPROPERTY(EditAnywhere)
	TArray<FText> DialogText;

	UPROPERTY(EditAnywhere)
	class USphereComponent* Tragger;

	UFUNCTION(BlueprintCallable)
	virtual void Talk();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Dialog.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDialog : public UInterface
{
	GENERATED_BODY()
};

class UEFORCODE_API IDialog
{
	GENERATED_BODY()

public:
	virtual void Talk() = 0;//=0 means that it cant be defined in this class


};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Charcaters/NPC.h"
#include "Blueprint/UserWidget.h"

void ANPC::Talk()
{
	int32 DialogNum = DialogText.Num();
	if (DialogNum > 0) {
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Talk"));
}

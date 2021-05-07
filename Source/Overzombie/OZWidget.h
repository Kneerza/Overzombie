// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OZWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERZOMBIE_API UOZWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnInitializeEndGame"))
		void OnInitializeEndGame(bool isWin);
};

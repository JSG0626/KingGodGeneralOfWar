// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerAimUI.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UPlayerAimUI : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UImage* IMG_CenterUI ;
public:

	void ChangeColor(bool Red);
};

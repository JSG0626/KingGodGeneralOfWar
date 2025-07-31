// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPUI.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API UEnemyHPUI : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HPPercent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)

	float	StunGuagePercent;
	void SetHPBar(float Percent);
	void SetStunGuageBar(float Percent);
};

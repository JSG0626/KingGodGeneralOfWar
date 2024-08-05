// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SG_GodOfWar_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API ASG_GodOfWar_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASG_GodOfWar_GameModeBase();

public:
	virtual void BeginPlay() override;

	//�ӽ÷� UI �־��
	UPROPERTY(EditAnywhere)
	TSubclassOf <UUserWidget> BDThorFadeOutFactory; //���̵� �ƿ� UI Ŭ���� ȣ��

	UPROPERTY()
	class UBDthorFadeOut* BDThorFadeOutUI;

	UFUNCTION()
	void ThorFadeOut();
	
};

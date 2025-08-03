// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../CommonHeaders/PlayerCommon.h"
#include "PlayerStateMappingDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UPlayerStateMappingDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Player States")
	//TArray<FStateClassPair> StateClassMapData; // 매핑 데이터를 저장할 배열
	TMap < EPlayerState, TSubclassOf<class UKratosState>> StateClassMapData;
};

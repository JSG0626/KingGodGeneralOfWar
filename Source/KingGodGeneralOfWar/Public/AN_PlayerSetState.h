// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include <Kratos.h>
#include "AN_PlayerSetState.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UAN_PlayerSetState : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	EPlayerState NewState;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

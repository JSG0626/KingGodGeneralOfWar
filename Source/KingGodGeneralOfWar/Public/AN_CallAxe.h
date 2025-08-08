// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_CallAxe.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UAN_CallAxe : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	float MaxReturnDuration = 0.5f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	float MinReturnDuration = 0.5f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	float RadiusScale = 0.15f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	bool bImmediateReturn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	bool bRightHand = true;
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_TimeDilation.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UAN_TimeDilation : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override ;

	UPROPERTY(EditAnywhere)
	float Duration = 0.015f;

	UPROPERTY(EditAnywhere)
	float SlowScale = 0.06f ;
};

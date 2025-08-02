// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ThrowAxe.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UAN_ThrowAxe : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	bool bIsHeavy ;
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override ;

};

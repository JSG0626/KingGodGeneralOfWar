// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SetAxeRotation.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UANS_SetAxeRotation : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	FRotator OriginAxeRotation;
	class AAxe* Axe;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator NewAxeRotation;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	//virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
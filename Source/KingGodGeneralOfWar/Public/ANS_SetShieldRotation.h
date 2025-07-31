// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SetShieldRotation.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API UANS_SetShieldRotation : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	FRotator OriginRotation;
	class ASG_Shield* Shield;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator NewRotation;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

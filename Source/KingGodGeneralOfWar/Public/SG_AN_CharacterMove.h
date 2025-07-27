// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "SG_AN_CharacterMove.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API USG_AN_CharacterMove : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NewWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction;

	float OriginWalkSpeed;
	class UCharacterMovementComponent* CMC;
	FVector WorldDirection;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

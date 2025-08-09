// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ThrowAxeInAttack.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UANS_ThrowAxeInAttack : public UAnimNotifyState
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<class AKratos> Kratos ;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Throw")
	bool bThrowInAttack;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Throw")
	FRotator LocalRotationOffset;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Return")
	float MaxReturnDuration = 0.5f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Return")
	float MinReturnDuration = 0.5f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Return")
	float RadiusScale = 0.15f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Return")
	bool bCatchRightHand = true;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Return")
	bool bOrbital = false ;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Return")
	int OrbitalCount = 1;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Return")
	bool bClockWise = true;
	public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	//virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

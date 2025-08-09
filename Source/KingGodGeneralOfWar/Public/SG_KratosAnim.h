// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kratos.h"
#include "SG_KratosAnim.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API USG_KratosAnim : public UAnimInstance
{
	GENERATED_BODY()

private:

public:
	USG_KratosAnim();
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TMap<EPlayerMontage, TObjectPtr<UAnimMontage>> MontageMap;

	void PlayMontage(const EPlayerMontage State, bool bJumpSection = false, const FString SectionName = TEXT("Default"));

	void JumpToAttackMontageSection(int32 NewSection);

	void ActiveLookAt(bool Active);

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerState();

	UPROPERTY()
	class AKratos* Me;
	UPROPERTY(BlueprintReadOnly)
	EPlayerState PlayerState;
	UPROPERTY(BlueprintReadOnly)
	float Direction;
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	UPROPERTY(BlueprintReadWrite)
	bool bMeshSpaceRotationBlend = false;

	UPROPERTY(BlueprintReadWrite)
	FVector LookAtTarget;

	UPROPERTY(BlueprintReadWrite)
	bool bRecallAxe;

private:
	UFUNCTION()
	void AnimNotify_AbleNextAttack();

	UFUNCTION()
	void AnimNotify_EndDodge();

	UFUNCTION()
	void AnimNotify_FaceCameraForward();

	FName GetAttackMontageSection(int32 Section);

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	bool bActiveLookAt;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kratos.h"
#include "SG_KratosAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextWeakAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnWeakAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnWeakAttackEndCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMovableChekcDelegate);
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
	TMap<EPlayerMontage, UAnimMontage*> MontageMap ;

	void PlayMontage(const EPlayerMontage State, bool bJumpSection = false, const FString SectionName = TEXT("Default"));
	
	void JumpToAttackMontageSection(int32 NewSection);

	void ActiveLookAt(bool Active);

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerState();

	UPROPERTY(BlueprintReadOnly)
	EPlayerState PlayerState;
	UPROPERTY(BlueprintReadOnly)
	float Direction;
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	UPROPERTY(BlueprintReadWrite)
	bool bMeshSpaceRotationBlend = false ;
	
	UPROPERTY(BlueprintReadWrite)
	FVector LookAtTarget ;

	UPROPERTY()
	class AKratos* Me;
private:
	UFUNCTION()
	void AnimNotify_AbleNextAttack() ;

	UFUNCTION()
	void AnimNotify_FieldSpawn();

	UFUNCTION()
	void AnimNotify_RuneAttackGroundShake();

	UFUNCTION()
	void AnimNotify_RuneAttackCameraSet();

	UFUNCTION()
	void AnimNotify_GetUPCameraSet();

	UFUNCTION()
	void AnimNotify_RuneReady();

	UFUNCTION()
	void AnimNotify_RuneAttackEnd();

	UFUNCTION()
	void AnimNotify_SpawnEarthCrack();

	UFUNCTION()
	void AnimNotify_InitAttackType();

	UFUNCTION()
	void AnimNotify_AttackProgress();

	UFUNCTION()
	void AnimNotify_LaunchCharacterInStrongAttack();

	UFUNCTION()
	void AnimNotify_EndWithFail();

	UFUNCTION()
	void AnimNotify_JumpCharacterInStrongAttack();
	
	UFUNCTION()
	void AnimNotify_EndDodge() ;

	FName GetAttackMontageSection(int32 Section);
	FName GetRollMontageSection(int32 Section);
	FName GetGuardMontageSection(int32 Section);

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	bool bActiveLookAt;
};

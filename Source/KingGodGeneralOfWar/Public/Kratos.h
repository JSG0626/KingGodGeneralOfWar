// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include <KratosStates/KratosState.h>
#include "KingGodGeneralOfWar.h"
#include "CommonHeaders/PlayerCommon.h"
#include "Kratos.generated.h"


const float PlayerMaxSpeed = 900.0f; // 플레이어 최대 속도. (달리기)
const FRotator DefaultCameraAngle = FRotator(0);
const float DefaultTargetTargetArmLength = 147;
const float DefaultTargetFOV = 90;

UCLASS()
class KINGGODGENERALOFWAR_API AKratos : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKratos();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float UnarmedAttackPower = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float AxeAttackPower = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float BladeAttackPower = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	float SpearAttackPower = 10.0f;

	bool bLerpPlayerRotation = false;
	int LeprPlayerRotationScale = 1;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnMyActionMove(const FInputActionValue& Value);

	UFUNCTION()
	void OnMyActionLook(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionDodge(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionRunOn(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionRunOff(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionGuardOn(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionGuardOff(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionLockOn(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionIdle(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionLAttack(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionHAttack(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionHChargeAttack(const FInputActionValue& value) ;

	UFUNCTION()
	void OnMyActionAimOn(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionAimOff(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionAbility(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionRuneBase(const FInputActionValue& value);

	void OnMyGuardDisappear();

	void OnMyLaunchCharacterInStrongAttack();
	void OnMyJumpCharacterInStrongAttack();

	void IncreaseTargetTargetArmLength(float value);
	void IncreaseTargetCameraOffset(FVector value);

	// Axe Throwing
	void HideHoldingAxe();
	void ThrowAxe(const bool bIsHeavy);
	void CallAxe();
	void CallAxe(const float MaxReturnDuration, const float MinReturnDuration, bool bImmediateReturn, const float RadiusScale);
	void CatchFlyingAxe();
	void ThrowAxeInAttack(const FRotator LocalRotationOffset);

	// Damage Function
	// if Kratos get damage, return true; else return false;
	UFUNCTION(BlueprintCallable)
	bool Damage(AActor* Attacker, int DamageValue, EHitType HitType, bool IsMelee);
	void LaunchKratos(float LaunchScale = 2000);

	// Rune
	void OnMyRuneAttackCameraSet();
	void OnMySpawnEarthCrack();
	void OnMyAttackProgress();

	void OnMyEndWithFail();
	void OnMyGetUPCameraSet();
	void OnMyActionDebugKey();


	void CameraShakeOnAttack(EAttackDirectionType attackDir = EAttackDirectionType::UP, float scale = 1.0f);
	FString GetPlayerStateString() const;
	void SetGlobalTimeDilation(float Duration, float SlowScale);
	void SetAnimationSpeedSlow(float Duration, float SlowScale);

	void PlayerMove();
	void SetWeapon();
	void SetShield();

	void SetLockOnTarget();
	void LockOnTargetTick(float DeltaTime);

	FString GetHitSectionName(EHitType hitType);
	FString GetDodgeDirection();

public:
	// Player Input
	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Dodge;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Run;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Guard;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_LockOn;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_LightAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_HeavyAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_HeavyChargeAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_Aim;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_WithdrawAxe;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_RuneBase;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	class UInputAction* IA_DebugKey;

	// UClass Pointer

	UPROPERTY()
	class ABaseEnemy* LockTarget;

	UPROPERTY()
	class USG_KratosAnim* Anim;

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* WithdrawPositionComp;

	// Kratos Weapon
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AAxe> AxeFactory;

	UPROPERTY()
	class AAxe* Axe;

	UPROPERTY()
	class UCharacterMovementComponent* CMC;


	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ASG_Shield> ShieldFactory;

	UPROPERTY()
	class ASG_Shield* Shield;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AFlyingAxe> FlyingAxeFactory;

	UPROPERTY()
	class AFlyingAxe* FlyingAxe;

	// VFX & Camera Shake
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> DownAttackShakeFactory;
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> UpAttackShakeFactory;
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> LeftAttackShakeFactory;
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> RightAttackShakeFactory;
	UPROPERTY(EditAnywhere, Category = "Camera Shake")

	TSubclassOf<class UCameraShakeBase> CatchAxeShakeFactory;

	UPROPERTY()
	TArray< TSubclassOf<class UCameraShakeBase>> AttackShakeFactoryArr;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* ParryVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* ShockWaveVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* GuardBlockVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* GuardCrashVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* EarthCrackVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX Actor")
	TSubclassOf<class AActor> ParryingLightFactory;

	UPROPERTY(EditDefaultsOnly, Category = "VFX Actor")
	TSubclassOf<class AActor> GuardBlockLightFactory;

	// ------------------------------------------ SFX -----------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* AvoidSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* AxeThrowSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* AxeWithdrawSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* HitSound1;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* HitSound2;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* HitSound3;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RollSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RuneBaseSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RuneAttack1Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RuneAttack2Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RuneAttack3Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* RuneAttack4Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* StrongAttack1Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* StrongAttack2Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* StrongAttack3Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* StrongAttack4Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* WeakAttack1Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* WeakAttack2Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* WeakAttack3Sound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* GrabAxeSound;

	
	UPROPERTY()
	TArray<class USoundBase*> WeakAttackSoundArr;

	UPROPERTY()
	TArray<class USoundBase*> StrongAttackSoundArr;

	UPROPERTY()
	TArray<class USoundBase*> RuneAttackSoundArr;

	UPROPERTY()
	class ACSWGameMode* GameMode;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> AimWidgetClass;
	UPROPERTY()
	class UPlayerAimUI* AimWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerState State = EPlayerState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera");
	float MinPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera");
	float MaxPitch;

	// 룬공격 막타 줌아웃
	bool bZoomOut;

public:
	bool bTraceEnemy = false;
	bool bFaceEnemy = true;
	FVector DefaultCameraOffset;
	FRotator TargetCameraRotation;
	FRotator TargetActorRotation;
	float TargetShieldScale = 0;
	FVector Direction;
	FVector PrevDirection;

	float TargetFOV = DefaultTargetFOV;
	float TargetTargetArmLength = DefaultTargetTargetArmLength;
	FVector TargetCameraOffset;
	FRotator TargetCameraAngle = FRotator(0);

	bool bLockOn;
	bool bEvade;
	bool bAxeGone;
	bool bIsAxeWithdrawing;

	UPROPERTY(EditDefaultsOnly)
	class UPlayerStateMappingDataAsset* PlayerStateDataAsset;

	void InitializeStates();
	void SetKratosState(const EPlayerState& NewState, const FGenericStateParams& params = FGenericStateParams());

	//UPROPERTY(EditDefaultsOnly, Category = "States")
	//TArray<FStateClassPair> StateClassSetUp;
	UPROPERTY()
	TMap<EPlayerState, TObjectPtr<UKratosState>> KratosStatesMap;
	UPROPERTY()
	TObjectPtr<UKratosState> CurrentState;

	bool CanComboAttack;
	bool bIsRunning;

	const float DEFAULT_FOV = 90;

	void ActiveLerpPlayerRotation(FRotator TargetRotation, int Scale);
	void InactiveLerpPlayerRotation();

	float SetHP(const float NewHP);

	float GetAttackPower(EPlayerWeaponType WeaponType) const;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float DistPoint = 2.0f ;

	void ActiveAxeTrail(bool ActiveState);

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float FindTargetDistPoint = 0.5f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float FindTargetRadius = 200;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float FindTargetEndLocation = 400;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float AttackRange = 100.0f;
	float AttackRangeSquared;
	UPROPERTY()
	TObjectPtr<class AActor> CurTargetEnemy;

	TObjectPtr<class AActor> FindTargetEnemy() const;

	void InitMaxWalkSpeed();
	void SetMaxWalkSpeed(const float NewWalkSpeed);
	void PlayMontage(const EPlayerMontage MontageType, bool bJumpSection = false, const FString SectionName = TEXT("Default"));
	void OnHitHChargeAttack();

	void SwapAxeHands(bool Right);

	inline EPlayerState GetState() const { return State; }
};

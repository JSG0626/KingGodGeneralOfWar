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

	// ================== Bind Action ====================
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
	void OnMyActionHChargeAttack(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionAimOn(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionAimOff(const FInputActionValue& value);

	UFUNCTION()
	void OnMyActionAbility(const FInputActionValue& value);

	void OnMyActionDebugKey();
	// ================== Bind Action ====================

	// ================== Input Mapping ====================

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Input Action")
	TObjectPtr<class  UInputMappingContext> IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction> IA_Dodge;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction> IA_Run;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction> IA_Guard;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction> IA_LockOn;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction> IA_LightAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction> IA_HeavyAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction> IA_HeavyChargeAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction>IA_Aim;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction> IA_WithdrawAxe;

	UPROPERTY(EditDefaultsOnly, Category = "Input Action")
	TObjectPtr<class UInputAction> IA_DebugKey;
	// ================== Input Mapping ====================
	
	// ================== State ====================
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	TObjectPtr<class UPlayerStateMappingDataAsset> PlayerStateDataAsset;
	UPROPERTY()
	TMap<EPlayerState, TObjectPtr<UKratosState>> KratosStatesMap;
	UPROPERTY()
	TObjectPtr<UKratosState> CurrentState;
	// ================== State ====================
	
	// ================== Components ====================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<class  USpringArmComponent> SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<class  UCameraComponent> CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<class  UArrowComponent> RightHandTransformComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<class  UArrowComponent> LeftHandTransformComp;

	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> CMC;
	// ================== Components ====================

	// ================== UClass Pointer ====================
	UPROPERTY()
	TObjectPtr<class  ABaseEnemy> LockTarget;

	UPROPERTY()
	TObjectPtr<class  USG_KratosAnim> Anim;

	UPROPERTY()
	TObjectPtr<class AAxe> Axe;

	UPROPERTY()
	TObjectPtr<class ASG_Shield> Shield;

	UPROPERTY()
	TObjectPtr<class AFlyingAxe> FlyingAxe;

	UPROPERTY()
	TObjectPtr<class UPlayerAimUI> AimWidget;
	// ================== UClass Pointer ====================

	// ================== VFX & Camera Shake ====================
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> DownAttackShakeFactory;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> UpAttackShakeFactory;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> LeftAttackShakeFactory;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> RightAttackShakeFactory;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = "Camera Shake")

	TSubclassOf<class UCameraShakeBase> CatchAxeShakeFactory;

	UPROPERTY()
	TArray< TSubclassOf<class UCameraShakeBase>> AttackShakeFactoryArr;
	// ================== VFX & Camera Shake ====================

	// ================== Factory ===============================
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Factory/Weapon")
	TSubclassOf<class AAxe> AxeFactory;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Factory/Weapon")
	TSubclassOf<class ASG_Shield> ShieldFactory;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Factory/Weapon")
	TSubclassOf<class AFlyingAxe> FlyingAxeFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Factory/UI")
	TSubclassOf<class UUserWidget> AimWidgetClass;
	// ================== Factory ===============================

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "TraceEnemy")
	float FindTargetDistPoint = 0.5f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "TraceEnemy")
	float FindTargetRadius = 200;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "TraceEnemy")
	float FindTargetEndLocation = 400;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "TraceEnemy")
	float AttackRange = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Status/Attack")
	float UnarmedAttackPower = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Status/Attack")
	float AxeAttackPower = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Status/Attack")
	float BladeAttackPower = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Status/Attack")
	float SpearAttackPower = 10.0f;

	bool bLerpPlayerRotation = false;
	int LeprPlayerRotationScale = 1;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), BlueprintReadWrite, Category = "Status")
	float MaxHP = 100;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	EPlayerState State = EPlayerState::Idle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), BlueprintReadWrite, Category = "Camera");
	float MinPitch;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), BlueprintReadWrite, Category = "Camera");
	float MaxPitch;

	float CurHP;
	float AttackRangeSquared;


	void PlayerMove();
	void InitAxe();
	void InitShield();
	void LockOnTargetTick(float DeltaTime);
	void SetTargetToLockOn();

protected:

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	// Damage Function
	// if Kratos get damage, return true; else return false;
	UFUNCTION(BlueprintCallable)
	bool Damage(AActor* Attacker, int DamageValue, EHitType HitType, bool IsMelee);

	// Axe Throwing
	void HideHoldingAxe();
	void ThrowAxe(const bool bIsHeavy);
	void CallAxe();
	void CallAxe(const float MaxReturnDuration, const float MinReturnDuration, bool bImmediateReturn, const float RadiusScale, const bool bRightHand = true);
	void CatchFlyingAxe();
	void ThrowAxeInAttack(const FRotator LocalRotationOffset, const bool bOrbital = false, const float OrbitalDuration = 0.0f, const float OrbitalCount = 1, const bool bClockWise = true);

	// VFX
	void CameraShakeOnAttack(EAttackDirectionType attackDir = EAttackDirectionType::UP, float scale = 1.0f);
	void SetGlobalTimeDilation(float Duration, float SlowScale);
	void SetAnimationSpeedSlow(float Duration, float SlowScale);


	UPROPERTY()
	class ACSWGameMode* GameMode;

public:
	bool bTraceEnemy = false;
	bool bFaceEnemy = true;
	bool bLockOn;
	bool bEvade;
	bool bAxeGone;
	bool bIsAxeWithdrawing;

	float TargetShieldScale = 0;
	float TargetFOV = DefaultTargetFOV;
	float TargetTargetArmLength = DefaultTargetTargetArmLength;

	FVector Direction;
	FVector PrevDirection;
	FVector DefaultCameraOffset;
	FVector TargetCameraOffset;
	FRotator TargetCameraRotation;
	FRotator TargetActorRotation;
	FRotator TargetCameraAngle = FRotator(0);

	void InitializeStates();
	void SetKratosState(const EPlayerState& NewState, const FGenericStateParams& params = FGenericStateParams());


	UPROPERTY()
	TObjectPtr<class AActor> CurTargetEnemy;

	bool CanComboAttack;
	bool bIsRunning;

	const float DEFAULT_FOV = 90;

	void ActiveLerpPlayerRotation(FRotator TargetRotation, int Scale);
	void InactiveLerpPlayerRotation();

	float SetHP(const float NewHP);

	float GetAttackPower(EPlayerWeaponType WeaponType) const;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float DistPoint = 2.0f;





	void InitMaxWalkSpeed();
	void SetMaxWalkSpeed(const float NewWalkSpeed);
	void PlayMontage(const EPlayerMontage MontageType, bool bJumpSection = false, const FString SectionName = TEXT("Default"));
	void PlayMontage(const EPlayerMontage MontageType, const int SectionNumber);
	void Montage_JumpToSection(const int SectionNumber);
	void Montage_JumpToSection(const FString SectionName);
	void OnHitHChargeAttack();

	void SwapAxeHands(bool Right);

	inline EPlayerState GetState() const { return State; }

	float GetDamage(const float Damage);
	FVector GetCameraLocation() const;
	FRotator GetCameraRotation(bool bRemovePitch = true) const;
	float GetAttackRangeSquared() const;
	TObjectPtr<class  UArrowComponent> GetHandTransformComp(const bool bRightHand) const;
	TObjectPtr<class AAxe> GetAxe() const;
	TObjectPtr<class ASG_Shield> GetShield() const;

	void SetMeshSpaceRotationBlend(const bool bActive) const;
	void ChangeAimUIColor(bool bHit) const;
	void ActiveAxeTrail(bool ActiveState);

	TObjectPtr<class AActor> FindTargetEnemy() const;
};

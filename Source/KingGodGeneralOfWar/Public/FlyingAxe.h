// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponInterface.h"
#include "FlyingAxe.generated.h"


UENUM(BlueprintType)
enum class EAxeState : uint8
{
	Idle,     // 대기
	Flying,   // 앞으로 날아가는 중
	Stuck,    // 어딘가에 박혀있는 상태
	Bounce,   // 충돌 후 튕겨나가는 상태
	Vibration, // 회수 전 진동 상태
	Returning, // 플레이어에게 돌아오는 중
};

UCLASS()
class AFlyingAxe : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	// ...
public:
	// Sets default values for this actor's properties
	AFlyingAxe();

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* SubMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UPointLightComponent* LightComp;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* BladeLocationComp;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* LerpInitRotationComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* BloodVFXFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundBase> ReturnSpinningSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<class USoundCue> BaseHitSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<class USoundBase> CatchAxeSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<class USoundBase> StuckSound;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<class USoundBase> BounceSound;

	void BackToPlayer(bool bImmediateReturn);
	void BackToPlayer(const float MaxReturnDuration, const float _MinReturnDurationbool, bool bImmediateReturn, const float _RadiusScale = 0.25f);

	virtual void ActiveHitCollision(bool Active) override;
	virtual TObjectPtr<class USoundCue> GetBaseHitSound() const override;
	void Init(class AKratos* _Me, bool _bIsHeavy);
	void Init(class AKratos* _Me, const FRotator LocalRotationOffset);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// 상태별 로직을 처리할 함수
	void TickState_Flying(float DeltaTime);
	void TickState_Bounce(float DeltaTime);
	void TickState_Vibration(float DeltaTime);
	void TickState_Returning(float DeltaTime);

	void SetState(const EAxeState NewState, const FHitResult& HitResult = FHitResult{});

	// 매 프레임 라인트레이싱을 통해 충돌을 체크
	bool CollisionCheck(FHitResult& HitResult);

	// 상태 전환 및 처리 헬퍼 함수
	void HandleCatch();

	// 도끼를 강하게 던졌는지를 확인
	bool bIsHeavy;

	// 현재 도끼의 상태
	EAxeState CurrentState = EAxeState::Flying;

	// 크레토스를 변수로 갖고있음
	UPROPERTY()
	class AKratos* Kratos;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Decal")
	class UMaterialInterface* ImpactDecalMaterial ;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Decal")
	FVector DecalSize = FVector(50.0f, 50.0f, 50.0f); // X,Y,Z (깊이, 폭, 높이)
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Decal")
	float DecalLifeSpan = 5.0f; // 5초 후 사라짐

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Attack")
	float HeavyScale = 1.5f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Attack")
	float DefaultScale = 1.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Attack")
	float StunDamage = 3;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement")
	float GravityTime = 0.7f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement")
	float CatchDistanceThreshold = 100.0f;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Flying)")
	float HeavyThrowingMoveSpeed = 3500.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Flying)")
	float DefaultThrowingMoveSpeed = 3000.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Flying)")
	float ThrowingRotationSpeed = 50.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Flying)")
	float RotationSpeed = -1;


	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Bounce)")
	float BounceSpeed = 2000.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Bounce)")
	float BounceGravityScale = 2.0f ;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Bounce)")
	float BounceUpScale = 2.0f ;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Bounce)")
	float BouncingRotationInitSpeed = 10.0f ;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Bounce)")
	float BouncingRotationIncrementAlpha = 10.0f;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Vibration)")
	float VibrationTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Vibration)")
	float VibrationRandomRange = 5.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Vibration)")
	float VibrationRollIncrement = 10.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Vibration)")
	float VibrationYawDelta = 0.01;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Return)")
	float ReturnRotationSpeed = -1 ;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Return)")
	float MinReturnSpeed = 2000;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Return)")
	float MaxReturnDuration = 2.5f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Return)")
	float MinReturnDuration = 0.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Return)")
	float RadiusScale = 0.25;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Return)")
	float StartInterpRotationDist = 1000 ;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Return)")
	float InterpRotationDuration = 0.5f ;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Return)")
	float LerpInitReturnRotationTime = 0.2f ;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement(Return)")
	float LerpInitReturnRotationSpeed = 40.0f ;

	// 상태 관련 변수
	FVector PrevLocation;
	FVector TargetLocation;
	FVector CurrentVelocity;

	FVector AxeMeshOffset;
	float PathCurveRadius;
	float ReturnDuration;
	FVector ReturnStartLocation;
	float StartInterpRotationDistSquared;
	FVector VibrationMoveDirection;
	FVector PrevBladeLocation;
	float VibrationRoll = 0;
	int VibrationTickCount = 0;
	FQuat TargetLeprInitQuat;
	bool bPass;

	struct ST_StateElapsedTime
	{
		float Flying = 0.0f;
		float Return = 0.0f;
		float Slerp = 0.0f;
		float Vibration = 0.0f;
		float Bounce = 0.0f;
		float SlerpHandRotation = 0.0f;
	} ;
	ST_StateElapsedTime StateElapsedTime;
	UPROPERTY()
	TObjectPtr<class AActor> StuckActor;	

	UPROPERTY()
	TArray<TObjectPtr<ABaseEnemy>> DamagedActors;

	void OnEnterStuck(const FHitResult& HitResult);
	void OnEnterBounce(const FHitResult& HitResult);
	void OnEnterVibration();
	void OnEnterReturning();
};

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
	Rising,   // 회수 시 위로 떠오르는 중
	Returning // 플레이어에게 돌아오는 중
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* BloodVFXFactory;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* BaseHitSoundCue;

	void BackToPlayer();

	virtual void ActiveHitCollision(bool Active) override;
	virtual TObjectPtr<class USoundCue> GetBaseHitSound() const override;
	void Init(class AKratos* _Me, bool _bIsHeavy);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// 상태별 로직을 처리할 함수
	void TickState_Flying(float DeltaTime);
	void TickState_Rising(float DeltaTime);
	void TickState_Returning(float DeltaTime);

	// 매 프레임 라인트레이싱을 통해 충돌을 체크
	bool CollisionCheck();

	// 상태 전환 및 처리 헬퍼 함수
	void HandleCatch();

	// 도끼를 강하게 던졌는지를 확인
	bool bIsHeavy;

	// 현재 도끼의 상태
	EAxeState CurrentState = EAxeState::Flying;

	// 크레토스를 변수로 갖고있음
	UPROPERTY()
	class AKratos* Kratos;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Attack")
	float HeavyScale = 1.5f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Attack")
	float DefaultScale = 1.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Attack")
	float StunDamage = 3;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement")
	float HeavyThrowingMoveSpeed = 3500.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement")
	float DefaultThrowingMoveSpeed = 3000.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement")
	float ThrowingRotationSpeed = 50.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement")
	float CatchDistanceThreshold = 100.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Movement")
	float RotationSpeed = -1;

	// 상태 관련 변수
	float CurrentFlySpeed;
	FVector PrevLocation;
	FVector TargetLocation;
	FVector CurrentVelocity;
	float FlyingTime = 0.0f;

	float ReturningInterpAlpha = 0.0f;
	float ReturningAlphaDelta = 0.02f;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Custom")
	float ReturningSpeed = 1000;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Custom")
	float ReturningSpeedDelta = 1;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Custom")
	float RisingSpeed = 1800;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Custom")
	float RisingSpeedDelta = 25;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Custom")
	float GravityTime = 1.0f;
};

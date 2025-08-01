// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponInterface.h"
#include "FlyingAxe.generated.h"


UENUM(BlueprintType)
enum class EAxeState : uint8
{
	Idle,     // ���
	Flying,   // ������ ���ư��� ��
	Stuck,    // ��򰡿� �����ִ� ����
	Rising,   // ȸ�� �� ���� �������� ��
	Returning // �÷��̾�� ���ƿ��� ��
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
	class UArrowComponent* DirectionArrowComp;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* HitArrowComp;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* WithdrawTargetPosition1;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* WithdrawTargetPosition2;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* WithdrawTargetPosition3;

	UPROPERTY()
	TArray<class UArrowComponent*> WithdrawTargetPositionArr;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* WithdrawRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* BloodVFXFactory;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* BaseHitSoundCue;

	UFUNCTION()
	void FlyingAxeOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void BackToPlayer();

	UPROPERTY(EditDefaultsOnly)
	float FLYING_AXE_DAMAGE = 1;
	UPROPERTY(EditDefaultsOnly)
	float FLYING_AXE_STUN_DAMAGE = 3;

	virtual void ActiveHitCollision(bool Active) override;

	virtual class USoundCue* GetBaseHitSound() const override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	// ���� ������ ����
	EAxeState CurrentState = EAxeState::Flying;

	UPROPERTY()
	class AKratos* Me;

	// ���º� ������ ó���� �Լ�
	void TickState_Flying(float DeltaTime);
	void TickState_Rising(float DeltaTime);
	void TickState_Returning(float DeltaTime);

	// ���� ��ȯ �� ó�� ���� �Լ�
	void HandleCatch();

	// �����丵�� ��� ��
	static const float FLYING_MOVE_SPEED;
	static const float FLYING_ROTATION_SPEED;
	static const float RISING_LERP_SPEED;
	static const float RETURNING_INTERP_SPEED;
	static const float CATCH_DISTANCE_THRESHOLD;

	// ���� ���� ����
	float RotationSpeed = 1;
	float CurrentFlySpeed;
	FVector PrevLocation;
	FVector TargetLocation;
	FVector CurrentVelocity;

	float ReturningInterpAlpha = 0.0f;
	float ReturningAlphaDelta = 0.02f;

};

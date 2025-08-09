// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KratosState.h"
#include "KS_Dodge.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_Dodge :public UKratosState
{
	GENERATED_BODY()

public:
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleMove() const override { return true; }
	virtual bool CanHandleDodge() const override { return bDashing; }
	virtual bool CanHandleHit() const override;
	virtual bool CanHandleAim() const override { return true; }
	virtual bool CanHandleLAttack() const override { return true; }
	virtual bool CanHandleHAttack() const override { return true; }

	virtual void HandleMove(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleLAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleHAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleAim(const FGenericStateParams& params = FGenericStateParams()) override;
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	TObjectPtr<class USoundBase> RollSound;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float RollVelocityThreshhold ;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float DashDodgeAttackTimeThreshold = 0.5f;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float RollDodgeAttackTimeThreshold = .05f;

	bool bDashing = false;
	FString GetDodgeDirection(const FVector& Direction) const;

	bool bAimInputOn;
	EAttackType AttackInput = EAttackType::None;

	float TickTime;
	FVector2D InputDirection;
};

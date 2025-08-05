// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KratosStates/KratosState.h"
#include "KS_LDashAttack.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_LDashAttack : public UKratosState
{
	GENERATED_BODY()

public:
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleDodge() const override { return true; }
	virtual bool CanHandleLAttack() const override { return true; }
	virtual bool CanHandleHAttack() const override { return true; }

	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleLAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleHAttack(const FGenericStateParams& params = FGenericStateParams()) override;

private:
	FVector InitVelocity;
	float CurrentSpeedScale = 1.0f;

	bool bLAttackInputOn = false;
	bool bHAttackInputOn = false;
};

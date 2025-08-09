// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KratosStates/KratosState.h"
#include "KS_CombatIdleLAttack.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_CombatIdleLAttack : public UKratosState
{
	GENERATED_BODY()
public:
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleDodge() const override { return true; }
	virtual bool CanHandleLAttack() const override { return true; }
	virtual bool CanHandleHAttack() const override { return true; }
	virtual bool CanHandleAbility() const override { return true; }
	virtual bool CanHandleGuard() const override { return true; }

	virtual void HandleLAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleHAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleAbility(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleGuard(const FGenericStateParams& params = FGenericStateParams()) override;
private:

	bool bGuardInputOn;
	EAttackType AttckInputOn;
};

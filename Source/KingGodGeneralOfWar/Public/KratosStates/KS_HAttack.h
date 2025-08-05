// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KratosState.h"
#include "KS_HAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class KINGGODGENERALOFWAR_API UKS_HAttack : public UKratosState
{
	GENERATED_BODY()

	public:
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleHAttack() const override { return true; }
	virtual bool CanHandleDodge() const override { return true; }
	virtual bool CanHandleGuard() const override { return true; }
	virtual bool CanHandleAim() const override { return true; }

	virtual void HandleHAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleGuard(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleAim(const FGenericStateParams& params = FGenericStateParams()) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float DashAttackSpeedThreshold = 800;

	int CurrentAttackNum = 1;
	bool bGuardInputOn;
	bool bAimInputOn;

};

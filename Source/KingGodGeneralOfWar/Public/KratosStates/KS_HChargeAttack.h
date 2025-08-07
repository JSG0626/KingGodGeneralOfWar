// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KratosStates/KratosState.h"
#include "KS_HChargeAttack.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_HChargeAttack : public UKratosState
{
	GENERATED_BODY()
	
public:	
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleDodge() const override { return true; }
	virtual bool CanHandleHChargeAttackHit() const override { return true; }

	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleHChargeAttackHit(const FGenericStateParams& params = FGenericStateParams()) override;

};

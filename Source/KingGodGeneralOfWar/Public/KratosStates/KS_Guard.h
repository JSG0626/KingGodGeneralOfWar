// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KratosState.h"
#include "KS_Guard.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_Guard : public UKratosState
{
	GENERATED_BODY()

public:
	virtual void SetUp(class AKratos* kratos) override;
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleIdle() const override { return true; }
	virtual bool CanHandleMove() const override { return true; }
	virtual bool CanHandleDodge() const override { return true; }
	virtual bool CanHandleWAttack() const override { return true; }
	virtual bool CanHandleHit() const override { return true; };

	virtual void HandleIdle(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleMove(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleWAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleHit(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleParry(const FGenericStateParams& params = FGenericStateParams()) override;

private:
	class ASG_Shield* Shield;
	const float WalkMoveScale = 0.15f;
	float TargetMoveScale = WalkMoveScale;
	float CurrentMoveScale;

	const float PARRIABLE_TIME = 0.2f;
	float TickTime = 0.0f;
};

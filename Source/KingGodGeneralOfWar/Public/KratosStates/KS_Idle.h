// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KratosState.h"
#include "KS_Idle.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class KINGGODGENERALOFWAR_API UKS_Idle : public UObject, public IKratosState
{
	GENERATED_BODY()

public:
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleMove() const override { return true; }
	virtual bool CanHandleDodge() const override { return true; }
	virtual bool CanHandleWAttack() const override { return true; }
	virtual bool CanHandleGuard() const override { return true; }
	virtual bool CanHandleAim() const override { return true; }

	virtual void HandleMove(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleWAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleGuard(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleAim(const FGenericStateParams& params = FGenericStateParams()) override;

private:
	FVector Direction;
	const float TargetMoveScale = 0.0f;
	float CurrentMoveScale = 0.56f;
	float TickTime = 0.0f;
};

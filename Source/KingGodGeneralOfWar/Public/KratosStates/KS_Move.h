// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KratosState.h"
#include "KS_Move.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class KINGGODGENERALOFWAR_API UKS_Move : public UObject, public IKratosState
{
	GENERATED_BODY()

public:
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleIdle() const override { return true; }
	virtual bool CanHandleMove() const override { return true; }
	virtual bool CanHandleDodge() const override { return true; }
	virtual bool CanHandleWAttack() const override { return true; }
	virtual bool CanHandleSAttack() const override { return true; }
	virtual bool CanHandleGuard() const override { return true; }
	virtual bool CanHandleAim() const override { return true; }

	virtual void HandleIdle(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleMove(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleWAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleSAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleGuard(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleAim(const FGenericStateParams& params = FGenericStateParams()) override;

private:
	const float RunMoveScale = 1.0f;
	const float WalkMoveScale = 0.56f;
	float TargetMoveScale = WalkMoveScale;
	float CurrentMoveScale;
	FRotator TargetActorRotation;
};

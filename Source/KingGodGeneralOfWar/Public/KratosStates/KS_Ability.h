// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KratosState.h"
#include "KS_Ability.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_Ability : public UKratosState
{
	GENERATED_BODY()
	
public:
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleIdle() const override { return true; }
	virtual bool CanHandleMove() const override { return true; }
	virtual bool CanHandleDodge() const override { return true; }
	virtual bool CanHandleGrabAxe() const override { return true; }

	virtual void HandleIdle(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleMove(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleGrabAxe(const FGenericStateParams& params = FGenericStateParams()) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess))
	float WalkMoveScale = 0.3f;

	FRotator TargetActorRotation;
};

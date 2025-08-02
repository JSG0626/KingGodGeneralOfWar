// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KratosState.h"
#include "KS_Aim.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_Aim : public UKratosState
{
	GENERATED_BODY()

public:
	virtual void SetUp(class AKratos* Kratos) override;
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleIdle() const override { return true; }
	virtual bool CanHandleMove() const override { return true; }
	virtual bool CanHandleDodge() const override { return true; }
	virtual bool CanHandleWAttack() const override { return true; }

	virtual void HandleIdle(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleMove(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleWAttack(const FGenericStateParams& params = FGenericStateParams()) override;

private:
	const float THROW_AXE_COOLDOWN = 1.0f;
	const float AIM_UI_MAX_DIST = 1000000.0f;
	const float AIM_FOV = 60.0f;
	const float WalkMoveScale = 0.3f;
	float TargetMoveScale = WalkMoveScale;
	float CurrentMoveScale;
	float CoolDown = THROW_AXE_COOLDOWN;

	class UPlayerAimUI* AimWidget;

};

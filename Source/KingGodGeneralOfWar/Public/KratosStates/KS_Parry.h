// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KratosState.h"
#include "KS_Parry.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_Parry : public UKratosState
{
	GENERATED_BODY()

public:
	virtual void SetUp(class AKratos* Kratos) override;
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleLAttack() const override { return true; }
	virtual bool CanHandleDodge() const override { return true; }
	virtual bool CanHandleHit() const override { return false; }

	virtual void HandleWAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
private:
	class ASG_Shield* Shield;

	const float PARRY_DAMAGE = 10.0f;
	const float PARRY_STUN_DAMAGE = 10.0f;
};

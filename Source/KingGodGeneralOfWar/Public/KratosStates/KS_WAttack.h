// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KratosState.h"
#include "KS_WAttack.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_WAttack : public UKratosState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int TestInteger ;

	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleWAttack() const override { return true; }
	virtual bool CanHandleDodge() const override { return true; }
	virtual bool CanHandleGuard() const override { return true; }

	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleWAttack(const FGenericStateParams& params = FGenericStateParams()) override;
	virtual void HandleGuard(const FGenericStateParams& params = FGenericStateParams()) override;

private:
	int CurrentAttackNum = 1;
	bool bGuardInputOn;

};

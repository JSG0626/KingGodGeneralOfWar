// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KratosState.h"
#include "KS_Hit.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_Hit : public UObject, public IKratosState
{
	GENERATED_BODY()

	public:
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleHit() const override { return false; };
private:
};

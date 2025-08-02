// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KratosState.h"
#include "KS_Dodge.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_Dodge :public UKratosState
{
	GENERATED_BODY()

public:
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleDodge() const override { return bDashing; }
	virtual bool CanHandleHit() const override;

	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) override;
private:

	bool bDashing = false;
	FString GetDodgeDirection(const FVector& Direction) const;
};

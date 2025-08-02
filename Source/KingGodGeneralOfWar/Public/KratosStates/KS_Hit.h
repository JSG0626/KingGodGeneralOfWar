// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KratosState.h"
#include "KS_Hit.generated.h"

/**
 *
 */
UCLASS()
class KINGGODGENERALOFWAR_API UKS_Hit : public UKratosState
{
	GENERATED_BODY()

public:
	virtual void SetUp(class AKratos* Kratos) override;
	virtual void EnterState(const FGenericStateParams& params) override;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) override;
	virtual void ExitState(const FGenericStateParams& params) override;

	virtual bool CanHandleHit() const override { return false; };
private:
	UPROPERTY()
	TMap<EHitType, float> NuckBackScale;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_LDashAttack.h"
#include "Kratos.h"

void UKS_LDashAttack::EnterState(const FGenericStateParams& params)
{
	Super::EnterState(params);
	StateLog(TEXT("LDashAttack Enter"));
	Me->CanComboAttack = false;
	CurrentSpeedScale = 1;
	InputOn = false;
	bLAttackInputOn = false;
	bHAttackInputOn = false;
	InitVelocity = Me->GetVelocity();

	Me->CurTargetEnemy = Me->FindTargetEnemy();

	if (true == Me->bAxeGone)
	{
		StateLog(TEXT("맨손 대쉬 공격")); 
		Me->SetKratosState(EPlayerState::Idle);
	}
	else
	{
		Me->PlayMontage(EPlayerMontage::LDashAttack);
	}
}

void UKS_LDashAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("LDashAttack Tick"), true);

	if (false == Me->bTraceEnemy)
	{
		FRotator rotate = Me->GetControlRotation();
		rotate.Pitch = 0.0f;
		Me->SetActorRotation(rotate);
	}
	if (Me->CanComboAttack)
	{
		if (bLAttackInputOn)
		{
			Me->CanComboAttack = false;
			Me->SetKratosState(EPlayerState::LAttack);
		}
		else if (bHAttackInputOn)
		{
			Me->CanComboAttack = false;
			Me->SetKratosState(EPlayerState::HAttack);
		}
	}

	Me->AddMovementInput(Me->GetActorForwardVector(), CurrentSpeedScale);

	CurrentSpeedScale = FMath::Max(0, CurrentSpeedScale  - DeltaTime);
}

void UKS_LDashAttack::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("LDashAttack Exit"));
}

void UKS_LDashAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_LDashAttack::HandleLAttack(const FGenericStateParams& params)
{
	bLAttackInputOn = true;
}

void UKS_LDashAttack::HandleHAttack(const FGenericStateParams& params)
{
	bHAttackInputOn = true;
}

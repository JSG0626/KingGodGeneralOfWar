// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_LDashAttack.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"

void UKS_LDashAttack::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("LDashAttack Enter"));
	Anim->PlayMontage(EPlayerMontage::LDashAttack);
	InitVelocity = Me->GetVelocity();

	CurrentSpeedScale = 1;
}

void UKS_LDashAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("LDashAttack Tick"), true);
	FRotator rotate = Me->GetControlRotation();
	rotate.Pitch = 0.0f;
	Me->SetActorRotation(rotate);

	if (Me->CanComboAttack && InputOn)
	{
		Me->CanComboAttack = false;
		InputOn = false;
		Me->SetKratosState(EPlayerState::LAttack);
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
	InputOn = true;
}

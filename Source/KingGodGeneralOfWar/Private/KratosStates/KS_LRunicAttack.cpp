// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_LRunicAttack.h"
#include "Kratos.h"

void UKS_LRunicAttack::EnterState(const FGenericStateParams& params)
{
	Super::EnterState(params);
	StateLog(TEXT("LRunicAttack Enter"));

	FRotator rotate = Me->GetController()->GetControlRotation();
	rotate.Pitch = 0;
	Me->SetActorRotation(rotate);
	InputOn = false;
	Me->CanComboAttack = false;

	if (Me->bAxeGone)
	{
		UE_LOG(LogTemp, Display, TEXT("회수 공격"));
	}
	else
	{
		Me->PlayMontage(EPlayerMontage::LRunicAttack);
	}
}

void UKS_LRunicAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	if (Me->CanComboAttack && InputOn)
	{
		Me->Montage_JumpToSection(TEXT("Second"));
		InputOn = false;
		Me->CanComboAttack = false;
	}
}

void UKS_LRunicAttack::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("LRunicAttack Exit"));
}

void UKS_LRunicAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_LRunicAttack::HandleHit(const FGenericStateParams& params)
{
	const float CurHP = Me->GetDamage(params.AttackParams.Damage);
	if (CurHP == 0)
	{
		HandleDie(params);
	}
}

void UKS_LRunicAttack::HandleLAttack(const FGenericStateParams& params)
{
	InputOn = true;
}

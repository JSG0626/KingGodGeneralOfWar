// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_HDashAttack.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"

void UKS_HDashAttack::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("HDashAttack Enter"));

	Me->CanComboAttack = false;
	CurrentSpeedScale = 1;
	bFaceCameraForward = true;
	InputOn = false;
	bLAttackInputOn = false;
	bHAttackInputOn = false;
	Me->CurTargetEnemy = Me->FindTargetEnemy();

	InitVelocity = Me->GetVelocity();
	if (true == Me->bAxeGone)
	{
		StateLog(TEXT("맨손 대쉬 공격"));
		Me->SetKratosState(EPlayerState::Idle);
	}
	else
	{
		Anim->PlayMontage(EPlayerMontage::HDashAttack);
	}
}

void UKS_HDashAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("HDashAttack Tick"), true);

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

	//Me->AddMovementInput(Me->GetActorForwardVector(), CurrentSpeedScale);

	//CurrentSpeedScale = FMath::Max(0, CurrentSpeedScale - DeltaTime * 0.5f);
}

void UKS_HDashAttack::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("HDashAttack Exit"));
}

void UKS_HDashAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_HDashAttack::HandleLAttack(const FGenericStateParams& params)
{
	bLAttackInputOn = true;
}

void UKS_HDashAttack::HandleHAttack(const FGenericStateParams& params)
{
	bHAttackInputOn = true;
}

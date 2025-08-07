// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_HAttack.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"
#include <Kismet/KismetMathLibrary.h>
#include "WeaponInterface.h"
#include "Axe.h"
#include "SG_Shield.h"

void UKS_HAttack::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("HAttack Enter"));
	if (Me->GetVelocity().Size() >= DashAttackSpeedThreshold)
	{
		// 대쉬 공격
		Me->SetKratosState(EPlayerState::HDashAttack);
		return;
	}
	if (Me->bAxeGone)
	{
		// 맨손 공격
		Me->SetKratosState(EPlayerState::Idle);
		return;
	}
	bGuardInputOn = false;
	bAimInputOn = false;
	CurrentAttackNum = 1;
	Anim->PlayMontage(EPlayerMontage::HAttack);
	CurrentAttackNum++;
	Me->CanComboAttack = false;
}

void UKS_HAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("HAttack Tick"), true);
	if (false == Me->bTraceEnemy)
	{
		FRotator rotate = Me->GetControlRotation();
		rotate.Pitch = 0.0f;
		Me->SetActorRotation(rotate);
	}

	if (Me->CanComboAttack && InputOn)
	{
		if (bAimInputOn)
		{
			FGenericStateParams Params;
			Params.Bool = true;
			Params.Integer = 1;
			Me->SetKratosState(EPlayerState::Aim, Params);
		}
		else
		{
			Me->CanComboAttack = false;
			InputOn = false;
			Anim->JumpToAttackMontageSection(CurrentAttackNum++);
		}
	}
}

void UKS_HAttack::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("HAttack Exit"));
	Me->CanComboAttack = false;
	InputOn = false;
	Anim->Montage_Stop(.1f);
	TScriptInterface<IWeaponInterface>(Me->Axe)->ActiveHitCollision(false);
	TScriptInterface<IWeaponInterface>(Me->Shield)->ActiveHitCollision(false);
}


void UKS_HAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_HAttack::HandleGuard(const FGenericStateParams& params)
{
	bGuardInputOn = true;
	if (InputOn)
	{
		//Me->SetKratosState(EPlayerState::RuneSAttack);
	}
}

void UKS_HAttack::HandleAim(const FGenericStateParams& params)
{
	bAimInputOn = true;
}

void UKS_HAttack::HandleHChargeAttack(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::HChargeAttack);
}

void UKS_HAttack::HandleHAttack(const FGenericStateParams& params)
{
	InputOn = true;
}

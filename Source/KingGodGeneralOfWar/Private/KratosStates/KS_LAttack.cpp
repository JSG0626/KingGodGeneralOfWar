// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_LAttack.h"
#include "Kratos.h"
#include "Axe.h"
#include "SG_Shield.h"
#include "SG_KratosAnim.h"
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <WeaponInterface.h>
#include <Kismet/KismetMathLibrary.h>

void UKS_LAttack::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("LAttack Enter"));

	if (Me->GetVelocity().Size() >= DashAttackSpeedThreshold)
	{
		// 대쉬 공격
		Me->SetKratosState(EPlayerState::LDashAttack);
		return;
	}
	if (Me->bAxeGone)
	{
		// 맨손 공격
		Me->SetKratosState(EPlayerState::Idle);
		return;
	}
	
	CurrentAttackNum = 1;
	bGuardInputOn = false;
	bAimInputOn = false;
	Me->CanComboAttack = false;

	Anim->PlayMontage(EPlayerMontage::LAttack);
	CurrentAttackNum++;

	Me->CurTargetEnemy = Me->FindTargetEnemy();
	if (nullptr != Me->CurTargetEnemy)
	{
		UE_LOG(LogTemp, Display, TEXT("타겟 설정 완료!"));
	}
}

void UKS_LAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("LAttack Tick"), true);

	
	if (nullptr == Me->CurTargetEnemy && false == Me->bTraceEnemy)
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
			Me->CurTargetEnemy = Me->FindTargetEnemy();

			Me->CanComboAttack = false;
			InputOn = false;
			Anim->JumpToAttackMontageSection(CurrentAttackNum++);
		}
	}
}

void UKS_LAttack::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("LAttack Exit"));
	Me->CanComboAttack = false;
	Me->CurrentAttackType = EAttackType::NONE;
	InputOn = false;
	Anim->Montage_Stop(.1f);
	Me->Axe->ActiveHitCollision(false);
	Me->Shield->ActiveHitCollision(false);
}


void UKS_LAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_LAttack::HandleLAttack(const FGenericStateParams& params)
{
	InputOn = true;
}

void UKS_LAttack::HandleGuard(const FGenericStateParams& params)
{
	bGuardInputOn = true;
	if (InputOn)
	{
		Me->SetKratosState(EPlayerState::LRunicAttack);
	}
}

void UKS_LAttack::HandleAim(const FGenericStateParams& params)
{
	bAimInputOn = true;
}

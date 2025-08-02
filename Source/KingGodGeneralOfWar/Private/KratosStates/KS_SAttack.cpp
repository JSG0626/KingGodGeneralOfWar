// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_SAttack.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"
#include <Kismet/KismetMathLibrary.h>
#include "WeaponInterface.h"
#include "Axe.h"
#include "SG_Shield.h"

void UKS_SAttack::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("SAttack Enter"));
	if (Me->bAxeGone)
	{
		// 맨손 공격
		return;
	}
	bGuardInputOn = false;
	CurrentAttackNum = 1;
	Anim->PlayMontage(EPlayerMontage::SAttack);
	CurrentAttackNum++;
	Me->CurrentAttackType = EAttackType::STRONG_ATTACK;
}

void UKS_SAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("SAttack Tick"), true);
	FRotator rotate = Me->GetControlRotation();
	rotate.Pitch = 0.0f;
	Me->SetActorRotation(UKismetMathLibrary::RLerp(Me->GetActorRotation(), rotate, DeltaTime * 4, true));

	UE_LOG(LogTemp, Display, TEXT("CCA: %d, InputOn: %d"), Me->CanComboAttack, InputOn);
	if (Me->CanComboAttack && InputOn)
	{
		Me->CanComboAttack = false;
		InputOn = false;
		Anim->JumpToAttackMontageSection(CurrentAttackNum++);
	}
}

void UKS_SAttack::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("WAttack Exit"));
	Me->CanComboAttack = false;
	Me->CurrentAttackType = EAttackType::NONE;
	InputOn = false;
	Anim->Montage_Stop(.1f);
	TScriptInterface<IWeaponInterface>(Me->Axe)->ActiveHitCollision(false);
	TScriptInterface<IWeaponInterface>(Me->Shield)->ActiveHitCollision(false);
}


void UKS_SAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_SAttack::HandleGuard(const FGenericStateParams& params)
{
	bGuardInputOn = true;
	if (InputOn)
	{
		//Me->SetKratosState(EPlayerState::RuneSAttack);
	}
}

void UKS_SAttack::HandleSAttack(const FGenericStateParams& params)
{
	InputOn = true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_WAttack.h"
#include "Kratos.h"
#include "Axe.h"
#include "SG_Shield.h"
#include "SG_KratosAnim.h"
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <WeaponInterface.h>
#include <Kismet/KismetMathLibrary.h>

void UKS_WAttack::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("WAttack Enter"));
	if (Me->bAxeGone) return;
	FRotator rotate = Me->CameraComp->GetComponentRotation();
	rotate.Pitch = 0;

	Anim->PlayWeakAttackMontage();
	Anim->JumpToAttackMontageSection(CurrentAttackNum++);

	Me->bIsAttacking = true;
	Me->CurrentAttackType = EAttackType::WEAK_ATTACK;
}

void UKS_WAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("WAttack Tick"), true);
	FRotator rotate = Me->CameraComp->GetComponentRotation();
	rotate.Pitch = 0.0f;
	Me->SetActorRotation(UKismetMathLibrary::RLerp(Me->GetActorRotation(), rotate, DeltaTime * 4, true));

	if (Me->CanComboAttack && InputOn)
	{
		UE_LOG(LogTemp, Display, TEXT("HandleWAttack, CurrentAttckNum: %d"), CurrentAttackNum);
		
		Me->CanComboAttack = false;
		InputOn = false;
		Anim->JumpToAttackMontageSection(CurrentAttackNum++);
		//UGameplayStatics::PlaySound2D(GetWorld(), Me->WeakAttackSoundArr[Me->CurrentWeakCombo], 1, 1, 0.8f);
	}
}

void UKS_WAttack::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("WAttack Exit"));
	Me->bIsAttacking = true;
	Me->CanComboAttack = false;
	CurrentAttackNum = 1;
	Me->CurrentAttackType = EAttackType::NONE;
	InputOn = false;
	Anim->Montage_Stop(.1f);
	Me->Axe->ActiveHitCollision(false);
	Me->Shield->ActiveHitCollision(false);
}


bool UKS_WAttack::CanHandleWAttack() const
{
	UE_LOG(LogTemp, Display, TEXT("CanHandleWAttack, %d"), Me->CanComboAttack);
	return Me->CanComboAttack;
}

void UKS_WAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_WAttack::HandleWAttack(const FGenericStateParams& params)
{
	UE_LOG(LogTemp, Display, TEXT("HandleWAttack, CurrentAttckNum: %d"), CurrentAttackNum);
	Me->CanComboAttack = false;
	Anim->JumpToAttackMontageSection(CurrentAttackNum++);
	UGameplayStatics::PlaySound2D(GetWorld(), Me->WeakAttackSoundArr[Me->CurrentWeakCombo], 1, 1, 0.8f);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Hit.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"

void UKS_Hit::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Hit Enter"));
	FEnemyAttackParams AttackParams = params.AttackParams;
	
	const float CurHP = Me->SetHP(Me->CurHP - AttackParams.Damage);

	if (CurHP == 0)
	{
		HandleDie();
		/*Anim->PlayHitMontage();
		Anim->JumpToHitMontageSection(TEXT("Death"));
		Me->TargetCameraOffset = FVector(0, 50, -60);
		Me->TargetCameraAngle = FRotator(10, 0, 0);
		Me->TargetTargetArmLength = 180;
		Me->CameraShakeOnAttack(EAttackDirectionType::DOWN, 1);*/
	}
	else
	{
		Anim->PlayHitMontage();
		Anim->JumpToHitMontageSection(Me->GetHitSectionName(AttackParams.HitType));
		if (AttackParams.HitType == EHitType::NB_HIGH)
		{
			Me->TargetCameraOffset = FVector(0, 50, -60);
			Me->TargetCameraAngle = FRotator(20, 0, 0);
			Me->TargetTargetArmLength = 190;
			Me->CameraShakeOnAttack(EAttackDirectionType::DOWN, 1);
		}
		else if (AttackParams.HitType == EHitType::STAGGER)
			Me->CameraShakeOnAttack(EAttackDirectionType::DOWN, .1);
		else
			Me->CameraShakeOnAttack(EAttackDirectionType::DOWN, .5);

	}
}

void UKS_Hit::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("Hit Tick"), true);
}

void UKS_Hit::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Hit Exit"));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Die.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"

void UKS_Die::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Die Enter"));

	Anim->PlayDieMontage();
	Me->TargetCameraOffset = FVector(0, 0, 0);
	Me->TargetCameraAngle = FRotator(-30, 0, 0);
	Me->TargetTargetArmLength = 250;
	Me->CameraShakeOnAttack(EAttackDirectionType::DOWN, 1);
}

void UKS_Die::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_Die::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Die Exit"));
}

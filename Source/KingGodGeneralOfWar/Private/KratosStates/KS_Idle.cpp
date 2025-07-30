// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Idle.h"
#include "SG_KratosAnim.h"
#include "Kratos.h"
#include <Kismet/KismetMathLibrary.h>

void UKS_Idle::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Idle Enter"));
	Me->TargetFOV = DefaultTargetFOV;
	Me->TargetCameraOffset = Me->DefaultCameraOffset;
	Me->TargetCameraAngle = FRotator(0);
	Me->TargetTargetArmLength = 143;
	Me->bIsDodging = false;
	Me->bIsAttacking = false;
	Me->TargetShieldScale = 0.0f;
	Anim->StopAllMontages(0.3f);

	Direction = Me->GetVelocity();
	Direction.Normalize();

}

void UKS_Idle::TickState( const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("Idle Tick"), true);

	CurrentMoveScale = FMath::Lerp(CurrentMoveScale, TargetMoveScale, DeltaTime * 8);
	Me->AddMovementInput(Direction, CurrentMoveScale);

	TickTime += DeltaTime;

	if (TickTime >= 2.0f)
	{
		Anim->ActiveLookAt(true);
	}
	LookAtProcess();
}

void UKS_Idle::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Idle Exit"));

	CurrentMoveScale = 0.56f;
	Anim->ActiveLookAt(false);
	TickTime = 0;
}

void UKS_Idle::HandleWAttack(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::WAttack);
}

void UKS_Idle::HandleGuard(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Guard);
}

void UKS_Idle::HandleAim(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Aim);
}

void UKS_Idle::HandleMove(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Move);
}

void UKS_Idle::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

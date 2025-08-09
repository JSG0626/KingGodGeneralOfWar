// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Idle.h"
#include "Kratos.h"
#include <Kismet/KismetMathLibrary.h>

void UKS_Idle::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Idle Enter"));
	Me->TargetFOV = DefaultTargetFOV;
	Me->TargetCameraOffset = Me->DefaultCameraOffset;
	Me->TargetCameraAngle = FRotator(0);
	Me->TargetTargetArmLength = 143;
	Me->TargetShieldScale = 0.0f;
	//Anim->StopAllMontages(0.3f);

	Direction = Me->GetVelocity();
	Direction.Normalize();

	TickTime = 0;
}

void UKS_Idle::TickState( const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("Idle Tick"), true);

	CurrentMoveScale = FMath::Lerp(CurrentMoveScale, TargetMoveScale, DeltaTime * 8);
	Me->AddMovementInput(Direction, CurrentMoveScale);

	TickTime += DeltaTime;

	if (TickTime >= 1.0f)
	{
		//LookAtProcess(DeltaTime);
	}
}

void UKS_Idle::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Idle Exit"));

	CurrentMoveScale = 0.56f;
}

void UKS_Idle::HandleLAttack(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::LAttack);
}

void UKS_Idle::HandleHAttack(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::HAttack);
}

void UKS_Idle::HandleGuard(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Guard);
}

void UKS_Idle::HandleAim(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Aim);
}

void UKS_Idle::HandleAbility(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Ability);
}

void UKS_Idle::HandleMove(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Move);
}

void UKS_Idle::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

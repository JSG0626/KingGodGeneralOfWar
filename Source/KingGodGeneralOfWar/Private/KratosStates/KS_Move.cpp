// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Move.h"
#include "Kratos.h"
#include <Kismet/KismetMathLibrary.h>
void UKS_Move::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Move Enter"));
}

void UKS_Move::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("Move Tick"), true);

	Me->SetActorRotation(UKismetMathLibrary::RLerp(Me->GetActorRotation(), TargetActorRotation, DeltaTime * 2, true));

	FRotator ControlRotation = Me->GetControlRotation();
	ControlRotation.Pitch = 0;
	FTransform T = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), ControlRotation, FVector(1, 1, 1));
	FVector ForwardDirection = UKismetMathLibrary::TransformDirection(T, Me->Direction);

	Me->PrevDirection = Me->Direction;
	Me->Direction = FVector(0, 0, 0);

	CurrentMoveScale = FMath::Lerp(CurrentMoveScale, TargetMoveScale, DeltaTime * 6);

	Me->AddMovementInput(ForwardDirection, CurrentMoveScale);
}

void UKS_Move::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Move Exit"));
	CurrentMoveScale = 0.0f;
}

void UKS_Move::HandleMove(const FGenericStateParams& params)
{
	TargetActorRotation = FRotator(0, Me->GetControlRotation().Yaw, 0);
	TargetMoveScale = Me->bIsRunning ? RunMoveScale : WalkMoveScale;
}

void UKS_Move::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge, FGenericStateParams(Me->PrevDirection));
}

void UKS_Move::HandleIdle(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Idle);
}

void UKS_Move::HandleWAttack(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::WAttack);
}

void UKS_Move::HandleSAttack(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::SAttack);
}

void UKS_Move::HandleGuard(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Guard);
}

void UKS_Move::HandleAim(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Aim);

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Ability.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"
#include <Kismet/KismetMathLibrary.h>
#include "Camera/CameraComponent.h"

void UKS_Ability::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Ability Enter"));
	if (Me->bAxeGone)
	{
		Anim->PlayMontage(EPlayerMontage::CallAxe);
		Me->CallAxe();
		Anim->bRecallAxe = true;
	}
}

void UKS_Ability::TickState(const FGenericStateParams& params, float DeltaTime)
{
	//Me->SetActorRotation(FMath::RInterpTo(Me->GetActorRotation(), Me->CameraComp->GetComponentRotation(), DeltaTime * 24, true));

	FRotator ControlRotation = Me->GetControlRotation();
	ControlRotation.Pitch = 0;
	FTransform T(ControlRotation, FVector::ZeroVector, FVector::OneVector);
	FVector ForwardDirection = UKismetMathLibrary::TransformDirection(T, Me->Direction);

	Me->PrevDirection = Me->Direction;
	Me->Direction = FVector(0, 0, 0);

	Me->AddMovementInput(ForwardDirection, WalkMoveScale);
}

void UKS_Ability::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Ability Exit"));
}

void UKS_Ability::HandleIdle(const FGenericStateParams& params)
{
}

void UKS_Ability::HandleMove(const FGenericStateParams& params)
{
	TargetActorRotation = FRotator(0, Me->GetControlRotation().Yaw, 0);
}

void UKS_Ability::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_Ability::HandleGrabAxe(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::GrabAxe);
}

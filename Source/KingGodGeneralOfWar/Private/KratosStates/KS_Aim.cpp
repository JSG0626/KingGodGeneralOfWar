// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Aim.h"
#include "Kratos.h"
#include <Kismet/KismetMathLibrary.h>
#include "PlayerAimUI.h"


void UKS_Aim::EnterState(const FGenericStateParams& params)
{
	Super::EnterState(params);
	StateLog(TEXT("Aim Enter"));
	CoolDown = 0.0f;

	if (params.Bool)
	{
		if (params.Integer == 0)
		{
			HandleLAttack();
		}
		else
		{
			HandleHAttack();
		}
		Me->SetKratosState(EPlayerState::Idle);
	}
}

void UKS_Aim::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("Aim Tick"), true);
	CoolDown -= DeltaTime;

	FRotator CameraRotator = Me->GetCameraRotation();
	Me->SetActorRotation(CameraRotator);
	{
		FRotator ControlRotation = Me->GetControlRotation();
		if (abs(Me->GetVelocity().Dot(Me->GetActorRightVector())) >= 0.5f)
		{
			ControlRotation.Yaw -= 20.0f;
		}
	}
	

	FRotator ControlRotation = Me->GetControlRotation();
	ControlRotation.Pitch = 0;
	FTransform T = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), ControlRotation, FVector(1, 1, 1));
	FVector ForwardDirection = UKismetMathLibrary::TransformDirection(T, Me->Direction);

	Me->PrevDirection = Me->Direction;
	Me->Direction = FVector(0, 0, 0);

	Me->AddMovementInput(ForwardDirection, WalkMoveScale);
	
	{
		FHitResult hitResult;
		FVector Start = Me->GetCameraLocation();
		FVector End = Start + Me->GetCameraRotation().Vector() * AIM_UI_MAX_DIST;
		FCollisionObjectQueryParams traceParams;
		traceParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
		bool bHit = GetWorld()->LineTraceSingleByObjectType(hitResult, Start, End, traceParams);
		Me->ChangeAimUIColor(bHit);
	}
}

void UKS_Aim::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Aim Exit"));
}

void UKS_Aim::HandleIdle(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Idle);
}

void UKS_Aim::HandleMove(const FGenericStateParams& params)
{
}

void UKS_Aim::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_Aim::HandleLAttack(const FGenericStateParams& params)
{
	StateLog(TEXT("Throw Axe"));

	if (!Me->bAxeGone && CoolDown <= 0)
	{
		CoolDown = THROW_AXE_COOLDOWN;
		Me->PlayMontage(EPlayerMontage::LightThrowAxe);
	}
}

void UKS_Aim::HandleHAttack(const FGenericStateParams& params)
{
	if (!Me->bAxeGone && CoolDown <= 0)
	{
		CoolDown = THROW_AXE_COOLDOWN;
		Me->PlayMontage(EPlayerMontage::HeavyThrowAxe);
	}
}

void UKS_Aim::HandleAbility(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Ability);
}

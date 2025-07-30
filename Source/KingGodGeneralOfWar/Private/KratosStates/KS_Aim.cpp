// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Aim.h"
#include "Kratos.h"
#include <Kismet/KismetMathLibrary.h>
#include "PlayerAimUI.h"
#include "Camera/CameraComponent.h"
#include "SG_KratosAnim.h"

void UKS_Aim::SetUp(AKratos* Kratos)
{
	IKratosState::SetUp(Kratos);

	AimWidget = Me->AimWidget;
}

void UKS_Aim::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Aim Enter"));

	AimWidget->SetVisibility(ESlateVisibility::Visible);
	Me->TargetFOV = AIM_FOV;
	Anim->ActiveLookAt(true);
}

void UKS_Aim::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("Aim Tick"), true);

	FRotator CameraRotator = Me->CameraComp->GetComponentRotation();
	CameraRotator.Pitch = 0;
	Me->SetActorRotation(UKismetMathLibrary::RLerp(Me->GetActorRotation(), CameraRotator, DeltaTime * 16, true));

	{
		FRotator ControlRotation = Me->GetControlRotation();
		if (abs(Me->GetVelocity().Dot(Me->GetActorRightVector())) >= 0.5f)
		{
			ControlRotation.Yaw -= 20.0f;
		}
		DrawDebugLine(GetWorld(), Me->GetMesh()->GetBoneLocation(FName(TEXT("neck_01"))), Me->GetMesh()->GetBoneLocation(FName(TEXT("neck_01"))) + ControlRotation.Vector() * 500, FColor::Red);
		Anim->LookAtTarget = Me->GetMesh()->GetBoneLocation(FName(TEXT("neck_01"))) + ControlRotation.Vector() * 500;
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
		FVector Start = Me->CameraComp->GetComponentLocation();
		FVector End = Start + Me->CameraComp->GetForwardVector() * AIM_UI_MAX_DIST;
		FCollisionObjectQueryParams traceParams;
		traceParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
		bool bHit = GetWorld()->LineTraceSingleByObjectType(hitResult, Start, End, traceParams);
		AimWidget->ChangeColor(bHit);
	}
}

void UKS_Aim::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Aim Exit"));
	AimWidget->SetVisibility(ESlateVisibility::Hidden);
	Me->TargetFOV = Me->DEFAULT_FOV;
	Anim->ActiveLookAt(false);
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

void UKS_Aim::HandleWAttack(const FGenericStateParams& params)
{
	StateLog(TEXT("Throw Axe"));

	Anim->PlayAxeThrowMontage();
}

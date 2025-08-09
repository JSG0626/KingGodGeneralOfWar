// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_KratosAnim.h"

#include "KingGodGeneralOfWar.h"
#include "RuneAttackField.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "SG_Shield.h"

USG_KratosAnim::USG_KratosAnim()
{
	
}

void USG_KratosAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	UpdatePlayerState();
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("RecallAxe: %d"), bRecallAxe));
}

void USG_KratosAnim::UpdatePlayerState()
{
	if (Me)
	{
		FVector velocity = Me->GetVelocity();
		FVector forwardVector = Me->GetActorForwardVector();

		PlayerState = Me->GetState();
		Speed = FVector::DotProduct(forwardVector, velocity);
		Direction = FVector::DotProduct(Me->GetActorRightVector(), velocity);
	}
}

void USG_KratosAnim::PlayMontage(const EPlayerMontage State, bool bJumpSection, const FString SectionName)
{
	UAnimMontage* Montage = MontageMap[State];
	if (Montage)
	{
		Montage_Play(Montage);
		if (bJumpSection)
		{
			Montage_JumpToSection(FName(*SectionName), Montage);
		}
	}
}

void USG_KratosAnim::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSection(NewSection), nullptr);
}

void USG_KratosAnim::ActiveLookAt(bool Active)
{
	if (Active == bActiveLookAt)
	{
		return;
	}

	if (!Active)
	{
		bActiveLookAt = false;
		return;
	}
	FRotator ActorRotation = Me->GetActorRotation();
	FRotator ControlRotation = Me->GetControlRotation();
	FRotator RelativeControlRotation = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation);
	float ClampedRelativePitch = FMath::ClampAngle(RelativeControlRotation.Pitch, -20.0f, 20.0f); // 상하 제한
	float ClampedRelativeYaw = FMath::ClampAngle(RelativeControlRotation.Yaw, -60.0f, 60.0f);   // 좌우 제한

	FRotator ClampedRelativeRotator = FRotator(ClampedRelativePitch, ClampedRelativeYaw, 0.0f);

	FRotator LookAtWorldRotation = ActorRotation + ClampedRelativeRotator;

	FVector NeckBoneLocation = FVector::ZeroVector;
	if (Me && Me->GetMesh())
	{
		NeckBoneLocation = Me->GetMesh()->GetBoneLocation(FName(TEXT("neck_01")));
	}
	else
	{
		NeckBoneLocation = Me->GetActorLocation() + FVector(0, 0, Me->GetDefaultHalfHeight());
	}

	float LookAtDistance = 500.0f;
	LookAtTarget = NeckBoneLocation + LookAtWorldRotation.Vector() * LookAtDistance;
	bActiveLookAt = true;
}

void USG_KratosAnim::AnimNotify_AbleNextAttack()
{
	Me->CanComboAttack = true;
}

void USG_KratosAnim::AnimNotify_EndDodge()
{
	Me->bEvade = false;
}

void USG_KratosAnim::AnimNotify_FaceCameraForward()
{
	//Me->GetState()->bFaceCameraForward = false;
}

FName USG_KratosAnim::GetAttackMontageSection(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
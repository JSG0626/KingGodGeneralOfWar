// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_HDashAttack.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"

void UKS_HDashAttack::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("HDashAttack Enter"));
	Me->CanComboAttack = false;

	Anim->PlayMontage(EPlayerMontage::HDashAttack);
	InitVelocity = Me->GetVelocity();

	CurrentSpeedScale = 1;
	bFaceCameraForward = true;

	InputOn = false;
	bLAttackInputOn = false;
	bHAttackInputOn = false;
	Me->CurTargetEnemy = Me->FindTargetEnemy();
}

void UKS_HDashAttack::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("HDashAttack Tick"), true);

	if (nullptr != Me->CurTargetEnemy)
	{
		const FVector ToTarget = (Me->CurTargetEnemy->GetActorLocation() - Me->GetActorLocation()).GetSafeNormal();
		if (Me->bFaceEnemy)
		{
			FRotator rotate = ToTarget.Rotation();
			rotate.Pitch = 0.0f;
			Me->SetActorRotation(rotate);
		}
		float DistSquared = FVector::DistSquared(Me->CurTargetEnemy->GetActorLocation(), Me->GetActorLocation());
		if (Me->bTraceEnemy && DistSquared >= Me->AttackRangeSquared)
		{
			UE_LOG(LogTemp, Display, TEXT("TraceEnemy"));
			Me->AddMovementInput(ToTarget, 1.0f);
		}
	}
	else
	{
		FRotator rotate = Me->GetControlRotation();
		rotate.Pitch = 0.0f;
		Me->SetActorRotation(rotate);
	}

	if (Me->CanComboAttack)
	{
		if (bLAttackInputOn)
		{
			Me->CanComboAttack = false;
			Me->SetKratosState(EPlayerState::LAttack);
		}
		else if (bHAttackInputOn)
		{
			Me->CanComboAttack = false;
			Me->SetKratosState(EPlayerState::HAttack);
		}
	}

	//Me->AddMovementInput(Me->GetActorForwardVector(), CurrentSpeedScale);

	//CurrentSpeedScale = FMath::Max(0, CurrentSpeedScale - DeltaTime * 0.5f);
}

void UKS_HDashAttack::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("HDashAttack Exit"));
}

void UKS_HDashAttack::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_HDashAttack::HandleLAttack(const FGenericStateParams& params)
{
	bLAttackInputOn = true;
}

void UKS_HDashAttack::HandleHAttack(const FGenericStateParams& params)
{
	bHAttackInputOn = true;
}

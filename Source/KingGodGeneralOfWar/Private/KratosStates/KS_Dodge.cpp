// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Dodge.h"
#include "SG_KratosAnim.h"
#include <Kismet/KismetMathLibrary.h>

void UKS_Dodge::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Dodge Enter"));
	bLAttackInputOn = false;
	bHAttackInputOn = false;
	bAimInputOn = false;
	Me->CanComboAttack = false;

	FRotator rotate = Me->GetController()->GetControlRotation();
	rotate.Pitch = 0;
	Me->SetActorRotation(rotate);

	// Roll
	if (Me->GetVelocity().Size() >= RollVelocityThreshhold)
	{
		Anim->PlayMontage(EPlayerMontage::Roll);
		Me->bEvade = true;
	}
	// Dash
	else
	{
		Anim->PlayMontage(EPlayerMontage::Dodge);
		bDashing = true;
	}
	FString DodgeDirString = GetDodgeDirection(Me->PrevDirection);
	Anim->Montage_JumpToSection(*DodgeDirString);
}

void UKS_Dodge::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("Dodge Tick"), true);

	if (Me->CanComboAttack)
	{
		UE_LOG(LogTemp, Display, TEXT("CacComboAttack: %d, bAimInputOn: %d, bLAttackInputOn: %d, bHAttackInputOn: %d"),	Me->CanComboAttack, bAimInputOn, bLAttackInputOn, bHAttackInputOn);
		if (bAimInputOn && (bLAttackInputOn || bHAttackInputOn))
		{
			FGenericStateParams Params;
			Params.Bool = true;
			if (bHAttackInputOn)
			{
				Params.Integer = 1;
			}
			Me->SetKratosState(EPlayerState::Aim, Params);
		}
		else if (bLAttackInputOn)
		{

		}
		else if (bHAttackInputOn)
		{
			
		}
	}
}

void UKS_Dodge::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Dodge Exit"));
	bDashing = false;
}

bool UKS_Dodge::CanHandleHit() const
{
	return !bDashing && !Me->bEvade;
}

void UKS_Dodge::HandleDodge(const FGenericStateParams& params)
{
	StateLog(TEXT("Dash -> Roll"));
	bDashing = false;

	Me->bEvade = true;

	FRotator rotate = Me->GetController()->GetControlRotation();
	rotate.Pitch = 0;
	Me->SetActorRotation(rotate);
	FString DodgeDirString = GetDodgeDirection(Me->PrevDirection);
	StateLog(FString::Printf(TEXT("DodgeString: %s"), *DodgeDirString));

	Anim->PlayMontage(EPlayerMontage::Roll, true, DodgeDirString);
}

void UKS_Dodge::HandleLAttack(const FGenericStateParams& params)
{
	bLAttackInputOn = true;
}

void UKS_Dodge::HandleHAttack(const FGenericStateParams& params)
{
	bHAttackInputOn = true;
}

void UKS_Dodge::HandleAim(const FGenericStateParams& params)
{
	UE_LOG(LogTemp, Display, TEXT("HandleAim"));
	bAimInputOn = true;
}

FString UKS_Dodge::GetDodgeDirection(const FVector& Direction) const
{
	float absX = abs(Direction.X), absY = abs(Direction.Y);
	FString DodgeDirString = "";
	UE_LOG(LogTemp, Display, TEXT("Direction: %s"), *Direction.ToString());
	if (absY >= 0.7)
	{
		if (Direction.Y >= 0.7)
			DodgeDirString += TEXT("R");
		else
			DodgeDirString += TEXT("L");
	}
	if (absX >= 0.7f)
	{
		if (Direction.X >= 0.7)
			DodgeDirString += TEXT("F");
		else
			DodgeDirString += TEXT("B");
	}
	return DodgeDirString;
}

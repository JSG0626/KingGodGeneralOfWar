// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Dodge.h"
#include "SG_KratosAnim.h"
#include <Kismet/KismetMathLibrary.h>

void UKS_Dodge::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Dodge Enter"));
	AttackInput = EAttackType::None;
	bAimInputOn = false;
	Me->CanComboAttack = false;
	InputDirection = FVector2D::ZeroVector;
	TickTime = 0.0f;
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
	TickTime += DeltaTime;
	//if (Me->CanComboAttack)
	//{
	//	if (bAimInputOn && (AttackInput != EAttackType::None))
	//	{
	//		Me->CanComboAttack = false;
	//		FGenericStateParams Params;
	//		Params.Bool = true;
	//		Params.Integer = static_cast<int32>(AttackInput);
	//		Me->SetKratosState(EPlayerState::Aim, Params);
	//		return;
	//	}
	//}
	UE_LOG(LogTemp, Display, TEXT("AttackInput == LAttack, InputDirection :%s"), *InputDirection.ToString());

	if (AttackInput != EAttackType::None)
	{
		if (bDashing && TickTime >= DashDodgeAttackTimeThreshold ||
			!bDashing && TickTime >= RollDodgeAttackTimeThreshold)
		{
			if (InputDirection.X <= -0.7f)
			{
				UE_LOG(LogTemp, Display, TEXT("BackWard"));
				Me->SetKratosState(EPlayerState::DodgeBackAttack);
			}
			else if (InputDirection.X >= 0.7f)
			{
				UE_LOG(LogTemp, Display, TEXT("Front"));
				Me->SetKratosState(EPlayerState::DodgeFrontAttack);
			}
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

void UKS_Dodge::HandleMove(const FGenericStateParams& params)
{
	InputDirection = params.Vector2D;
}

void UKS_Dodge::HandleDodge(const FGenericStateParams& params)
{
	StateLog(TEXT("Dash -> Roll"));
	bDashing = false;
	TickTime = 0.0f;
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
	AttackInput = EAttackType::LAttack;
}

void UKS_Dodge::HandleHAttack(const FGenericStateParams& params)
{
	AttackInput = EAttackType::HAttack;
}

void UKS_Dodge::HandleAim(const FGenericStateParams& params)
{
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

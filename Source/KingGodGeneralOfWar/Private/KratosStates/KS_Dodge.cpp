// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Dodge.h"
#include "SG_KratosAnim.h"
#include <Kismet/KismetMathLibrary.h>

void UKS_Dodge::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Dodge Enter"));
	FRotator rotate = Me->GetController()->GetControlRotation();
	rotate.Pitch = 0;
	Me->SetActorRotation(rotate);
	Anim->PlayMontage(EPlayerMontage::Dodge);

	// Roll
	if (Me->GetVelocity().Size() >= Me->RollVelocityThreshhold)
	{
		Me->bEvade = true;
	}
	// Dash
	else
	{
		bDashing = true;
	}
	FString DodgeDirString = GetDodgeDirection(Me->PrevDirection);
	Anim->Montage_JumpToSection(*DodgeDirString);
}

void UKS_Dodge::TickState(const FGenericStateParams& params, float DeltaTime)
{

	StateLog(TEXT("Dodge Tick"), true);
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

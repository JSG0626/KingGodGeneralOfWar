// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Guard.h"
#include "Kratos.h"
#include "SG_KratosAnim.h"
#include "SG_Shield.h"
#include <Kismet/KismetMathLibrary.h>
#include <NiagaraFunctionLibrary.h>
#include <Components/ArrowComponent.h>
void UKS_Guard::SetUp(AKratos* kratos)
{
	IKratosState::SetUp(kratos);
	Shield = kratos->Shield;

}

void UKS_Guard::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Guard Enter"));
	Anim->PlayGuardMontage();

	Shield->SetTargetScale(true);
	Anim->bMeshSpaceRotationBlend = true;

	FRotator ControlRotation = Me->GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	Me->ActiveLerpPlayerRotation(ControlRotation, 20);

	TickTime = 0.0f;
}

void UKS_Guard::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("Guard Tick"), true);

	TickTime += DeltaTime;
}

void UKS_Guard::ExitState(const FGenericStateParams& params)
{
	Anim->Montage_Stop(0.1f);
	StateLog(TEXT("Guard Exit"));
	Shield->SetTargetScale(false);
	Anim->bMeshSpaceRotationBlend = false;
}


void UKS_Guard::HandleIdle(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Idle);
}

void UKS_Guard::HandleMove(const FGenericStateParams& params)
{
	FRotator ControlRotation = Me->GetControlRotation();
	FTransform T = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), ControlRotation, FVector(1, 1, 1));
	FVector ForwardDirection = UKismetMathLibrary::TransformDirection(T, Me->Direction);

	Me->PrevDirection = Me->Direction;
	Me->Direction = FVector(0, 0, 0);

	Me->AddMovementInput(ForwardDirection, WalkMoveScale);
}

void UKS_Guard::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_Guard::HandleWAttack(const FGenericStateParams& params)
{
	if (Me->bAxeGone)
	{
		StateLog(TEXT("Axe Return Attack"));
	}
	else
	{
		Me->SetKratosState(EPlayerState::RuneWAttack);
	}
}

void UKS_Guard::HandleHit(const FGenericStateParams& params)
{
	FEnemyAttackParams AttackParams = params.AttackParams;
	// 가드 가능한 공격에 대한 로직
	if (AttackParams.bGuardable)
	{
		if (TickTime > PARRIABLE_TIME)
		{
			Anim->JumpToGuardMontageSection(TEXT("Guard_Block"));

			Me->LaunchCharacter(Me->GetActorForwardVector() * -1 * 1500, true, false);
			GetWorld()->SpawnActor<AActor>(Me->GuardBlockLightFactory, Shield->GetActorTransform())->AttachToActor(Shield, FAttachmentTransformRules::KeepWorldTransform);
			UNiagaraFunctionLibrary::SpawnSystemAttached(Me->GuardBlockVFX, Shield->LightPosition, TEXT("GuardBlockVFX"), Shield->LightPosition->GetComponentLocation(),
				Shield->LightPosition->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true);
		}
		// 패링 가능 상태
		else
		{
			HandleParry(params);
		}
	}
	// 가드 불능 공격
	else
	{
		Me->SetKratosState(EPlayerState::Hit);
	}
}

void UKS_Guard::HandleParry(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Parry, params);
}

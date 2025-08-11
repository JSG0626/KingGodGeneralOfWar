// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Guard.h"
#include "Kratos.h"
#include "SG_Shield.h"
#include <Kismet/KismetMathLibrary.h>
#include <NiagaraFunctionLibrary.h>
#include <Components/ArrowComponent.h>

void UKS_Guard::SetUp(AKratos* kratos)
{
	UKratosState::SetUp(kratos);
	Shield = kratos->GetShield();
}

void UKS_Guard::EnterState(const FGenericStateParams& params)
{
	Super::EnterState(params);
	StateLog(TEXT("Guard Enter"));
	Me->PlayMontage(EPlayerMontage::Guard);
	Shield->SetTargetScale(true);
	Me->SetMeshSpaceRotationBlend(true);

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
	Me->StopAnimMontage();
	StateLog(TEXT("Guard Exit"));
	Shield->SetTargetScale(false);
	Me->SetMeshSpaceRotationBlend(false);
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

void UKS_Guard::HandleLAttack(const FGenericStateParams& params)
{
	if (Me->bAxeGone)
	{
		Me->SetKratosState(EPlayerState::AxeReturnLAttack);
	}
	else
	{
		Me->SetKratosState(EPlayerState::LRunicAttack);
	}
}

void UKS_Guard::HandleHAttack(const FGenericStateParams& params)
{
	if (Me->bAxeGone)
	{
		Me->SetKratosState(EPlayerState::AxeReturnHAttack);
	}
	else
	{
		Me->SetKratosState(EPlayerState::HRunicAttack);
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
			Me->Montage_JumpToSection(TEXT("Guard_Block"));
			Me->LaunchCharacter(Me->GetActorForwardVector() * -1 * 750, true, false);
			GetWorld()->SpawnActor<AActor>(GuardBlockLightFactory, Shield->GetActorTransform())->AttachToActor(Shield, FAttachmentTransformRules::KeepWorldTransform);
			UNiagaraFunctionLibrary::SpawnSystemAttached(GuardBlockVFX, Shield->LightPosition, TEXT("GuardBlockVFX"), Shield->LightPosition->GetComponentLocation(),
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

void UKS_Guard::HandleAbility(const FGenericStateParams& params)
{
	/*Me->SetKratosState(EPlayerState::Ability);*/
	if (Me->bAxeGone)
	{
		Me->PlayMontage(EPlayerMontage::CallAxe);
		Me->CallAxe();
	}
}

void UKS_Guard::HandleGrabAxe(const FGenericStateParams& params)
{
	Me->PlayMontage(EPlayerMontage::GrabAxe);
}

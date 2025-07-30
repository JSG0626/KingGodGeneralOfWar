// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_Parry.h"
#include "Kratos.h"
#include "SG_Shield.h"
#include "SG_KratosAnim.h"
#include <NiagaraFunctionLibrary.h>
#include <Components/ArrowComponent.h>
#include <BaseEnemy.h>
#include <WeaponInterface.h>

void UKS_Parry::SetUp(AKratos* Kratos)
{
	IKratosState::SetUp(Kratos);
	Shield = Kratos->Shield;
}

void UKS_Parry::EnterState(const FGenericStateParams& params)
{
	StateLog(TEXT("Parry Enter"));
	Shield->SetTargetScale(true);
	FEnemyAttackParams AttackParams = params.AttackParams;

	GetWorld()->SpawnActor<AActor>(Me->ParryingLightFactory, Shield->LightPosition->GetComponentTransform())->AttachToActor(Shield, FAttachmentTransformRules::SnapToTargetIncludingScale);
	//UNiagaraFunctionLibrary::SpawnSystemAttached(Me->ParryVFX, Shield->LightPosition, TEXT("ParryVFX"), Shield->LightPosition->GetComponentLocation(), Shield->LightPosition->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true);
	UNiagaraFunctionLibrary::SpawnSystemAttached(Me->ShockWaveVFX, Shield->LightPosition, TEXT("ShockWaveVFX"), Shield->LightPosition->GetComponentLocation(), Shield->LightPosition->GetComponentRotation(), EAttachLocation::KeepWorldPosition, true);

	Me->CameraShakeOnAttack(EAttackDirectionType::DOWN, 0.5f);
	Anim->PlayParryMontage();
	//Anim->JumpToGuardMontageSection(TEXT("Guard_Parrying"));
	if (AttackParams.bMelee)
	{
		//auto* Thor = Cast<ABDThor>(Attacker);

		//if (Thor)
		//{
		//	Thor->fsm->Damage(PARRYING_DAMAGE, EAttackDirectionType::UP);
		//}
		//else
		//{
		//	auto AwakenThor = Cast<AAwakenThor>(Attacker);

		//	//AwakenThor->getFSM()->SetDamage(PARRYING_DAMAGE, EAttackDirectionType::UP, true);
		//	bool bThorDead = AwakenThor->getFSM()->SetDamage(PARRYING_DAMAGE, EAttackDirectionType::UP);
		//	if (bThorDead)
		//	{
		//		SetState(EPlayerState::NoneMovable);
		//	}
		//}
		if (AttackParams.Attacker)
		{
			ABaseEnemy* Enemy = Cast<ABaseEnemy>(AttackParams.Attacker);
			if (Enemy)
			{
				Enemy->TakeDamage(FGenericAttackParams(Me, PARRY_DAMAGE, PARRY_STUN_DAMAGE, EAttackDirectionType::LEFT));
			}
			else
			{

			}
		}
	}
}

void UKS_Parry::TickState(const FGenericStateParams& params, float DeltaTime)
{
	StateLog(TEXT("Parry Tick"), true);
}

void UKS_Parry::ExitState(const FGenericStateParams& params)
{
	StateLog(TEXT("Parry Exit"));
	Shield->SetTargetScale(false);
}

void UKS_Parry::HandleWAttack(const FGenericStateParams& params)
{
}

void UKS_Parry::HandleDodge(const FGenericStateParams& params)
{
}

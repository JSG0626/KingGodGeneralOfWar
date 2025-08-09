// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KS_CombatIdle.h"
#include "Kratos.h"
#include <Kismet/GameplayStatics.h>

void UKS_CombatIdle::EnterState(const FGenericStateParams& params)
{
	Me->CameraShakeOnAttack(EAttackDirectionType::DOWN, 0.1f);
	Me->PlayMontage(EPlayerMontage::CombatIdle);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CombatReadySound, Me->GetActorLocation());
}

void UKS_CombatIdle::TickState(const FGenericStateParams& params, float DeltaTime)
{
}

void UKS_CombatIdle::ExitState(const FGenericStateParams& params)
{
	Me->StopAnimMontage();
}

void UKS_CombatIdle::HandleMove(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Move);
}

void UKS_CombatIdle::HandleDodge(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Dodge);
}

void UKS_CombatIdle::HandleGuard(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Guard);
}

void UKS_CombatIdle::HandleAim(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Aim);
}

void UKS_CombatIdle::HandleLAttack(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::CombatIdleLAttack);
}

void UKS_CombatIdle::HandleHAttack(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::CombatIdleHAttack);
}

void UKS_CombatIdle::HandleAbility(const FGenericStateParams& params)
{

}

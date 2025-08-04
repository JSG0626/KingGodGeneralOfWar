// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInterface.h"
#include "BaseEnemy.h"
#include "Sound/SoundCue.h"
#include <Kismet/GameplayStatics.h>

void IWeaponInterface::SetAttackScale(const float AttackScale)
{
	CurrentAttackScale = AttackScale;
}

void IWeaponInterface::DealDamage(ABaseEnemy* Target, const FGenericAttackParams& params)
{
    UGameplayStatics::PlaySoundAtLocation(
        Target->GetWorld(),
        GetBaseHitSound(),
        Target->GetActorLocation()
    );
	Target->GetDamage(params);
}

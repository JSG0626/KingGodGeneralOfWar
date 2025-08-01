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
        Target->GetWorld(),             // 월드 컨텍스트를 제공하는 오브젝트 (보통 this를 사용)
        GetBaseHitSound(),  // 재생할 사운드 큐
        Target->GetActorLocation()       // 사운드가 발생할 월드 위치
    );
	Target->GetDamage(params);
}

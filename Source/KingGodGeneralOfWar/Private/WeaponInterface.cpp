// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInterface.h"
#include "BaseEnemy.h"

void IWeaponInterface::DealDamage(ABaseEnemy* Target, const FGenericAttackParams& params)
{
	Target->GetDamage(params);
}

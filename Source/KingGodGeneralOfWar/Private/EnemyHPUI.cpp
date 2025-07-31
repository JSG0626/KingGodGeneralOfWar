// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHPUI.h"
#include "Components/ProgressBar.h"

void UEnemyHPUI::SetHPBar(float Percent)
{
	HPPercent = Percent;
}

void UEnemyHPUI::SetStunGuageBar(float Percent)
{
	StunGuagePercent = Percent;
}

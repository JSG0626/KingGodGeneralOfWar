// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "WeaponInterface.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	CurHP = MaxHP;
}

void ABaseEnemy::SetHP(float Damage)
{
	CurHP = FMath::Max(0, CurHP - Damage);
}

void ABaseEnemy::SetStunGuage(float StunDamage)
{
	CurStunGuage = FMath::Max(0, CurStunGuage - StunDamage);
}

void ABaseEnemy::DieProcess()
{

}

void ABaseEnemy::StunProcess()
{
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABaseEnemy::TakeDamage(const FGenericAttackParams& params)
{
	SetHP(params.Damage);
	if (CurHP == 0)
	{
		DieProcess();
	}
	SetStunGuage(params.StunDamage);
	if (CurStunGuage)
	{
		StunProcess();
	}
	return false;
}


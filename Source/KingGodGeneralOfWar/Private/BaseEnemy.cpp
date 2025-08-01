// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "WeaponInterface.h"
#include "EnemyHPUI.h"
#include <Components/WidgetComponent.h>
#include "Components/CapsuleComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HPUIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPUIComp"));
	HPUIComp->SetupAttachment(GetMesh());
	HPUIComp->SetRelativeLocation(FVector{ 0, 0, 180 });
	HPUIComp->SetWidgetClass(HPUIFactory);
	HPUIComp->SetWidgetSpace(EWidgetSpace::Screen);
	HPUIComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FClassFinder<UUserWidget> TempLockOnUIClass(TEXT("/ Script / UMGEditor.WidgetBlueprint'/Game/JSG/UI/WBP_LockOn.WBP_LockOn'"));
	if (TempLockOnUIClass.Succeeded())	LockOnUIFactory = TempLockOnUIClass.Class;
	
	LockOnUIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockUIComp"));
	LockOnUIComp->SetupAttachment(GetMesh());
	LockOnUIComp->SetWidgetClass(LockOnUIFactory);
	LockOnUIComp->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnUIComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"), true);
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	CurHP = MaxHP;
	CurStunGuage = MaxStunGuage;
	HPUI = Cast<UEnemyHPUI>(HPUIComp->GetWidget());
	if (HPUI)
	{
		HPUI->SetHPBar(1);
		HPUI->SetStunGuageBar(1);
	}

	LockOnUI = Cast<UUserWidget>(LockOnUIComp->GetWidget());
	check(LockOnUI);
}

void ABaseEnemy::SetHP(float Damage)
{
	CurHP = FMath::Max(0, CurHP - Damage);
	if (HPUI)
	{
		HPUI->SetHPBar(CurHP / MaxHP);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("HPUI is nullptr"));
	}
}

void ABaseEnemy::SetStunGuage(float StunDamage)
{
	CurStunGuage = FMath::Max(0, CurStunGuage - StunDamage);
	UE_LOG(LogTemp, Display, TEXT("CurStunGuage: %f"), CurStunGuage);
	if (HPUI)
	{
		HPUI->SetStunGuageBar(CurStunGuage / MaxStunGuage);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("HPUI is nullptr"));
	}
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

void ABaseEnemy::ActiveLockOnUI(bool ActiveState)
{
	if (LockOnUI)
	{
		LockOnUIComp->SetVisibility(ActiveState);
		//LockOnUI->SetVisibility(ActiveState ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

bool ABaseEnemy::GetDamage(const FGenericAttackParams& params)
{
	CurrentTarget = params.Instigator;
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


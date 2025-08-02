// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_Shield.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "BDThor.h"
#include "CSW/AwakenThor.h"
#include "BDThorFSM.h"
#include "CSW/AwakenThorFSM.h"
#include "BaseEnemy.h"

const float SHIELD_DAMAGE = 2;

// Sets default values
ASG_Shield::ASG_Shield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	LightPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("LightPosition"));
	LightPosition->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ASG_Shield::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASG_Shield::OnShieldAttackOverlap);
}

// Called every frame
void ASG_Shield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (abs(CurrentScale - TargetScale) >= 0.001f)
	{
		LerpScale(DeltaTime);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, FString::Printf(TEXT("Shield Rotator: %s"), *MeshComp->GetComponentRotation().ToString()));
}

void ASG_Shield::OnShieldAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Me->SetAnimationSpeedSlow(0.1f, 0.01f);
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
	if (Enemy)
	{
		DealDamage(Enemy, FGenericAttackParams(Me, BaseAttackPower * CurrentAttackScale, CurrentStunAttackScale, EAttackDirectionType::UP));
	}
	else
	{
		//auto* Thor = Cast<ABDThor>(OtherActor);

		//if (Thor)
		//{
		//	Thor->fsm->Damage(SHIELD_DAMAGE, EAttackDirectionType::UP);
		//}
		//else
		//{
		//	auto AwakenThor = Cast<AAwakenThor>(OtherActor);

		//	bool bThorDead = AwakenThor->getFSM()->SetDamage(SHIELD_DAMAGE, EAttackDirectionType::UP);
		//	if (bThorDead)
		//	{
		//		auto* Me = Cast<AKratos>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		//	}
		//}

		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01f);
		//FTimerHandle handle;
		//GetWorld()->GetTimerManager().SetTimer(handle, [&]()
		//	{
		//		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		//	}, 0.001f, false);
	}
}

void ASG_Shield::Init(AKratos* _Me)
{
	MeshComp->SetWorldScale3D(FVector(0));
	SetTargetScale(false);
	TargetScale = 0.0f;
	CurrentScale = MeshComp->GetComponentScale().X;
	Me = _Me;
}

void ASG_Shield::ActiveHitCollision(bool ActiveState)
{
	if (ActiveState)
	{
		MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("HitableWeapon"), true);
	}
	else
	{
		MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleWeapon"), true);
	}
}


void ASG_Shield::SetTargetScale(const bool Bigger)
{
	TargetScale = Bigger ? MAX_SCALE * 0.7f : 0.03f;
	bBigger = Bigger;
}

TObjectPtr<USoundCue> ASG_Shield::GetBaseHitSound() const
{
	return BaseHitSoundCue;
}

void ASG_Shield::LerpScale(float DeltaTime)
{
	CurrentScale = FMath::Lerp(MeshComp->GetComponentScale().X, TargetScale, DeltaTime * 20);
	MeshComp->SetWorldScale3D(FVector(CurrentScale));

	float RotationSpeedDegreesPerSecond = 360.0f; 
	FQuat DeltaRotation = FQuat(FRotator(RotationSpeedDegreesPerSecond * DeltaTime, 0, 0));
	MeshComp->AddRelativeRotation(DeltaRotation); 

	if (abs(CurrentScale - TargetScale) < 0.001f)
	{
		if (TargetScale != MAX_SCALE && bBigger)
		{
			{
				TargetScale = MAX_SCALE;
			}
		}
		else if (TargetScale != 0.0f && !bBigger)
		{
			{
				TargetScale = 0.0f;
			}
		}
	}
}


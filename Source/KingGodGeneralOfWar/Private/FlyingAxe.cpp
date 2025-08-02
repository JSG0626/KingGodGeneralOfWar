// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingAxe.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kratos.h"
#include "Axe.h"
#include "Kismet/KismetMathLibrary.h"
#include "CSW/AwakenThor.h"
#include "CSW/AwakenThorFSM.h"
#include "BDThor.h"
#include "BDThorFSM.h"
#include "Kismet/GameplayStatics.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "BaseEnemy.h"

//const float AFlyingAxe::FLYING_MOVE_SPEED = 4000.0f;
//const float AFlyingAxe::FLYING_ROTATION_SPEED = 40.0f;
//const float AFlyingAxe::RISING_LERP_SPEED = 6.0f;
//const float AFlyingAxe::RETURNING_INTERP_SPEED = 10.0f;
//const float AFlyingAxe::CATCH_DISTANCE_THRESHOLD = 10.0f;

// Sets default values
AFlyingAxe::AFlyingAxe()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	SubMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubMeshComp"));
	SubMeshComp->SetupAttachment(CapsuleComp);
}

// Called when the game starts or when spawned
void AFlyingAxe::BeginPlay()
{
	Super::BeginPlay();
	Me = Cast<AKratos>(GetWorld()->GetFirstPlayerController()->GetPawn());

	CurrentVelocity = GetActorForwardVector() * FLYING_MOVE_SPEED;
	PrevLocation = GetActorLocation();

}

// Called every frame
void AFlyingAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Me) return;
	UE_LOG(LogTemp, Display, TEXT("CurrentAxeState: %s"), *UEnum::GetValueAsString(CurrentState));
	// 현재 상태에 따라 적절한 함수를 호출
	switch (CurrentState)
	{
	case EAxeState::Flying:
		TickState_Flying(DeltaTime);
		break;
	case EAxeState::Rising:
		TickState_Rising(DeltaTime);
		break;
	case EAxeState::Returning:
		TickState_Returning(DeltaTime);
		break;
	default:
		break;
	}

	
}

void AFlyingAxe::FlyingAxeOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
	//if (Enemy)
	//{
	//	DealDamage(Enemy, FGenericAttackParams(Me, BaseAttackPower * CurrentAttackScale, CurrentStunAttackScale, EAttackDirectionType::UP));
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
	//}
	//if (CurrentState == EAxeState::Flying)
	//{
	//	ActiveHitCollision(false);
	//	AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);
	//	SubMeshComp->SetRelativeRotation(HitArrowComp->GetRelativeRotation());
	//	CurrentState = EAxeState::Stuck;
	//}
}

void AFlyingAxe::TickState_Flying(float DeltaTime)
{
	FlyingTime += DeltaTime;

	if (FlyingTime >= GravityTime)
	{
		const float GravityZ = GetWorld()->GetGravityZ();
		const FVector GravityAcceleration = FVector(0.0f, 0.0f, GravityZ);
		CurrentVelocity += GravityAcceleration * DeltaTime;
	}
	SetActorLocation(GetActorLocation() + CurrentVelocity * DeltaTime);

	FVector Axis = GetActorRightVector();
	const FQuat RotationDelta(Axis, FLYING_ROTATION_SPEED * DeltaTime);
	AddActorWorldRotation(RotationDelta);

	// 충돌 처리
	bool bHit = CollisionCheck();
}

// 회수 시 위로 떠오를 때의 로직
void AFlyingAxe::TickState_Rising(float DeltaTime)
{
	// 임시로 라이징 스테이트를 비활성화
	CurrentState = EAxeState::Returning;
	return;

	//// Lerp를 이용해 목표 위치로 이동
	//const FVector CurLocation = GetActorLocation();
	//FVector Direction = (TargetLocation - CurLocation).GetSafeNormal();
	//const FVector NextLocation = CurLocation + RisingSpeed * Direction * DeltaTime;
	//RisingSpeed += RisingSpeedDelta;
	//SetActorLocation(NextLocation);

	//// 회전 로직
	//FVector Axis = SubMeshComp->GetRightVector();
	//const FQuat RotationDelta(Axis, RotationSpeed * DeltaTime);
	//RotationSpeed += DeltaTime * 60;
	//SubMeshComp->AddWorldRotation(RotationDelta);

	//// 목표 지점에 도달하면 Returning 상태로 전환
	//if (FVector::DistSquared(GetActorLocation(), TargetLocation) < FMath::Square(CATCH_DISTANCE_THRESHOLD))
	//{
	//	ReturningSpeed = GetVelocity().Size();
	//	PrevLocation = GetActorLocation();
	//	CurrentState = EAxeState::Returning;
	//}

	//// 충돌 처리
	//bool bHit = CollisionCheck();
}

// 플레이어에게 돌아올 때의 로직
void AFlyingAxe::TickState_Returning(float DeltaTime)
{
	// 매 프레임 플레이어의 위치를 다시 타겟으로 설정하여 따라가게 함
	TargetLocation = Me->WithdrawPositionComp->GetComponentLocation();

	// VInterpTo를 사용해 부드럽게 타겟을 추적
	//const FVector NextLocation = FMath::Lerp(PrevLocation, TargetLocation, ReturningInterpAlpha);
	const FVector CurLocation = GetActorLocation();
	FVector Direction = (TargetLocation - CurLocation).GetSafeNormal();
	float RandScale = 0.2f;
	Direction += FVector(FMath::RandRange(-RandScale, RandScale), FMath::RandRange(-RandScale, RandScale), FMath::RandRange(-RandScale, RandScale));
	Direction.Normalize();
	const FVector NextLocation = CurLocation + ReturningSpeed * Direction * DeltaTime;
	ReturningSpeed += ReturningSpeedDelta;
	//UE_LOG(LogTemp, Display, TEXT("ReturningInterpAlpha: %f"), ReturningInterpAlpha);
	ReturningInterpAlpha += ReturningAlphaDelta;
	ReturningAlphaDelta += 0.0001;
	if (ReturningInterpAlpha > 1.0f)
	{
		//ReturningInterpAlpha = 1 - ReturningAlphaDelta - 0.001;
		ReturningInterpAlpha = 0.0f;
		PrevLocation = GetActorLocation();
	}
	SetActorLocation(NextLocation);

	// 회전 로직
	FVector Axis = GetActorRightVector();
	RandScale = 0.2f;
	Axis += FVector(FMath::RandRange(-RandScale, RandScale));
	const FQuat RotationDelta(Axis, RotationSpeed * DeltaTime);
	//RotationSpeed += DeltaTime * 30;
	AddActorWorldRotation(RotationDelta);


	// 플레이어에게 충분히 가까워지면 회수 처리
	if (FVector::DistSquared(GetActorLocation(), TargetLocation) < FMath::Square(CATCH_DISTANCE_THRESHOLD))
	{
		HandleCatch();
	}

	// 충돌 처리
	bool bHit = CollisionCheck();
}

bool AFlyingAxe::CollisionCheck()
{
	FHitResult HitResult;
	const FVector CurLocation = GetActorLocation();
	FCollisionQueryParams Params;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, PrevLocation, CurLocation, ECC_Visibility);
	PrevLocation = CurLocation;
	if (bHit)
	{
		ABaseEnemy* Enemy = Cast<ABaseEnemy>(HitResult.GetActor());
		if (Enemy)
		{
			DealDamage(Enemy, FGenericAttackParams(Me, BaseAttackPower * CurrentAttackScale, CurrentStunAttackScale, EAttackDirectionType::UP));
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
		}
		if (CurrentState == EAxeState::Flying)
		{
			AttachToComponent(HitResult.GetComponent(), FAttachmentTransformRules::KeepWorldTransform);
			
			{
				FVector Y = GetActorRightVector();
				FVector Z = Y.FVector::Cross(-HitResult.ImpactNormal);
				FRotator Rot = FRotationMatrix::MakeFromYZ(Y, Z).Rotator();
				
				SetActorRotation(Rot);
				AddActorLocalRotation(FRotator(0, 180, 180));
			}
			CurrentState = EAxeState::Stuck;
		}

	}
	return bHit;
}

void AFlyingAxe::HandleCatch()
{
	if (Me)
	{
		Me->CatchFlyingAxe();
	}
	Destroy();
}

void AFlyingAxe::BackToPlayer()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("BackToPlayer")));
	int randIdx = FMath::RandRange(0, 2);
	//TargetLocation = WithdrawTargetPositionArr[randIdx]->GetComponentLocation();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorEnableCollision(true);

	if (CurrentState == EAxeState::Stuck)
	{
		CurrentState = EAxeState::Rising;
	}
	else
	{
		PrevLocation = GetActorLocation();
		ReturningAlphaDelta /= 2;
		CurrentState = EAxeState::Returning;
	}
}

void AFlyingAxe::ActiveHitCollision(bool Active)
{
	//SetActorEnableCollision(Active);
}

TObjectPtr<USoundCue> AFlyingAxe::GetBaseHitSound() const
{
	return BaseHitSoundCue;
}
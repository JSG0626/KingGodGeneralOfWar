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
#include "Components/LightComponent.h"
#include <Components/PointLightComponent.h>

// Sets default values
AFlyingAxe::AFlyingAxe()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	LightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComp"));
	LightComp->SetupAttachment(CapsuleComp);
	SubMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubMeshComp"));
	SubMeshComp->SetupAttachment(CapsuleComp);
}

// Called when the game starts or when spawned
void AFlyingAxe::BeginPlay()
{
	Super::BeginPlay();
}

void AFlyingAxe::Init(AKratos* _Me, bool _bIsHeavy)
{
	Kratos = _Me;
	bIsHeavy = _bIsHeavy;
	UE_LOG(LogTemp, Display, TEXT("bIsHeavy: %d"), bIsHeavy);
	if (!bIsHeavy)
	{
		AddActorLocalRotation(FRotator(0, 0, 70));
	}
	IWeaponInterface::BaseAttackPower = Kratos->GetAttackPower(EPlayerWeaponType::Axe);
	IWeaponInterface::CurrentAttackScale = bIsHeavy ? HeavyScale : DefaultScale;
	IWeaponInterface::CurrentStunAttackScale = StunDamage;

	AxeMeshOffset = SubMeshComp->GetRelativeLocation();
	SubMeshComp->SetRelativeLocation(FVector(0));
	CurrentVelocity = GetActorForwardVector() * (bIsHeavy ? HeavyThrowingMoveSpeed : DefaultThrowingMoveSpeed);
	PrevLocation = GetActorLocation();
	StartInterpRotationDistSquared = StartInterpRotationDist * StartInterpRotationDist;
}
// Called every frame
void AFlyingAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Kratos) return;

	// 현재 상태에 따라 적절한 함수를 호출
	switch (CurrentState)
	{
	case EAxeState::Flying:
		TickState_Flying(DeltaTime);
		break;
	case EAxeState::Bounce:
		TickState_Bounce(DeltaTime);
		break;
	case EAxeState::Vibration:
		TickState_Vibration(DeltaTime);
		break;
	case EAxeState::Returning:
		TickState_Returning(DeltaTime);
		break;
	default:
		break;
	}
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
	const FQuat RotationDelta(Axis, ThrowingRotationSpeed * DeltaTime);
	AddActorWorldRotation(RotationDelta);

	// 충돌 처리
	FHitResult HitResult;
	bool bHit = CollisionCheck(HitResult);
	if (bHit)
	{
		if (bIsHeavy || Cast<ABaseEnemy>(HitResult.GetActor()) == nullptr)
		{
			SetState(EAxeState::Stuck, HitResult);
		}
		else
		{
			SetState(EAxeState::Bounce, HitResult);
		}
	}
}

// 충돌시 시 튕겨나갈 때의 로직
void AFlyingAxe::TickState_Bounce(float DeltaTime)
{
	const float GravityZ = GetWorld()->GetGravityZ() * BounceGravityScale;
	const FVector GravityAcceleration = FVector(0.0f, 0.0f, GravityZ);
	CurrentVelocity += GravityAcceleration * DeltaTime;

	SetActorLocation(GetActorLocation() + CurrentVelocity * DeltaTime);
	//SetActorRotation()

	FVector Axis = GetActorRightVector();
	const FQuat RotationDelta(Axis, BouncingRotationInitSpeed * DeltaTime);
	AddActorWorldRotation(RotationDelta);

	BouncingRotationInitSpeed += BouncingRotationIncrementAlpha * DeltaTime;
	// 충돌 처리
	FHitResult HitResult;
	bool bHit = CollisionCheck(HitResult);
	if (bHit)
	{
		if (StuckEnemy != HitResult.GetActor())
		{
			SetState(EAxeState::Stuck, HitResult);
		}
	}
}

void AFlyingAxe::TickState_Vibration(float DeltaTime)
{
	VibrationElapsedTime += DeltaTime;

	if (VibrationElapsedTime >= VibrationTime)
	{
		SetState(EAxeState::Returning);
	}

	FVector VibrationLocationDelta;
	VibrationLocationDelta.X = FMath::RandRange(-VibrationRandomRange, VibrationRandomRange);
	VibrationLocationDelta.Y = FMath::RandRange(-VibrationRandomRange, VibrationRandomRange);
	VibrationLocationDelta.Z = FMath::RandRange(-VibrationRandomRange, VibrationRandomRange);
	VibrationLocationDelta += -GetActorForwardVector() * 25;
	SetActorLocation(PrevLocation + VibrationLocationDelta);

	//const FVector& Axis = VibrationMoveDirection;

	//const FQuat RotationDelta(Axis, FMath::RandRange(-7.5f, 2.5f) * DeltaTime);
	//AddActorWorldRotation(RotationDelta);
}

// 플레이어에게 돌아올 때의 로직
void AFlyingAxe::TickState_Returning(float DeltaTime)
{
	ReturnElapsedTime += DeltaTime;

	const FVector CurLocation = GetActorLocation();
	TargetLocation = Kratos->WithdrawPositionComp->GetComponentLocation();
	const float LocationAlpha = FMath::Min(1.0f, ReturnElapsedTime / ReturnDuration);
	// 이동 로직
	{
		FVector NewLocation = FMath::Lerp(ReturnStartLocation, TargetLocation, LocationAlpha);
		FQuat TargetRotation = Kratos->GetActorQuat();
		const FVector PathCurveOffset = TargetRotation.GetRightVector() * FMath::Sin(UE_PI * LocationAlpha) * PathCurveRadius;
		SetActorLocation(NewLocation + PathCurveOffset);
	}

	// 회전 로직
	const float DistanceSquaredToTarget = FVector::DistSquared(CurLocation, TargetLocation);

	const float RotationAlpha = FMath::Min(1.0f, SlerpElapsedTime / InterpRotationDuration);
	//if (DistanceSquaredToTarget <= StartInterpRotationDistSquared)
	if (LocationAlpha >= .7f && DistanceSquaredToTarget <= StartInterpRotationDistSquared)
	{
		const FQuat NewRotation = FQuat::Slerp(GetActorQuat(), Kratos->WithdrawPositionComp->GetComponentQuat(), RotationAlpha);
		SetActorRotation(NewRotation);
		SlerpElapsedTime += DeltaTime;
	}
	else
	{
		FVector RightAxis = GetActorRightVector();
		FVector ForwardAxis = GetActorForwardVector();
		//float RandScale = 0.6f;
		//Axis += FVector(FMath::RandRange(-RandScale, RandScale));
		FQuat RotationDelta(RightAxis, ReturnRotationSpeed * DeltaTime);
		AddActorWorldRotation(RotationDelta);

		RotationDelta = FQuat(ForwardAxis, ReturnRotationSpeed * DeltaTime);
		AddActorWorldRotation(RotationDelta);
	}


	// 플레이어에게 충분히 가까워지면 회수 처리
	if (FVector::DistSquared(GetActorLocation(), TargetLocation) < FMath::Square(CatchDistanceThreshold))
	{
		HandleCatch();
	}

	// 충돌 처리
	FHitResult HitResult;
	bool bHit = CollisionCheck(HitResult);
}
void AFlyingAxe::SetState(const EAxeState NewState, const FHitResult& HitResult)
{
	DamagedActors.Empty();
	// Exit State
	switch (CurrentState)
	{
	case EAxeState::Idle:
		break;
	case EAxeState::Flying:
		break;
	case EAxeState::Bounce:
		break;
	case EAxeState::Stuck:
		break;
	case EAxeState::Vibration:
		break;
	case EAxeState::Returning:
		break;
	default:
		break;
	}

	CurrentState = NewState;

	// Enter State
	switch (CurrentState)
	{
	case EAxeState::Idle:
		break;
	case EAxeState::Flying:
		break;
	case EAxeState::Bounce:
		OnEnterBounce(HitResult);
		break;
	case EAxeState::Stuck:
		OnEnterStuck(HitResult);
		break;
	case EAxeState::Vibration:
		OnEnterVibration();
		break;
	case EAxeState::Returning:
		OnEnterReturning();
		break;
	default:
		break;
	}
}

void AFlyingAxe::OnEnterStuck(const FHitResult& HitResult)
{
	StuckEnemy = HitResult.GetActor();
	AttachToComponent(HitResult.GetComponent(), FAttachmentTransformRules::KeepWorldTransform);
	FVector Y = GetActorRightVector();
	FVector Z = Y.FVector::Cross(-HitResult.ImpactNormal);
	Z += (HitResult.ImpactNormal) / FMath::RandRange(1, 3);
	Z.Normalize();
	FRotator Rot = FRotationMatrix::MakeFromYZ(Y, Z).Rotator();

	SubMeshComp->SetRelativeLocation(AxeMeshOffset);
	SetActorLocation(HitResult.Location);
	SetActorRotation(Rot);
	AddActorLocalRotation(FRotator(0, 180, 180));
}
void AFlyingAxe::OnEnterBounce(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Display, TEXT("OnEnterBounce"));
	StuckEnemy = HitResult.GetActor();
	const FVector N = HitResult.ImpactNormal;
	const FVector D = CurrentVelocity.GetSafeNormal();
	const FVector ProjD = N.Dot(-D) * N;
	FVector NewDirection = ProjD + (D + ProjD);
	//NewDirection.Z *= NewDirection.Z > 0 ? BounceUpScale : -BounceUpScale;
	NewDirection.Z = FMath::Max(0, NewDirection.Z) + 2.0f;
	NewDirection.Normalize();
	CurrentVelocity = NewDirection * BounceSpeed;

	FRotator CurRotation = GetActorRotation();
	CurRotation.Roll = 0;
	SetActorRotation(CurRotation);
	/*DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10, FColor::Blue, false, 10.0f);
	DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.ImpactPoint + NewDirection * 1000, FColor::Red, false, 5.0f);
	DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.ImpactPoint + N * 1000, FColor::Green, false, 5.0f);*/
}
void AFlyingAxe::OnEnterVibration()
{
	LightComp->SetVisibility(true);
	VibrationMoveDirection = GetActorRightVector();
}
void AFlyingAxe::OnEnterReturning()
{
	LightComp->SetVisibility(false);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ActiveHitCollision(true);

	PrevLocation = GetActorLocation();
	ReturnStartLocation = PrevLocation;
	TargetLocation = Kratos->WithdrawPositionComp->GetComponentLocation();
	const float DistanceToTarget = FVector::Dist(PrevLocation, TargetLocation);
	ReturnDuration = FMath::Min(DistanceToTarget / MinReturnSpeed, MaxReturnDuration);
	PathCurveRadius = DistanceToTarget * RadiusScale;
}
bool AFlyingAxe::CollisionCheck(FHitResult& HitResult)
{
	const FVector CurLocation = GetActorLocation();
	FCollisionQueryParams Params;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, PrevLocation, CurLocation, ECC_Visibility);
	PrevLocation = CurLocation;
	if (bHit)
	{
		ABaseEnemy* Enemy = Cast<ABaseEnemy>(HitResult.GetActor());

		if (Enemy && StuckEnemy != Enemy && DamagedActors.Find(Enemy) == INDEX_NONE)
		{
			DamagedActors.Add(Enemy);
			DealDamage(Enemy, FGenericAttackParams(Kratos, BaseAttackPower * CurrentAttackScale, CurrentStunAttackScale, EAttackDirectionType::UP));
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
		}

	}
	return bHit;
}

void AFlyingAxe::HandleCatch()
{
	if (Kratos)
	{
		Kratos->CatchFlyingAxe();
	}
	Destroy();
}

void AFlyingAxe::BackToPlayer()
{
	if (CurrentState == EAxeState::Stuck)
	{
		SetState(EAxeState::Vibration);
	}
	else
	{
		SetState(EAxeState::Returning);
	}
}

void AFlyingAxe::ActiveHitCollision(bool Active)
{
	SetActorEnableCollision(Active);
}

TObjectPtr<USoundCue> AFlyingAxe::GetBaseHitSound() const
{
	return BaseHitSoundCue;
}


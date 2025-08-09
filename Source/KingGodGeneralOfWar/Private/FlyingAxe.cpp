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
#include "Engine/SkeletalMeshSocket.h"
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

	BladeLocationComp = CreateDefaultSubobject<UArrowComponent>(TEXT("BladeLocationComp"));
	BladeLocationComp->SetupAttachment(CapsuleComp);

	LerpInitRotationComp = CreateDefaultSubobject<UArrowComponent>(TEXT("LerpInitRotationComp"));
	LerpInitRotationComp->SetupAttachment(CapsuleComp);
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
	PrevBladeLocation = BladeLocationComp->GetComponentLocation();
	StartInterpRotationDistSquared = StartInterpRotationDist * StartInterpRotationDist;
}
void AFlyingAxe::Init(AKratos* _Me, const FRotator LocalRotationOffset, const bool _ApplyGravity, bool bOrbital, const float _OrbitalDuration, const int _OrbitalCount, const float _OrbitalDirection)
{
	Kratos = _Me;
	AddActorLocalRotation(LocalRotationOffset);
	IWeaponInterface::BaseAttackPower = Kratos->GetAttackPower(EPlayerWeaponType::Axe);
	IWeaponInterface::CurrentAttackScale = DefaultScale;
	IWeaponInterface::CurrentStunAttackScale = StunDamage;

	//AxeMeshOffset = SubMeshComp->GetRelativeLocation();
	SubMeshComp->SetRelativeLocation(FVector(0));
	CurrentVelocity = GetActorForwardVector() * HeavyThrowingMoveSpeed;
	PrevLocation = GetActorLocation();
	PrevBladeLocation = BladeLocationComp->GetComponentLocation();
	StartInterpRotationDistSquared = StartInterpRotationDist * StartInterpRotationDist;
	bPass = true;
	ApplyGravity = _ApplyGravity;
	OrbitalDuration = _OrbitalDuration;
	OrbitalCount = _OrbitalCount;
	OrbitalDirection = _OrbitalDirection;
	SetState(bOrbital ? EAxeState::Orbital : EAxeState::Flying);
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
	case EAxeState::Orbital:
		TickState_Orbital(DeltaTime);
		break;
	default:
		PrevLocation = GetActorLocation();
		break;
	}
	//

}
void AFlyingAxe::TickState_Flying(float DeltaTime)
{
	StateElapsedTime.Flying += DeltaTime;

	if (ApplyGravity && StateElapsedTime.Flying >= GravityTime)
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
		if (bPass) return;
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
	StateElapsedTime.Bounce += DeltaTime;
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

	if (StateElapsedTime.Bounce >= 0.2f)
	{
		bool bHit = CollisionCheck(HitResult);
		if (bHit)
		{
			if (StuckActor != HitResult.GetActor())
			{
				SetState(EAxeState::Stuck, HitResult);
			}
		}
	}
	else
	{
		PrevLocation = GetActorLocation();
	}


}

void AFlyingAxe::TickState_Vibration(float DeltaTime)
{
	StateElapsedTime.Vibration += DeltaTime;
	if (StateElapsedTime.Vibration >= VibrationTime)
	{
		SetState(EAxeState::Returning);
	}

	FVector VibrationLocationDelta = -GetActorForwardVector() * 12;
	SetActorLocation(PrevLocation + VibrationLocationDelta);

	if ((VibrationTickCount & 1) == 0)
	{
		FVector Axis = GetActorForwardVector();
		FQuat RotationDelta(Axis, VibrationRoll);
		VibrationRoll = -(VibrationRoll + (VibrationRoll >= 0 ? VibrationRollIncrement : -VibrationRollIncrement));
		AddActorWorldRotation(RotationDelta);

		Axis = GetActorRightVector();
		RotationDelta = FQuat(Axis, VibrationYawDelta);
		AddActorWorldRotation(RotationDelta);
	}

	++VibrationTickCount;
}

// 플레이어에게 돌아올 때의 로직
void AFlyingAxe::TickState_Returning(float DeltaTime)
{
	StateElapsedTime.Return += DeltaTime;

	const FVector CurLocation = GetActorLocation();
	TargetLocation = TargetSocketTransform->GetComponentLocation();
	const float LocationAlpha = FMath::Min(1.0f, StateElapsedTime.Return / ReturnDuration);
	// 이동 로직
	{
		FVector NewLocation = FMath::Lerp(ReturnStartLocation, TargetLocation, LocationAlpha);
		FQuat TargetRotation = Kratos->GetActorQuat();
		FVector VerticalPathCurveOffset = TargetRotation.GetUpVector() * FMath::Cos(UE_PI * (LocationAlpha + 0.5f)) * PathCurveRadius * 0.25f;
		VerticalPathCurveOffset.X = 0;
		VerticalPathCurveOffset.Y = 0;
		FVector HorizontalPathCurveOffset = TargetRotation.GetRightVector() * FMath::Sin(UE_PI * LocationAlpha) * PathCurveRadius * 2.0f;
		HorizontalPathCurveOffset.Z = 0;
		SetActorLocation(NewLocation + (VerticalPathCurveOffset + HorizontalPathCurveOffset) * 0.5f);
	}

	// 회전 로직
	const float DistanceSquaredToTarget = FVector::DistSquared(CurLocation, TargetLocation);

	if (DistanceSquaredToTarget <= StartInterpRotationDistSquared)
	{
		const float RotationAlpha = FMath::Min(1.0f, StateElapsedTime.SlerpHandRotation / InterpRotationDuration);
		const FQuat NewRotation = FQuat::Slerp(GetActorQuat(), TargetSocketTransform->GetComponentQuat(), RotationAlpha);
		SetActorRotation(NewRotation);
		StateElapsedTime.SlerpHandRotation += DeltaTime;
	}
	else
	{
		if (StateElapsedTime.Return <= LerpInitReturnRotationTime)
		{
			const float Alpha = StateElapsedTime.Return / LerpInitReturnRotationTime;
			const FQuat NewRotation = FQuat::Slerp(GetActorQuat(), TargetLeprInitQuat, Alpha);
			SetActorRotation(NewRotation);
		}
		else
		{
			const FVector& RightAxis = GetActorRightVector();
			const FQuat RotationDelta(RightAxis, ReturnRotationSpeed * DeltaTime);
			AddActorWorldRotation(RotationDelta);
		}
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
void AFlyingAxe::TickState_Orbital(float DeltaTime)
{
	StateElapsedTime.Orbital += DeltaTime;
	PathCurveRadius += 100.0f * DeltaTime;
	const float Alpha = StateElapsedTime.Orbital / OrbitalDuration * 2.0f * OrbitalCount * OrbitalDirection;
	float CurrentMultiple = FMath::FloorToFloat(Alpha / 0.25f);
	UE_LOG(LogTemp, Display, TEXT("CurrentMultiple: %f"), CurrentMultiple);
	if (CurrentMultiple != LastMultiple)
	{
		DamagedActors.Empty();
		LastMultiple = CurrentMultiple;
	}
	const FVector X = Kratos->GetActorForwardVector() * FMath::Cos(UE_PI * Alpha) * PathCurveRadius;
	const FVector Y = Kratos->GetActorRightVector() * FMath::Sin(UE_PI * Alpha) * PathCurveRadius;
	SetActorLocation(Kratos->GetActorLocation() + X + Y);

	FVector Axis = GetActorRightVector();
	const FQuat RotationDelta(Axis, OrbitalRotationSpeed * DeltaTime);
	AddActorWorldRotation(RotationDelta);

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
	case EAxeState::Orbital:
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
	case EAxeState::Orbital:
		OnEnterOrbital();
		break;
	default:
		break;
	}
}

void AFlyingAxe::OnEnterStuck(const FHitResult& HitResult)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), StuckSound, GetActorLocation());
	StuckActor = HitResult.GetActor();
	FVector Y = GetActorRightVector();
	FVector Z = Y.FVector::Cross(-HitResult.ImpactNormal);
	Z += (HitResult.ImpactNormal) / FMath::RandRange(1, 3);
	Z.Normalize();
	SetActorLocation(HitResult.Location);
	SubMeshComp->SetRelativeLocation(AxeMeshOffset);

	FRotator Rot = FRotationMatrix::MakeFromYZ(Y, Z).Rotator();
	SetActorRotation(Rot);
	AddActorLocalRotation(FRotator(0, 180, 180));

	if (HitResult.GetActor() == nullptr) return;
	if (TObjectPtr<USkeletalMeshComponent> StuckSkeletalMeshComp = 
		Cast< USkeletalMeshComponent>(HitResult.GetActor()->GetComponentByClass(USkeletalMeshComponent::StaticClass())))
	{
		UE_LOG(LogTemp, Display, TEXT("it is Character"));
		USkinnedAsset* SkinnedAsset = StuckSkeletalMeshComp->GetSkinnedAsset();
		if (SkinnedAsset == nullptr) return;

		TArray <USkeletalMeshSocket*> Sockets = SkinnedAsset->GetActiveSocketList();
		double MinDistSquared = DOUBLE_BIG_NUMBER;
		const FVector CurLocation = GetActorLocation();
		USkeletalMeshSocket* TargetSocket = nullptr;
		for (USkeletalMeshSocket* Socket : Sockets)
		{
			UE_LOG(LogTemp, Display, TEXT("Socket: %s"), *Socket->GetName());
			const float DistSqured = FVector::DistSquared(Socket->GetSocketLocation(StuckSkeletalMeshComp), CurLocation);
			if (DistSqured <= MinDistSquared)
			{
				MinDistSquared = DistSqured;
				TargetSocket = Socket;
			}
		}

		if (TargetSocket)
		{
			bool bAttach = AttachToComponent(StuckSkeletalMeshComp, FAttachmentTransformRules::KeepWorldTransform, TargetSocket->GetFName());
			UE_LOG(LogTemp, Display, TEXT("TargetSocket: %s, Attach: %d"), *TargetSocket->GetFName().ToString(), bAttach);
		}
	}
}
void AFlyingAxe::OnEnterBounce(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Display, TEXT("OnEnterBounce"));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), BounceSound, GetActorLocation());

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
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReturnSpinningSound,	GetActorLocation());
	LightComp->SetVisibility(false);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ActiveHitCollision(true);

	// TargetQuat 초기값 계산
	{
		const FVector AxeLocation = GetActorLocation();
		const FVector TargetHandLocation = TargetSocketTransform->GetComponentLocation();

		const FVector DirectionToPlayer = (TargetHandLocation - AxeLocation).GetSafeNormal();

		const FRotator BaseRotation = UKismetMathLibrary::MakeRotFromX(-DirectionToPlayer);

		const FQuat TiltOffset = LerpInitRotationComp->GetRelativeRotation().Quaternion();

		TargetLeprInitQuat = BaseRotation.Quaternion() * TiltOffset;
	}

	SubMeshComp->SetRelativeLocation(FVector::ZeroVector);
	PrevLocation = GetActorLocation();
	ReturnStartLocation = PrevLocation;
	TargetLocation = TargetSocketTransform->GetComponentLocation();
	const float DistanceToTarget = FVector::Dist(PrevLocation, TargetLocation);
	ReturnDuration = FMath::Max(MinReturnDuration, FMath::Min(DistanceToTarget / MinReturnSpeed, MaxReturnDuration));
	PathCurveRadius = DistanceToTarget * RadiusScale;

	if (TObjectPtr<ABaseEnemy> Enemy = Cast<ABaseEnemy>(StuckActor))
	{
		DealDamage(Enemy, FGenericAttackParams(Kratos, BaseAttackPower * CurrentAttackScale, CurrentStunAttackScale, EAttackDirectionType::UP));
	}
}
void AFlyingAxe::OnEnterOrbital()
{
	PathCurveRadius = 150.0f;
}
bool AFlyingAxe::CollisionCheck(FHitResult& HitResult)
{
	const FVector CurLocation = GetActorLocation();
	FCollisionQueryParams Params;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, PrevLocation, CurLocation, ECC_Visibility);
	if (bHit)
	{
		ABaseEnemy* Enemy = Cast<ABaseEnemy>(HitResult.GetActor());

		if (Enemy && StuckActor != Enemy && DamagedActors.Find(Enemy) == INDEX_NONE)
		{
			DamagedActors.Add(Enemy);
			DealDamage(Enemy, FGenericAttackParams(Kratos, BaseAttackPower * CurrentAttackScale, CurrentStunAttackScale, EAttackDirectionType::UP));
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
		}
	}

	FHitResult DecalHitResult;
	FVector CurBladeLocation = BladeLocationComp->GetComponentLocation();
	FVector HalfPoint = PrevBladeLocation + (CurBladeLocation - PrevBladeLocation) * 0.5f;
	bool bBladeHit = GetWorld()->LineTraceSingleByChannel(DecalHitResult, PrevBladeLocation, HalfPoint, ECC_Visibility);
	if (bBladeHit)
	{
		UE_LOG(LogTemp, Display, TEXT("bBladeHit"));
		FRotator DecalRotation = GetActorRotation();
		UGameplayStatics::SpawnDecalAttached(ImpactDecalMaterial, DecalSize, DecalHitResult.GetComponent(),
			TEXT("Slash"), DecalHitResult.ImpactPoint + DecalHitResult.ImpactNormal * 5, DecalRotation, EAttachLocation::KeepWorldPosition, DecalLifeSpan);
	}
	else if (GetWorld()->LineTraceSingleByChannel(DecalHitResult, HalfPoint, CurLocation, ECC_Visibility))
	{
		UE_LOG(LogTemp, Display, TEXT("bBladeHit"));
		FRotator DecalRotation = GetActorRotation();
		UGameplayStatics::SpawnDecalAttached(ImpactDecalMaterial, DecalSize, DecalHitResult.GetComponent(),
			TEXT("Slash"), DecalHitResult.ImpactPoint + DecalHitResult.ImpactNormal * 5, DecalRotation, EAttachLocation::KeepWorldPosition, DecalLifeSpan);
	}
	PrevLocation = CurLocation;
	PrevBladeLocation = CurBladeLocation;
	return bHit;
}

void AFlyingAxe::HandleCatch()
{
	if (Kratos)
	{

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CatchAxeSound, GetActorLocation());
		Kratos->CatchFlyingAxe();
	}
	Destroy();
}

void AFlyingAxe::BackToPlayer(bool bImmediateReturn)
{
	TargetSocketTransform = Kratos->RightHandTransformComp;
	if (CurrentState == EAxeState::Stuck && bImmediateReturn == false)
	{
		SetState(EAxeState::Vibration);
	}
	else
	{
		SetState(EAxeState::Returning);
	}
}

void AFlyingAxe::BackToPlayer(const float _MaxReturnDuration, const float _MinReturnDuration, const bool bImmediateReturn, const float _RadiusScale, const bool bRightHand)
{
	MaxReturnDuration = _MaxReturnDuration;
	MinReturnDuration = _MinReturnDuration;
	RadiusScale = _RadiusScale;
	BackToPlayer(bImmediateReturn);
	TargetSocketTransform = bRightHand ? Kratos->RightHandTransformComp : Kratos->LeftHandTransformComp;
	UE_LOG(LogTemp, Display, TEXT("TargetSocketComp: %s"), *TargetSocketTransform->GetName());
}

void AFlyingAxe::ActiveHitCollision(bool Active)
{
	SetActorEnableCollision(Active);
}

TObjectPtr<USoundCue> AFlyingAxe::GetBaseHitSound() const
{
	return BaseHitSound;
}


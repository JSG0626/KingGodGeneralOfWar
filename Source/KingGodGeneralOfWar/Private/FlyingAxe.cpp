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

const float AFlyingAxe::FLYING_MOVE_SPEED = 4000.0f;
const float AFlyingAxe::FLYING_ROTATION_SPEED = 40.0f;
const float AFlyingAxe::RISING_LERP_SPEED = 6.0f;
const float AFlyingAxe::RETURNING_INTERP_SPEED = 10.0f;
const float AFlyingAxe::CATCH_DISTANCE_THRESHOLD = 50.0f;

// Sets default values
AFlyingAxe::AFlyingAxe()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	SubMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubMeshComp"));
	SubMeshComp->SetupAttachment(CapsuleComp);

	DirectionArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrowComp"));
	DirectionArrowComp->SetupAttachment(CapsuleComp);

	HitArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("HitArrowComp"));
	HitArrowComp->SetupAttachment(CapsuleComp);
	HitArrowComp->SetRelativeRotation(FRotator(40, 180, 0));

	WithdrawTargetPosition1 = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawPosition1"));
	WithdrawTargetPosition1->SetupAttachment(CapsuleComp);
	WithdrawTargetPosition1->SetRelativeLocation(FVector(-400, 0, 330));

	WithdrawTargetPosition2 = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawPosition2"));
	WithdrawTargetPosition2->SetupAttachment(CapsuleComp);
	//WithdrawTargetPosition2->SetRelativeLocation(FVector());

	WithdrawTargetPosition3 = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawPosition3"));
	WithdrawTargetPosition3->SetupAttachment(CapsuleComp);
	//WithdrawTargetPosition3->SetRelativeLocation(FVector());

	WithdrawRotation = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawRotation"));
	WithdrawRotation->SetupAttachment(CapsuleComp);
	WithdrawRotation->SetRelativeRotation(FRotator(-30, 180, 0));
}

// Called when the game starts or when spawned
void AFlyingAxe::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AFlyingAxe::FlyingAxeOnComponentBeginOverlap);
	Me = Cast<AKratos>(GetWorld()->GetFirstPlayerController()->GetPawn());
	WithdrawTargetPositionArr.Add(WithdrawTargetPosition1);
	WithdrawTargetPositionArr.Add(WithdrawTargetPosition2);
	WithdrawTargetPositionArr.Add(WithdrawTargetPosition3);

	CurrentVelocity = GetActorForwardVector() * 6000;
}

// Called every frame
void AFlyingAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Me) return;

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
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
	if (Enemy)
	{
		DealDamage(Enemy, FGenericAttackParams(Me, BaseAttackPower * CurrentAttackScale, CurrentStunAttackScale, EAttackDirectionType::UP));
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
	}
	else
	{
		/*auto* Thor = Cast<ABDThor>(OtherActor);
		if (Thor)
		{
			Thor->fsm->Damage(AXE_THROW_DAMAGE, AttackTypeDirectionArr[static_cast<int8>(EAttackType::AXE_THROW_ATTACK)][bWithdrawing]);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
		}
		else
		{
			auto AwakenThor = Cast<AAwakenThor>(OtherActor);
			if (AwakenThor)
			{
				AwakenThor->getFSM()->SetDamage(AXE_THROW_DAMAGE, AttackTypeDirectionArr[static_cast<int8>(EAttackType::AXE_THROW_ATTACK)][bWithdrawing]);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFXFactory, GetActorLocation());
			}
		}*/
	}
	if (CurrentState == EAxeState::Flying)
	{
		ActiveHitCollision(false);
		AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);
		SubMeshComp->SetRelativeRotation(HitArrowComp->GetRelativeRotation());
		CurrentState = EAxeState::Stuck;
	}
}

void AFlyingAxe::TickState_Flying(float DeltaTime)
{
	// 1. 월드에 설정된 중력 값을 가져옵니다.
	const float GravityZ = GetWorld()->GetGravityZ();
	const FVector GravityAcceleration = FVector(0.0f, 0.0f, GravityZ);

	// 2. 중력 가속도를 현재 속도에 더해 포물선 운동을 시뮬레이션합니다.
	CurrentVelocity += GravityAcceleration * DeltaTime;

	// 3. 최종 계산된 속도로 도끼를 이동시킵니다.
	FHitResult HitResult;
	AddActorWorldOffset(CurrentVelocity * DeltaTime, true, &HitResult);

	// 만약 이동 중 어딘가에 부딪혔다면, Stuck 상태로 변경할 수 있습니다.
	if (HitResult.bBlockingHit)
	{
		// OnHit() 같은 충돌 처리 함수 호출
	}

	// 4. 기존 회전 로직은 그대로 유지합니다.
	FVector Axis = SubMeshComp->GetRightVector();
	// Axis += FVector(.005); // 이 부분은 회전 축을 미세하게 변경하여 불안정한 회전을 만듭니다. 의도한 것이 아니라면 제거하는 것이 좋습니다.
	const FQuat RotationDelta(Axis, -FLYING_ROTATION_SPEED * DeltaTime);
	SubMeshComp->AddWorldRotation(RotationDelta);
}

// 회수 시 위로 떠오를 때의 로직
void AFlyingAxe::TickState_Rising(float DeltaTime)
{
	// Lerp를 이용해 목표 위치로 이동
	const FVector CurrentLocation = GetActorLocation();
	const FVector NextLocation = FMath::Lerp(CurrentLocation, TargetLocation, RISING_LERP_SPEED * DeltaTime);
	SetActorLocation(NextLocation);

	// 회전 로직
	FVector Axis = SubMeshComp->GetRightVector();
	const FQuat RotationDelta(Axis, RotationSpeed * DeltaTime);
	RotationSpeed += DeltaTime * 60;
	SubMeshComp->AddWorldRotation(RotationDelta);

	// 목표 지점에 도달하면 Returning 상태로 전환
	if (FVector::DistSquared(GetActorLocation(), TargetLocation) < FMath::Square(CATCH_DISTANCE_THRESHOLD))
	{
		PrevLocation = GetActorLocation();
		CurrentState = EAxeState::Returning;
	}
}

// 플레이어에게 돌아올 때의 로직
void AFlyingAxe::TickState_Returning(float DeltaTime)
{
	// 매 프레임 플레이어의 위치를 다시 타겟으로 설정하여 따라가게 함
	TargetLocation = Me->WithdrawPositionComp->GetComponentLocation();

	// VInterpTo를 사용해 부드럽게 타겟을 추적
	const FVector NextLocation = FMath::Lerp(PrevLocation, TargetLocation, ReturningInterpAlpha);
	//UE_LOG(LogTemp, Display, TEXT("ReturningInterpAlpha: %f"), ReturningInterpAlpha);
	ReturningInterpAlpha += ReturningAlphaDelta;
	if (ReturningInterpAlpha > 1.0f)
	{
		ReturningInterpAlpha = 1 - ReturningAlphaDelta - 0.001;
	}
	SetActorLocation(NextLocation);

	// 회전 로직
	FVector Axis = SubMeshComp->GetRightVector();
	Axis += FVector(.1);

	const FQuat RotationDelta(Axis, RotationSpeed * DeltaTime);
	RotationSpeed += DeltaTime * 30;
	SubMeshComp->AddWorldRotation(RotationDelta);

	// 플레이어에게 충분히 가까워지면 회수 처리
	if (FVector::DistSquared(GetActorLocation(), TargetLocation) < FMath::Square(CATCH_DISTANCE_THRESHOLD))
	{
		HandleCatch();
	}
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
	TargetLocation = WithdrawTargetPositionArr[randIdx]->GetComponentLocation();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorEnableCollision(true);
	//SubMeshComp->SetRelativeRotation(WithdrawRotation->GetRelativeRotation());

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
	SetActorEnableCollision(Active);
}

USoundCue* AFlyingAxe::GetBaseHitSound() const
{
	return BaseHitSoundCue;
}
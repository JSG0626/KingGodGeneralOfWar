// Fill out your copyright notice in the Description page of Project Settings.


#include "Kratos.h"
#include "SG_KratosAnim.h"
#include "Axe.h"
#include "SG_Shield.h"
#include "FlyingAxe.h"
#include "PlayerHPUI.h"
#include "PlayerAimUI.h"
#include "CSW/CSWGameMode.h"
#include "BaseEnemy.h"
#include "KratosStates/PlayerStateMappingDataAsset.h"

#include <Components/ArrowComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include <EnhancedInputComponent.h>

#include <Kismet/KismetSystemLibrary.h>
#include <Camera/PlayerCameraManager.h>
#include <Kismet/KismetMathLibrary.h>
#include <EnhancedInputSubsystems.h>
#include <Kismet/GameplayStatics.h>
#include <NiagaraFunctionLibrary.h>
#include <Sound/SoundBase.h>
#include <TimerManager.h>

// Sets default values

AKratos::AKratos()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SocketOffset = DefaultCameraOffset;
	SpringArmComp->TargetArmLength = DefaultTargetTargetArmLength;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	RightHandTransformComp = CreateDefaultSubobject<UArrowComponent>(TEXT("RightHandTransformComp"));
	RightHandTransformComp->SetupAttachment(GetMesh(), TEXT("hand_rAxeSocket"));

	LeftHandTransformComp = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftHandTransformComp"));
	LeftHandTransformComp->SetupAttachment(GetMesh(), TEXT("hand_lAxeSocket"));

	CurHP = MaxHP;
	GetCharacterMovement()->MaxWalkSpeed = PlayerMaxSpeed;
}
// Called to bind functionality to input
void AKratos::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
	{
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKratos::OnMyActionMove);
		input->BindAction(IA_Move, ETriggerEvent::Completed, this, &AKratos::OnMyActionIdle);
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AKratos::OnMyActionLook);
		input->BindAction(IA_Dodge, ETriggerEvent::Started, this, &AKratos::OnMyActionDodge);
		input->BindAction(IA_Run, ETriggerEvent::Triggered, this, &AKratos::OnMyActionRunOn);
		input->BindAction(IA_Run, ETriggerEvent::Completed, this, &AKratos::OnMyActionRunOff);
		input->BindAction(IA_Guard, ETriggerEvent::Triggered, this, &AKratos::OnMyActionGuardOn);
		input->BindAction(IA_Guard, ETriggerEvent::Completed, this, &AKratos::OnMyActionGuardOff);
		input->BindAction(IA_LockOn, ETriggerEvent::Started, this, &AKratos::OnMyActionLockOn);
		input->BindAction(IA_DebugKey, ETriggerEvent::Started, this, &AKratos::OnMyActionDebugKey);

		input->BindAction(IA_LightAttack, ETriggerEvent::Started, this, &AKratos::OnMyActionLAttack);
		input->BindAction(IA_HeavyAttack, ETriggerEvent::Started, this, &AKratos::OnMyActionHAttack);
		input->BindAction(IA_HeavyChargeAttack, ETriggerEvent::Triggered, this, &AKratos::OnMyActionHChargeAttack);
		input->BindAction(IA_Aim, ETriggerEvent::Triggered, this, &AKratos::OnMyActionAimOn);
		input->BindAction(IA_Aim, ETriggerEvent::Completed, this, &AKratos::OnMyActionAimOff);
		input->BindAction(IA_WithdrawAxe, ETriggerEvent::Started, this, &AKratos::OnMyActionAbility);
	}
}
void AKratos::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Warning, TEXT("PostInitializeComponents"));
	Anim = Cast<USG_KratosAnim>(GetMesh()->GetAnimInstance());
	if (Anim)
	{
		Anim->Me = this;
	}
}
// Called when the game starts or when spawned
void AKratos::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("BeginPlay"));
	UE_LOG(LogTemp, Display, TEXT("BeginPlay"));

	AttackRangeSquared = AttackRange * AttackRange;
	CurHP = MaxHP;
	// 1. 컨트롤러를 가져와서 PlayerController인지 캐스팅해본다.
	auto* pc = Cast<APlayerController>(Controller);
	if (AimWidgetClass)
	{
		AimWidget = CreateWidget<UPlayerAimUI>(GetWorld(), AimWidgetClass);
		if (AimWidget)
		{
			AimWidget->AddToViewport();
			AimWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (pc)
	{
		// 2. 그 객체를 이용해서 EnhancedInputLocalPlayerSubsystem을 가져온다,
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}
	}
	GameMode = Cast<ACSWGameMode>(GetWorld()->GetAuthGameMode());
	if (nullptr == Axe)
	{
		InitAxe();
	}

	if (nullptr == Shield)
	{
		InitShield();
	}

	DefaultCameraOffset = SpringArmComp->SocketOffset;
	TargetCameraOffset = DefaultCameraOffset;

	CMC = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	check(CMC);

	/*HpBarUI = CreateWidget<UPlayerHPUI>(GetWorld(), HpBarUIFactory);
	HpBarUI->AddToViewport();*/

	AttackShakeFactoryArr.Add(DownAttackShakeFactory);
	AttackShakeFactoryArr.Add(UpAttackShakeFactory);
	AttackShakeFactoryArr.Add(LeftAttackShakeFactory);
	AttackShakeFactoryArr.Add(RightAttackShakeFactory);

	InitializeStates();
}
// -------------------------------------------------- TICK -------------------------------------------------------------
// Called every frame
void AKratos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(1, DeltaTime, FColor::White, FString::Printf(TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed));
	if (nullptr != CurrentState)
	{
		CurrentState->TickState(FGenericStateParams(), DeltaTime);
	}
	PlayerMove();
	LockOnTargetTick(DeltaTime);

	// 플레이어 로테이션 선형 보간
	if (bLerpPlayerRotation)
	{
		//SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), TargetActorRotation, DeltaTime * LeprPlayerRotationScale, true));
		SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetActorRotation, DeltaTime, LeprPlayerRotationScale));
		if (GetActorRotation().Equals(TargetActorRotation, 0.01f))
		{
			bLerpPlayerRotation = false;
		}
	}

	// 카메라 시야각 선형 보간
	CameraComp->FieldOfView = FMath::Lerp(CameraComp->FieldOfView, TargetFOV, DeltaTime * 3);

	// 카메라 오프셋 선형 보간
	SpringArmComp->SocketOffset = FMath::Lerp(SpringArmComp->SocketOffset, TargetCameraOffset, DeltaTime * 2);

	// 쉴드 스케일 선형 보간. (가드시 커짐, 평소 스케일 0) 
	//Shield->MeshComp->SetWorldScale3D(FVector(FMath::Lerp(Shield->MeshComp->GetComponentScale().X, TargetShieldScale, DeltaTime * 16)));

	// TargetArmLength 선형 보간
	SpringArmComp->TargetArmLength = FMath::Lerp(SpringArmComp->TargetArmLength, TargetTargetArmLength, DeltaTime * 2);

	// 카메라 앵글 선형 보간
	CameraComp->SetRelativeRotation(UKismetMathLibrary::RLerp(CameraComp->GetRelativeRotation(), TargetCameraAngle, DeltaTime * 2, true));


	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, UEnum::GetValueAsString(State));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("TargetTargetArmLength: %f"), TargetTargetArmLength));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("TargetCameraOffset: %s"), *TargetCameraOffset.ToString()));

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("bAxeGone: %d"), bAxeGone));
}
// -------------------------------------------------- TICK -------------------------------------------------------------

void AKratos::PlayerMove()
{

	//FRotator ControlRotation = GetControlRotation();
	//ControlRotation.Pitch = 0;
	//FTransform T = UKismetMathLibrary::MakeTransform(FVector(0, 0, 0), ControlRotation, FVector(1, 1, 1));
	//FVector ForwardDirection = UKismetMathLibrary::TransformDirection(T, Direction);

	//PrevDirection = Direction;
	//Direction = FVector(0, 0, 0);

	//float MoveScale = 0;
	//switch (State)
	//{
	//case EPlayerState::Move:
	//	MoveScale = .56f;
	//	break;
	//case EPlayerState::Run:
	//	MoveScale = 1.0f;
	//	break;
	//}
	//AddMovementInput(ForwardDirection, MoveScale);
}

void AKratos::SetTargetToLockOn()
{
	float lockOnRadius = 1000000.0f;
	FVector cameraForwardVector = UKismetMathLibrary::GetForwardVector(CameraComp->USceneComponent::K2_GetComponentRotation());
	FVector actorLocation = GetActorLocation() + cameraForwardVector * 500;
	FVector endLocation = GetActorLocation() + cameraForwardVector * 5000;
	float Radius = 500;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)));
	TArray<AActor*> ActorsToIgnore;
	//EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	TArray<FHitResult> OutHits;
	bool bIgnoreSelf = false;
	FLinearColor TraceColor = FLinearColor::White;
	FLinearColor TraceHitColor = FLinearColor::Red;
	float DrawTime = 3.0f;
	FCollisionObjectQueryParams ObjectQueryParams;

	bLockOn = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		actorLocation,
		endLocation,
		Radius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		OutHits,
		bIgnoreSelf,
		TraceColor,
		TraceHitColor,
		DrawTime
	);

	if (bLockOn)
	{
		const FVector ActorLocation = GetActorLocation();
		FVector CameraForward = CameraComp->GetForwardVector();
		FVector ToTarget;
		float MaxScore = 0.0f;
		float MaxDistSquared = 0.0f;
		int LockTargetIdx = 0;
		for (int i = 0; i < OutHits.Num(); i++)
		{
			FVector CandidateLocation = OutHits[i].GetActor()->GetActorLocation();
			float DistSquared = FVector::DistSquared(GetActorLocation(), CandidateLocation);
			if (DistSquared >= MaxDistSquared)
			{
				MaxDistSquared = DistSquared;
			}
		}
		for (int i = 0; i < OutHits.Num(); i++)
		{
			FVector CandidateLocation = OutHits[i].GetActor()->GetActorLocation();
			ToTarget = CandidateLocation - ActorLocation;
			ToTarget.Normalize();
			float Dot = FVector::DotProduct(CameraForward, ToTarget);
			float DistSquared = FVector::DistSquared(GetActorLocation(), CandidateLocation);
			float Score = Dot + -(DistSquared / MaxDistSquared) * (State == EPlayerState::Aim ? 0.0f : DistPoint);
			UE_LOG(LogTemp, Display, TEXT("Actor: %s, Dot: %f"), *OutHits[i].GetActor()->GetName(), Dot);
			if (Score >= MaxScore)
			{
				MaxScore = Score;
				LockTargetIdx = i;
			}
		}

		ABaseEnemy* NewTarget = Cast<ABaseEnemy>(OutHits[LockTargetIdx].GetActor());
		if (NewTarget != nullptr)
		{
			if (LockTarget != nullptr && LockTarget != NewTarget)
			{
				LockTarget->ActiveLockOnUI(false);
			}
			LockTarget = NewTarget;
			LockTarget->ActiveLockOnUI(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("적 없음"));
	}
}

FORCEINLINE void AKratos::LockOnTargetTick(float DeltaTime)
{
	if (!IsValid(LockTarget))
	{
		bLockOn = false;
		return;
	}
	if (bLockOn)
	{
		FRotator playerCameraRotation = GetController()->AController::GetControlRotation();
		TargetCameraRotation = UKismetMathLibrary::FindLookAtRotation(CameraComp->GetComponentLocation(), LockTarget->GetActorLocation());
		GetController()->AController::SetControlRotation(TargetCameraRotation);
	}
}

void AKratos::InitAxe()
{
	FActorSpawnParameters param;
	const FName SocketName = TEXT("hand_rAxeSocket");
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Axe = GetWorld()->SpawnActor<AAxe>(AxeFactory, GetMesh()->GetSocketTransform(SocketName), param);
	if (Axe)
	{
		Axe->K2_AttachToComponent(GetMesh(), SocketName, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		Axe->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleWeapon"), true);
		Axe->Me = this;
	}
}

void AKratos::InitShield()
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Shield = GetWorld()->SpawnActor<ASG_Shield>(ShieldFactory, GetMesh()->GetSocketTransform(TEXT("hand_lShieldSocket")), param);
	if (Shield)
	{
		Shield->SetOwner(this);
		Shield->K2_AttachToComponent(GetMesh(), TEXT("hand_lShieldSocket"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		Shield->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleWeapon"), true);
		Shield->Init(this);
	}
}

void AKratos::OnMyActionDebugKey()
{

}

void AKratos::CameraShakeOnAttack(EAttackDirectionType attackDir, float scale)
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(AttackShakeFactoryArr[static_cast<int8>(attackDir)], scale);
}


void AKratos::SetGlobalTimeDilation(float Duration, float SlowScale)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowScale);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}, Duration, false);
}

void AKratos::SetAnimationSpeedSlow(float Duration, float SlowScale)
{
	const float OriginScale = Anim->Montage_GetPlayRate(nullptr);
	if (OriginScale <= 0.2) return;
	UE_LOG(LogTemp, Display, TEXT("SetAnimationSpeedSlow, OriginScale: %f"), OriginScale);
	Anim->Montage_SetPlayRate(nullptr, SlowScale);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this, OriginScale]()
	{
		UE_LOG(LogTemp, Display, TEXT("Speed TurnBack"));
		Anim->Montage_SetPlayRate(nullptr, OriginScale);
	}, Duration, false);
}

void AKratos::InitializeStates()
{
	check(PlayerStateDataAsset);
	for (TPair<EPlayerState, TSubclassOf<UKratosState>> Pair : PlayerStateDataAsset->StateClassMapData)
	{
		const EPlayerState StateKey = Pair.Key;
		const TSubclassOf<UKratosState> StateClass = Pair.Value;
		check(StateClass);

		TObjectPtr<UKratosState> StateObject = NewObject<UKratosState>(this, Pair.Value, UEnum::GetValueAsName(StateKey));
		StateObject->SetUp(this);
		KratosStatesMap.Add(StateKey, StateObject);
	}
	SetKratosState(EPlayerState::Idle);
}

void AKratos::SetKratosState(const EPlayerState& NewState, const FGenericStateParams& params)
{
	if (nullptr != CurrentState)
	{
		CurrentState->ExitState(params);
	}

	CurrentState = KratosStatesMap[NewState];
	State = NewState;
	if (nullptr != CurrentState)
	{
		CurrentState->EnterState(params);
	}
}

void AKratos::ActiveLerpPlayerRotation(FRotator TargetRotation, int Scale)
{
	if (TargetRotation == FRotator::ZeroRotator) return;
	TargetActorRotation = TargetRotation;
	LeprPlayerRotationScale = Scale;
	bLerpPlayerRotation = true;
}

void AKratos::InactiveLerpPlayerRotation()
{
	bLerpPlayerRotation = false;
}

void AKratos::OnMyActionMove(const FInputActionValue& Value)
{
	FVector2D Direction2D = Value.Get<FVector2D>();
	PrevDirection = Direction;

	Direction.X = Direction2D.X;
	Direction.Y = Direction2D.Y;
	Direction.Normalize();

	if (CurrentState->CanHandleMove())
	{
		CurrentState->HandleMove(FGenericStateParams(Value.Get<FVector2D>()));
	}
}

void AKratos::OnMyActionLook(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();

	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);

	if (bLockOn)
	{

	}
}

void AKratos::OnMyActionDodge(const FInputActionValue& value)
{
	if (CurrentState->CanHandleDodge())
	{
		CurrentState->HandleDodge(FGenericStateParams(Direction));
	}
}

void AKratos::OnMyActionRunOn(const FInputActionValue& value)
{
	bIsRunning = true;
}

void AKratos::OnMyActionRunOff(const FInputActionValue& value)
{
	bIsRunning = false;
}

void AKratos::OnMyActionGuardOn(const FInputActionValue& value)
{
	if (CurrentState->CanHandleGuard())
	{
		CurrentState->HandleGuard();
	}
}

void AKratos::OnMyActionGuardOff(const FInputActionValue& value)
{
	if (State == EPlayerState::Guard && CurrentState->CanHandleIdle())
	{
		CurrentState->HandleIdle();
	}
}

void AKratos::OnMyActionLockOn(const FInputActionValue& value)
{
	if (bLockOn)
	{
		bLockOn = false;
		LockTarget->ActiveLockOnUI(false);
		LockTarget = nullptr;
		return;
	}

	SetTargetToLockOn();
}

void AKratos::OnMyActionIdle(const FInputActionValue& value)
{
	if (State == EPlayerState::Move && CurrentState->CanHandleIdle())
	{
		CurrentState->HandleIdle();
	}
}

void AKratos::OnMyActionLAttack(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Display, TEXT("OnMyActionLAttack"));
	if (CurrentState->CanHandleLAttack())
	{
		CurrentState->HandleLAttack();
	}
	else
	{
		CurrentState->SetInputOn();
	}
}

void AKratos::OnMyActionHAttack(const FInputActionValue& value)
{
	if (CurrentState->CanHandleHAttack())
	{
		CurrentState->HandleHAttack();
	}
}

void AKratos::OnMyActionHChargeAttack(const FInputActionValue& value)
{
	if (CurrentState->CanHandleHChargeAttack())
	{
		CurrentState->HandleHChargeAttack();
	}
}

void AKratos::OnMyActionAimOn(const FInputActionValue& value)
{
	AimWidget->SetVisibility(ESlateVisibility::Visible);

	if (CurrentState->CanHandleAim())
	{
		CurrentState->HandleAim();
	}
}

void AKratos::OnMyActionAimOff(const FInputActionValue& value)
{
	AimWidget->SetVisibility(ESlateVisibility::Hidden);

	if (State == EPlayerState::Aim)
	{
		CurrentState->HandleIdle();
	}
}

void AKratos::OnMyActionAbility(const FInputActionValue& value)
{
	if (CurrentState->CanHandleAbility())
	{
		CurrentState->HandleAbility();
	}
}

void AKratos::HideHoldingAxe()
{
	Axe->MeshComp->SetVisibility(false, true);
}

void AKratos::ThrowAxe(const bool bIsHeavy)
{
	const FVector SpawnLoc = CameraComp->GetComponentLocation() + CameraComp->GetForwardVector() * 100.0f;
	const FRotator SpawnRot = CameraComp->GetComponentRotation();
	FlyingAxe = GetWorld()->SpawnActor<AFlyingAxe>(FlyingAxeFactory, SpawnLoc, SpawnRot);
	FlyingAxe->Init(this, bIsHeavy);
	bAxeGone = true;
	CameraShakeOnAttack(EAttackDirectionType::UP, 1.0f);
}

void AKratos::CallAxe()
{
	bIsAxeWithdrawing = true;
	FlyingAxe->BackToPlayer(false);
}

void AKratos::CallAxe(const float MaxReturnDuration, const float MinReturnDuration, bool bImmediateReturn, const float RadiusScale, const bool bRightHand)
{
	bIsAxeWithdrawing = true;
	FlyingAxe->BackToPlayer(MaxReturnDuration, MinReturnDuration, bImmediateReturn, RadiusScale, bRightHand);
}

void AKratos::CatchFlyingAxe()
{
	if (CurrentState->CanHandleGrabAxe())
	{
		CurrentState->HandleGrabAxe();
	}
	Axe->MeshComp->SetVisibility(true, true);
	bAxeGone = false;
	bIsAxeWithdrawing = false;
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CatchAxeShakeFactory, 0.2f);
}

void AKratos::ThrowAxeInAttack(const FRotator LocalRotationOffset, const bool bOrbital, const float OrbitalDuration, const float OrbitalCount, const bool bClockWise)
{
	const FVector SpawnLoc = Axe->GetActorLocation();
	const FRotator SpawnRot = GetActorRotation();
	FlyingAxe = GetWorld()->SpawnActor<AFlyingAxe>(FlyingAxeFactory, SpawnLoc, SpawnRot);
	FlyingAxe->Init(this, LocalRotationOffset, false, bOrbital, OrbitalDuration, OrbitalCount, bClockWise ? 1.0f : -1.0f);
	bAxeGone = true;
	CameraShakeOnAttack(EAttackDirectionType::UP, 1.0f);
}

bool AKratos::Damage(AActor* Attacker, int DamageValue, EHitType HitType, bool IsMelee)
{
	if (CurrentState->CanHandleHit())
	{
		CurrentState->HandleHit(FEnemyAttackParams(Attacker, DamageValue, HitType, IsMelee));
		return true;
	}
	return false;
}

float AKratos::SetHP(const float NewHP)
{
	CurHP = FMath::Max(NewHP, 0);
	if (GameMode)
	{
		GameMode->SetPlayerHpBar(CurHP / MaxHP);
		GameMode->PlayHitWidgetAnim();
	}
	return CurHP;
}

float AKratos::GetAttackPower(EPlayerWeaponType WeaponType) const
{
	switch (WeaponType)
	{
	case EPlayerWeaponType::Unarmed:
		return UnarmedAttackPower;
	case EPlayerWeaponType::Axe:
		return AxeAttackPower;
	case EPlayerWeaponType::Blade:
		return BladeAttackPower;
	case EPlayerWeaponType::Spear:
		return SpearAttackPower;
	default:
		return -1;
	}
}

void AKratos::ActiveAxeTrail(bool ActiveState)
{
	Axe->ActiveTrail(ActiveState);
}

TObjectPtr<class AActor> AKratos::FindTargetEnemy() const
{
	if (bLockOn && LockTarget) return LockTarget;

	FVector CameraForwardVector = GetControlRotation().Vector();
	FVector StartLocation = GetActorLocation() + CameraForwardVector * FindTargetRadius;
	FVector EndLocation = StartLocation + CameraForwardVector * FindTargetEndLocation;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)));
	TArray<AActor*> ActorsToIgnore;
	//EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	TArray<FHitResult> OutHits;
	bool bIgnoreSelf = false;
	FLinearColor TraceColor = FLinearColor::White;
	FLinearColor TraceHitColor = FLinearColor::Red;
	float DrawTime = 3.0f;
	FCollisionObjectQueryParams ObjectQueryParams;
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		FindTargetRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugType,
		OutHits,
		bIgnoreSelf,
		TraceColor,
		TraceHitColor,
		DrawTime
	);

	const FVector CurLocation = GetActorLocation();
	if (bHit)
	{
		FVector ToTarget;
		float MaxScore = 0.0f;
		float MaxDistSquared = 0.0f;
		int LockTargetIdx = 0;
		for (int i = 0; i < OutHits.Num(); i++)
		{
			FVector CandidateLocation = OutHits[i].GetActor()->GetActorLocation();
			float DistSquared = FVector::DistSquared(CurLocation, CandidateLocation);
			if (DistSquared >= MaxDistSquared)
			{
				MaxDistSquared = DistSquared;
			}
		}
		for (int i = 0; i < OutHits.Num(); i++)
		{
			FVector CandidateLocation = OutHits[i].GetActor()->GetActorLocation();
			ToTarget = CandidateLocation - CurLocation;
			ToTarget.Normalize();
			float Dot = FVector::DotProduct(CameraForwardVector, ToTarget);
			float DistSquared = FVector::DistSquared(CurLocation, CandidateLocation);
			float Score = Dot + (1 - DistSquared / MaxDistSquared) * FindTargetDistPoint;
			if (Score >= MaxScore)
			{
				MaxScore = Score;
				LockTargetIdx = i;
			}
		}

		AActor* NewTarget = OutHits[LockTargetIdx].GetActor();
		if (NewTarget != nullptr)
		{
			return NewTarget;
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("적 없음"));
	}
	return nullptr;
}


void AKratos::InitMaxWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = PlayerMaxSpeed;
}

void AKratos::SetMaxWalkSpeed(const float NewWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;
}

void AKratos::PlayMontage(const EPlayerMontage MontageType, bool bJumpSection, const FString SectionName)
{
	Anim->PlayMontage(MontageType, bJumpSection, SectionName);
}

void AKratos::PlayMontage(const EPlayerMontage MontageType, const int SectionNumber)
{
	Anim->PlayMontage(MontageType, true, FString::FromInt(SectionNumber));
}

void AKratos::Montage_JumpToSection(const int SectionNumber)
{
	Anim->Montage_JumpToSection(*FString::FromInt(SectionNumber));
}

void AKratos::Montage_JumpToSection(const FString SectionName)
{
	Anim->Montage_JumpToSection(*SectionName);
}

void AKratos::OnHitHChargeAttack()
{
	UE_LOG(LogTemp, Display, TEXT("OnHitHChargeAttack"));
	if (CurrentState->CanHandleHChargeAttackHit())
	{
		UE_LOG(LogTemp, Display, TEXT("HandleHChargeAttack"));
		CurrentState->HandleHChargeAttackHit();
	}
}

void AKratos::SwapAxeHands(bool Right)
{
	FName SocketName = Right ? TEXT("hand_rAxeSocket") : TEXT("hand_lAxeSocket");
	Axe->K2_AttachToComponent(GetMesh(), SocketName, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
}

void AKratos::SetMeshSpaceRotationBlend(const bool bActive) const
{
	Anim->bMeshSpaceRotationBlend = bActive;
}

TObjectPtr<class UArrowComponent> AKratos::GetHandTransformComp(const bool bRightHand) const
{
	return bRightHand ? RightHandTransformComp : LeftHandTransformComp;
}
FVector AKratos::GetCameraLocation() const
{
	return CameraComp->GetComponentLocation();
}

FRotator AKratos::GetCameraRotation(bool bRemovePitch) const
{
	return CameraComp->GetComponentRotation();
}

float AKratos::GetAttackRangeSquared() const
{
	return AttackRangeSquared;
}

void AKratos::ChangeAimUIColor(bool bHit) const
{
	AimWidget->ChangeColor(bHit);
}

float AKratos::GetDamage(const float Damage)
{
	return SetHP(CurHP - Damage);
}
TObjectPtr<class AAxe> AKratos::GetAxe() const
{
	return Axe;
}

TObjectPtr<class ASG_Shield> AKratos::GetShield() const
{
	return Shield;
}

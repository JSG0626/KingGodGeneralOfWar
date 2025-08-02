// Fill out your copyright notice in the Description page of Project Settings.


#include "Kratos.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "TimerManager.h"
#include "SG_KratosAnim.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Axe.h"
#include "SG_Shield.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/ArrowComponent.h"
#include "PlayerHPUI.h"
#include "CSW/AwakenThor.h"
#include "CSW/AwakenThorFSM.h"
#include "BDThor.h"
#include "BDThorFSM.h"
#include "FlyingAxe.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "CSW/CSWGameMode.h"
#include "Sound/SoundBase.h"
#include "PlayerAimUI.h"
#include "KratosStates/KS_Idle.h"
#include "KratosStates/KS_WAttack.h"
#include <KratosStates/KS_Move.h>
#include <KratosStates/KS_Dodge.h>
#include <KratosStates/KS_Guard.h>
#include <KratosStates/KS_Aim.h>
#include <BaseEnemy.h>
#include <KratosStates/KS_Hit.h>
#include <KratosStates/KS_Parry.h>
#include <KratosStates/KS_SAttack.h>
#include <KratosStates/KS_RuneWAttack.h>
#include <KratosStates/KS_Die.h>

// Sets default values

const float WALK_FOV = 90;
const float RUN_FOV = 105;
const float GUARD_FOV = 70;
const float AIM_FOV = 60;
const float PARRY_FOV = 100;
const float ATTACK_FOV = 105;
const int GUARD_MAX_COUNT = 2;

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

	WithdrawPositionComp = CreateDefaultSubobject<UArrowComponent>(TEXT("WithdrawPositionComp"));
	WithdrawPositionComp->SetupAttachment(GetMesh(), TEXT("hand_rAxeSocket"));

	CurHP = MaxHP;
	GetCharacterMovement()->MaxWalkSpeed = PlayerMaxSpeed;

	// Sound Referencing
	{
		static ConstructorHelpers::FObjectFinder<USoundBase> avoid_sfx(TEXT("/ Script / Engine.SoundWave'/Game/JSG/SFX/RealSFX/Avoid.Avoid'"));
		if (avoid_sfx.Succeeded())	AvoidSound = avoid_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> axeThrow_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Axe2Throw.Axe2Throw'"));
		if (axeThrow_sfx.Succeeded())	AxeThrowSound = axeThrow_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> AxeWithdraw_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Axe3Get.Axe3Get'"));
		if (AxeWithdraw_sfx.Succeeded())	AxeWithdrawSound = AxeWithdraw_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> hit1_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/PlayerDamaged3.PlayerDamaged3'"));
		if (hit1_sfx.Succeeded())	HitSound1 = hit1_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> hit2_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/PlayerDamaged.PlayerDamaged'"));
		if (hit2_sfx.Succeeded())	HitSound2 = hit2_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> hit3_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/PlayerDamaged2.PlayerDamaged2'"));
		if (hit3_sfx.Succeeded())	HitSound3 = hit3_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> roll_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Roll.Roll'"));
		if (roll_sfx.Succeeded())	RollSound = roll_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> runeBase_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/RuneReady.RuneReady'"));
		if (runeBase_sfx.Succeeded())	RuneBaseSound = runeBase_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> rune1_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Rune1.Rune1'"));
		if (rune1_sfx.Succeeded())	RuneAttack1Sound = rune1_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> rune2_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Rune2.Rune2'"));
		if (rune2_sfx.Succeeded())	RuneAttack2Sound = rune2_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> rune3_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Rune3.Rune3'"));
		if (rune3_sfx.Succeeded())	RuneAttack3Sound = rune3_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> rune4_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/Rune4.Rune4'"));
		if (rune4_sfx.Succeeded())	RuneAttack4Sound = rune4_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> strong1_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/StrongAttack1.StrongAttack1'"));
		if (strong1_sfx.Succeeded())	StrongAttack1Sound = strong1_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> strong2_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/StrongAttack2.StrongAttack2'"));
		if (strong2_sfx.Succeeded())	StrongAttack2Sound = strong2_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> strong3_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/StrongAttack3.StrongAttack3'"));
		if (strong3_sfx.Succeeded())	StrongAttack3Sound = strong3_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> strong4_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/StrongAttack4.StrongAttack4'"));
		if (strong4_sfx.Succeeded())	StrongAttack4Sound = strong4_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> weak1_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/WeakAttack1.WeakAttack1'"));
		if (weak1_sfx.Succeeded())	WeakAttack1Sound = weak1_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> weak2_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/WeakAttack2.WeakAttack2'"));
		if (weak2_sfx.Succeeded())	WeakAttack2Sound = weak2_sfx.Object;

		static ConstructorHelpers::FObjectFinder<USoundBase> weak3_sfx(TEXT("/Script/Engine.SoundWave'/Game/JSG/SFX/RealSFX/WeakAttack3.WeakAttack3'"));
		if (weak3_sfx.Succeeded())	WeakAttack3Sound = weak3_sfx.Object;

		WeakAttackSoundArr.Add(WeakAttack1Sound);
		WeakAttackSoundArr.Add(WeakAttack1Sound);
		WeakAttackSoundArr.Add(WeakAttack2Sound);
		WeakAttackSoundArr.Add(WeakAttack3Sound);

		StrongAttackSoundArr.Add(StrongAttack1Sound);
		StrongAttackSoundArr.Add(StrongAttack1Sound);
		StrongAttackSoundArr.Add(StrongAttack2Sound);
		StrongAttackSoundArr.Add(StrongAttack3Sound);
		StrongAttackSoundArr.Add(StrongAttack4Sound);

		RuneAttackSoundArr.Add(RuneAttack1Sound);
		RuneAttackSoundArr.Add(RuneAttack1Sound);
		RuneAttackSoundArr.Add(RuneAttack2Sound);
		RuneAttackSoundArr.Add(RuneAttack3Sound);
		RuneAttackSoundArr.Add(RuneAttack4Sound);

	}
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

		input->BindAction(IA_WeakAttack, ETriggerEvent::Started, this, &AKratos::OnMyActionWAttack);
		input->BindAction(IA_StrongAttack, ETriggerEvent::Started, this, &AKratos::OnMyActionSAttack);
		input->BindAction(IA_Aim, ETriggerEvent::Triggered, this, &AKratos::OnMyActionAimOn);
		input->BindAction(IA_Aim, ETriggerEvent::Completed, this, &AKratos::OnMyActionAimOff);
		input->BindAction(IA_WithdrawAxe, ETriggerEvent::Started, this, &AKratos::OnMyActionAbility);
		input->BindAction(IA_RuneBase, ETriggerEvent::Started, this, &AKratos::OnMyActionRuneBase);

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
		SetWeapon();
	}

	if (nullptr == Shield)
	{
		SetShield();
	}
	GuardHitCnt = GUARD_MAX_COUNT;

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

	GEngine->AddOnScreenDebugMessage(1, DeltaTime, FColor::White, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(State)));
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


	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("CurrentAttackType: %s"), *UEnum::GetValueAsString(CurrentAttackType)));

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, GetPlayerStateString());
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("TargetTargetArmLength: %f"), TargetTargetArmLength));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, FString::Printf(TEXT("TargetCameraOffset: %s"), *TargetCameraOffset.ToString()));

}
// -------------------------------------------------- TICK -------------------------------------------------------------

FString AKratos::GetPlayerStateString()
{
	return UEnum::GetValueAsString(State);
}
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

void AKratos::SetLockOnTarget()
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
		float MaxDot = 0.0f;
		int LockTargetIdx = 0;
		for (int i = 0; i < OutHits.Num(); i++)
		{
			ToTarget = OutHits[i].GetActor()->GetActorLocation() - ActorLocation;
			ToTarget.Normalize();
			float Dot = FVector::DotProduct(CameraForward, ToTarget);
			UE_LOG(LogTemp, Display, TEXT("Actor: %s, Dot: %f"), *OutHits[i].GetActor()->GetName(), Dot);
			if (Dot >= MaxDot)
			{
				MaxDot = Dot;
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

void AKratos::OnMyGuardDisappear()
{
	TargetShieldScale = 0;
}

void AKratos::OnMyLaunchCharacterInStrongAttack()
{
	const float LaunchScale = 500;
	LaunchCharacter(GetActorForwardVector() * LaunchScale, true, false);
}

void AKratos::OnMyJumpCharacterInStrongAttack()
{
	const float LaunchScale = 250;
	FVector dir = GetActorForwardVector() + FVector(0, 0, 1);
	dir.Normalize();
	LaunchCharacter(dir * LaunchScale, true, true);
}

void AKratos::IncreaseTargetTargetArmLength(float value)
{
	TargetTargetArmLength += value;
}

void AKratos::IncreaseTargetCameraOffset(FVector value)
{
	TargetCameraOffset += value;
}

void AKratos::SetWeapon()
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Axe = GetWorld()->SpawnActor<AAxe>(AxeFactory, GetMesh()->GetSocketTransform(TEXT("hand_rAxeSocket")), param);
	if (Axe)
	{
		Axe->K2_AttachToComponent(GetMesh(), TEXT("hand_rAxeSocket"), EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		Axe->MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleWeapon"), true);
		Axe->Me = this;
	}
}

void AKratos::SetShield()
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


void AKratos::OnMyRuneAttackCameraSet()
{
	TargetFOV = 100;
	TargetCameraOffset = DefaultCameraOffset + FVector(0, 20, 55);
	TargetCameraAngle = DefaultCameraAngle + FRotator(-10, 0, 0);
	TargetTargetArmLength = TargetTargetArmLength + 30;
}

void AKratos::OnMySpawnEarthCrack()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EarthCrackVFX, Axe->MeshComp->GetComponentLocation() - FVector(0, 0, 22));
}



void AKratos::OnMyInitAttackType()
{
	CurrentAttackType = EAttackType::NONE;
}

void AKratos::OnMyAttackProgress()
{
	GetMovementComponent()->Velocity = GetActorForwardVector() * 2000;
}

void AKratos::OnMyEndWithFail()
{
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
	{
		GameMode->EndWithFail();
	}, 2.0f, false);
}

void AKratos::OnMyGetUPCameraSet()
{
	TargetCameraOffset = DefaultCameraOffset;
	TargetCameraAngle = DefaultCameraAngle;
	TargetTargetArmLength = DefaultTargetTargetArmLength;
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

FString AKratos::GetHitSectionName(EHitType hitType)
{
	FString HitTypeValueAsString = UEnum::GetValueAsString(hitType);
	return HitTypeValueAsString.Mid(10);
}

FString AKratos::GetDodgeDirection()
{
	float absX = abs(PrevDirection.X), absY = abs(PrevDirection.Y);
	FString DodgeDirString = "";
	UE_LOG(LogTemp, Display, TEXT("PrevDirection: %s"), *PrevDirection.ToString());
	if (absY >= 0.7)
	{
		if (PrevDirection.Y >= 0.7)
			DodgeDirString += TEXT("R");
		else
			DodgeDirString += TEXT("L");
	}
	if (absX >= 0.7f)
	{
		if (PrevDirection.X >= 0.7)
			DodgeDirString += TEXT("F");
		else
			DodgeDirString += TEXT("B");
	}
	return DodgeDirString;
}

void AKratos::InitializeStates()
{
	for (int i = 0; i < StateClassSetUp.Num(); i++)
	{
		const FStateClassPair& Pair = StateClassSetUp[i];
		TObjectPtr<UKratosState> StateObject = NewObject<UKratosState>(this, Pair.StateClass, UEnum::GetValueAsName(Pair.State));
		StateObject->SetUp(this);
		UClass* ActualClass = StateObject->GetClass();
		FString ClassName = ActualClass->GetName();
		UE_LOG(LogTemp, Display, TEXT("%s : %s"), *UEnum::GetValueAsString(Pair.State),  *ClassName);
		KratosStatesMap.Add(Pair.State, StateObject);
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
	TargetActorRotation = TargetRotation;
	LeprPlayerRotationScale = Scale;
	bLerpPlayerRotation = true;
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

	SetLockOnTarget();
}

void AKratos::OnMyActionIdle(const FInputActionValue& value)
{
	if (State == EPlayerState::Move && CurrentState->CanHandleIdle())
	{
		CurrentState->HandleIdle();
	}
}

void AKratos::OnMyActionWAttack(const FInputActionValue& value)
{
	if (CurrentState->CanHandleWAttack())
	{
		CurrentState->HandleWAttack();
	}
	else
	{
		CurrentState->SetInputOn();
	}
}

void AKratos::OnMyActionSAttack(const FInputActionValue& value)
{
	if (CurrentState->CanHandleSAttack())
	{
		CurrentState->HandleSAttack();
	}
}

void AKratos::OnMyActionAimOn(const FInputActionValue& value)
{
	if (CurrentState->CanHandleAim())
	{
		CurrentState->HandleAim();
	}
}

void AKratos::OnMyActionAimOff(const FInputActionValue& value)
{
	if (State == EPlayerState::Aim)
	{
		CurrentState->HandleIdle();
	}
}

void AKratos::OnMyActionAbility(const FInputActionValue& value)
{
	if (bAxeGone && !bIsAxeWithdrawing)
	{
		bIsAxeWithdrawing = true;
		WithdrawAxe();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Use Ability"));
	}
}

void AKratos::OnMyActionRuneBase(const FInputActionValue& value)
{
	//if (bAxeGone) return;
	//if (State == EPlayerState::Idle || State == EPlayerState::Move || State == EPlayerState::Run)
	//{
	//	Anim->PlayRuneBaseMontage();
	//	UGameplayStatics::PlaySound2D(GetWorld(), RuneBaseSound, 1, 1, 0.2f);
	//}
}

void AKratos::HideHoldingAxe()
{
	Axe->MeshComp->SetVisibility(false, true);
}

void AKratos::ThrowAxe()
{
	FlyingAxe = GetWorld()->SpawnActor<AFlyingAxe>(FlyingAxeFactory, CameraComp->GetComponentLocation() + CameraComp->GetForwardVector() * 100.0f, CameraComp->GetComponentRotation());
	bAxeGone = true;
	CameraShakeOnAttack(EAttackDirectionType::UP, 1.0f);
}

void AKratos::WithdrawAxe()
{
	const float dist = FVector::Dist(GetActorLocation(), FlyingAxe->GetActorLocation());
	if (State != EPlayerState::Dodge)
	{
		Anim->PlayMontage(EPlayerMontage::GrabAxe);
	}
	if (dist <= DirectGrabRange)
	{
		CatchFlyingAxe();
		FlyingAxe->Destroy();
	}
	else
	{
		FlyingAxe->BackToPlayer();
	}
}

void AKratos::CatchFlyingAxe()
{
	UGameplayStatics::PlaySound2D(GetWorld(), GrabAxeSound);

	Axe->MeshComp->SetVisibility(true, true);
	bAxeGone = false;
	bIsAxeWithdrawing = false;

	//if (State != EPlayerState::Hit && State != EPlayerState::Dodge)
	{
		Anim->Montage_JumpToSection(TEXT("Catch"));
	}

	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CatchAxeShakeFactory, 0.2f);
}



bool AKratos::Damage(AActor* Attacker, int DamageValue, EHitType HitType, bool IsMelee)
{
	if (CurrentState->CanHandleHit())
	{
		CurrentState->HandleHit(FEnemyAttackParams(Attacker, DamageValue, HitType, IsMelee));
	}
	return true;
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
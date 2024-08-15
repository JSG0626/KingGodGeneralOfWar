// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThor.h"
#include "BDThorFSM.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kratos.h"
#include "WindSlash.h"
#include "BDThorMjolnir.h"
#include "BDThorHP.h"
#include "Blueprint/UserWidget.h"
#include "SG_GodOfWar_GameModeBase.h"
#include "DrawDebugHelpers.h" //����� Ȯ�ο�
#include "Components/CapsuleComponent.h"
#include "SG_Shield.h"
#include "Components/SphereComponent.h"
#include "CSW/CSWGameMode.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

// Sets default values
ABDThor::ABDThor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�丣 ���̷�Ż �޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Bada/Asset/Model/BDThor/BDThor.BDThor'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//�޽� ũ�� ����
		GetMesh()->SetRelativeScale3D(FVector(0.25f));
		GetMesh()->SetRelativeLocation(FVector(0, 0, -118.0f)); //�޽� ��ġ ����
	}

	//���� ������ �ε�
	BDWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BDWeapon"));
	BDWeapon->SetupAttachment(GetMesh(), FName("BDMjolnirHips"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("/Script/Engine.StaticMesh'/Game/Bada/Asset/Model/weapon/BDMjolnir.BDMjolnir'"));
	if (WeaponMesh.Succeeded()) {
		BDWeapon->SetStaticMesh(WeaponMesh.Object);
		BDWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision); //�ӽ� �ڵ�
	}

	//�ݸ��� ����
	BDWeaponCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BDWeaponCol"));
	BDWeaponCol->SetupAttachment(BDWeapon); //���⿡ �ݸ��� ���̱�
	BDWeaponCol->SetCollisionProfileName(FName("BDThorMjonlr"));

	//BDThorFSM ������Ʈ �߰�
	fsm = CreateDefaultSubobject<UBDThorFSM>(TEXT("FSM"));

	//�ִϸ��̼� �������Ʈ �Ҵ��ϱ�
	/*ConstructorHelpers::FClassFinder<UAnimInstance> BDThorAnimation(TEXT("/Script/Engine.AnimBlueprint'/Game/Bada/BDAnimation/ABP_BDThor.ABP_BDThor_C'"));
	if (BDThorAnimation.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(BDThorAnimation.Class);
	}*/

	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(25.f);

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -110.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));


	//�� �ݸ���
	HeadCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HeadCapsule"));
	HeadCapsule->SetupAttachment(GetMesh(), FName("Head"));
	HeadCapsule->SetCapsuleHalfHeight(90.f);
	HeadCapsule->SetCapsuleRadius(90.f);
	HeadCapsule->SetCollisionProfileName(TEXT("Enemy"));
	HeadCapsule->SetRelativeLocation(FVector(-0.000000f, -40.000002f, 0.000000f));

	LArmCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LArmCapsule"));
	LArmCapsule->SetupAttachment(GetMesh(), FName("LeftArm"));
	LArmCapsule->SetCapsuleHalfHeight(110.f);
	LArmCapsule->SetCapsuleRadius(70.f);
	LArmCapsule->SetCollisionProfileName(TEXT("Enemy"));
	LArmCapsule->SetRelativeLocation(FVector(-0.000000f, -40.000002f, -0.000000f));
	LArmCapsule->SetRelativeRotation(FRotator(90.000000f, 26.565051f, -63.434949f));

	RArmCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RArmCapsule"));
	RArmCapsule->SetupAttachment(GetMesh(), FName("RightArm"));
	RArmCapsule->SetCapsuleHalfHeight(110.f);
	RArmCapsule->SetCapsuleRadius(70.f);
	RArmCapsule->SetCollisionProfileName(TEXT("Enemy"));
	RArmCapsule->SetRelativeLocation(FVector(0.000000f, -40.000002f, 0.000000f));
	RArmCapsule->SetRelativeRotation(FRotator(90.000000f, 14.036243f, -75.963757f));

	LForeArmCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LForeArmCapsule"));
	LForeArmCapsule->SetupAttachment(GetMesh(), FName("LeftForeArm"));
	LForeArmCapsule->SetCapsuleHalfHeight(100.f);
	LForeArmCapsule->SetCapsuleRadius(60.f);
	LForeArmCapsule->SetCollisionProfileName(TEXT("Enemy"));
	LForeArmCapsule->SetRelativeLocation(FVector(0.000000f, -120.000007f, -0.000000f));
	LForeArmCapsule->SetRelativeRotation(FRotator(90.000000f, -7.125016f, -97.125016f));

	RForeArmCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RForeArmCapsule"));
	RForeArmCapsule->SetupAttachment(GetMesh(), FName("RightForeArm"));
	RForeArmCapsule->SetCapsuleHalfHeight(100.f);
	RForeArmCapsule->SetCapsuleRadius(60.f);
	RForeArmCapsule->SetCollisionProfileName(TEXT("Enemy"));
	RForeArmCapsule->SetRelativeLocation(FVector(-0.000000f, -120.000007f, -0.000000f));
	RForeArmCapsule->SetRelativeRotation(FRotator(90.000000f, 90.000000f, 0.000000f));

	RLegCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RLegCapsule"));
	RLegCapsule->SetupAttachment(GetMesh(), FName("RightLeg"));
	RLegCapsule->SetCapsuleHalfHeight(130.f);
	RLegCapsule->SetCapsuleRadius(70.f);
	RLegCapsule->SetCollisionProfileName(TEXT("Enemy"));
	RLegCapsule->SetRelativeLocation(FVector(0.000000f, -80.000005f, 0.000000f));
	RLegCapsule->SetRelativeRotation(FRotator(90.000000f, 90.000000f, 0.000000f));

	RUpLegCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RUpLegCapsule"));
	RUpLegCapsule->SetupAttachment(GetMesh(), FName("RightUpLeg"));
	RUpLegCapsule->SetCapsuleHalfHeight(160.f);
	RUpLegCapsule->SetCapsuleRadius(90.f);
	RUpLegCapsule->SetCollisionProfileName(TEXT("Enemy"));
	RUpLegCapsule->SetRelativeLocation(FVector(-0.000000f, -80.000005f, 0.000000f));
	RUpLegCapsule->SetRelativeRotation(FRotator(90.000000f, 90.000000f, 0.000000f));

	LLegCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LLegCapsule"));
	LLegCapsule->SetupAttachment(GetMesh(), FName("LeftLeg"));
	LLegCapsule->SetCapsuleHalfHeight(130.f);
	LLegCapsule->SetCapsuleRadius(70.f);
	LLegCapsule->SetCollisionProfileName(TEXT("Enemy"));
	LLegCapsule->SetRelativeLocation(FVector(0.000000f, -80.000005f, 0.000000f));
	LLegCapsule->SetRelativeRotation(FRotator(90.000000f, 90.000000f, 0.000000f));

	LUpLegCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LUpLegCapsule"));
	LUpLegCapsule->SetupAttachment(GetMesh(), FName("LeftUpLeg"));
	LUpLegCapsule->SetCapsuleHalfHeight(160.f);
	LUpLegCapsule->SetCapsuleRadius(90.f);
	LUpLegCapsule->SetCollisionProfileName(TEXT("Enemy"));
	LUpLegCapsule->SetRelativeLocation(FVector(-0.000000f, -80.000005f, 0.000000f));
	LUpLegCapsule->SetRelativeRotation(FRotator(90.000000f, 90.000000f, 0.000000f));

}

// Called when the game starts or when spawned
void ABDThor::BeginPlay()
{
	Super::BeginPlay();


	BDWeaponCol->OnComponentBeginOverlap.AddDynamic(this, &ABDThor::BDWeaponOverlap);


	//�ε��� �� ����� �ϴ� �㸮�� ���̰� �ϱ�
	visibleWeapon();

	////UI �����ֱ�
	//if (BDThorHPClass) {
	//	UUserWidget* BDHPBar = CreateWidget<UUserWidget>(GetWorld(), BDThorHPClass);
	//	BDThorHPBar = Cast<UBDThorHP>(BDHPBar);
	//	if (BDThorHPBar) {
	//		BDThorHPBar->AddToViewport();
	//		BDThorHPBar->SetVisibility(ESlateVisibility::Visible);
	//	}
	//}

	fsm->BDCurrentHP = fsm->BDMaxHp; //ü�� ����

	GameMode = Cast<ACSWGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->SetEnemyHpBar(fsm->BDCurrentHP / fsm->BDMaxHp);
	//UpdateHpUI();
}

// Called every frame
void ABDThor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called to bind functionality to input
void ABDThor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//���� ��ġ�� ������ �����ϴ� �Լ�
void ABDThor::EquipWeapon()
{
	if (this) {
		IsWeaponHold = true;
		BDWeapon->SetVisibility(true);
		BDWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BDMjolnirHand"));
		BDWeaponCol->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//UE_LOG(LogTemp, Warning, TEXT("Equip Weapon"));
	}
}


//���� ��ġ�� �㸮�� �����ϴ� �Լ�
void ABDThor::DrawWeapon()
{
	if (this) {
		IsWeaponHold = false;
		BDWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BDMjolnirHips"));
		BDWeapon->SetVisibility(true);
		BDWeaponCol->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

//�����տ� ���
void ABDThor::EquipRight()
{
	if (this) {
		IsWeaponHold = true;
		BDWeapon->SetVisibility(true);
		BDWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BDMjolnirRightHand"));
		BDWeaponCol->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ABDThor::HiddenWeapon()
{
	//UE_LOG(LogTemp, Warning, TEXT("Hidden"));
	if (this) {
		IsWeaponHold = false;
		BDWeapon->SetVisibility(false); //�޽� �Ⱥ��̰� �ϱ�
	}
}

void ABDThor::visibleWeapon()
{
	if (this) {
		IsWeaponHold = true;
		BDWeapon->SetVisibility(true); //�޽� �Ⱥ��̰� �ϱ�
		//UE_LOG(LogTemp, Warning, TEXT("Visible"));
	}
}

void ABDThor::BDHammerThrowHit()
{
	FTransform t = GetMesh()->GetSocketTransform(TEXT("BDMjolnirHand"));
	//ABDThorMjolnir* Mjolnir = GetWorld()->SpawnActor<ABDThorMjolnir>(MjolnirFactory, t);
	FlyWeapon  = GetWorld()->SpawnActor<ABDThorMjolnir>(MjolnirFactory, t);

	if (FlyWeapon)
	{
		//FVector LaunchDirection = (fsm->Target->GetActorLocation() - t.GetLocation()).GetSafeNormal();
		FVector LaunchDirection = GetActorForwardVector();
		FlyWeapon->FireInDirection(LaunchDirection);
		//UE_LOG(LogTemp, Warning, TEXT("Mjolnir spawned successfully and fired"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Mjolnir spawned failed"));
	}
}

//�ٶ� ������
void ABDThor::BDHammerWindSlash()
{
	//�ٶ� ���� �����ؼ� ����
	FActorSpawnParameters parm;
	parm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AWindSlash>(SlashFat, GetActorLocation(), GetActorRotation(), parm);
}

//void ABDThor::UpdateHpUI()
//{
//	if (BDThorHPBar) {
//		BDThorHPBar->SetHP(fsm->BDCurrentHP, fsm->BDMaxHp);
//	}
//
//	if (fsm->BDCurrentHP < 0) {
//		//2 ������� ��ȯ
//		ASG_GodOfWar_GameModeBase* BDGameMode = Cast<ASG_GodOfWar_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); //���Ӹ�� ĳ��Ʈ
//
//		if (BDThorHPBar) {
//			BDThorHPBar->RemoveFromParent(); //HP�� 0�� �Ǹ� �����ض�
//		}
//
//		if (BDGameMode) {
//			BDGameMode->ThorFadeOut(); //���̵� �ƿ� ����
//		}
//
//
//	}
//
//}

void ABDThor::BDWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//������ ó��
		auto* AttackTarget = Cast<AKratos>(OtherActor); //Ÿ���϶�
		auto* shield = Cast<ASG_Shield>(OtherActor);
		//�÷��̾ �°�, �÷��̾�� �������� ���� ����, true�� return�Ҷ� ��� ����
		if (AttackTarget) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), fsm->BDThreeSwingVFX, BDWeaponCol->GetComponentLocation()); //����Ʈ
			if (AttackTarget->Damage(this, 10, EHitType::NB_HIGH, true) == true) {
				fsm->bBDAttackCheck = true;
				AttackTarget->Damage(this, 10, EHitType::NB_HIGH, true);
				fsm->BDSetState(BDThorGeneralState::BDBackDodge);
				//fsm->BDSetState(BDThorGeneralState::BDAvoidance);
				UE_LOG(LogTemp, Warning, TEXT("Kratos Attack!!")); //ȸ�Ƿ� ����
			}
		}
		else if(shield){
			UE_LOG(LogTemp, Warning, TEXT("shield Attack"));
		}
	}
}




// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThor.h"
#include "BDThorFSM.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kratos.h"
#include "WindSlash.h"
#include "BDThorMjolnir.h"

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

	//BDThorFSM ������Ʈ �߰�
	fsm = CreateDefaultSubobject<UBDThorFSM>(TEXT("FSM"));

	//�ִϸ��̼� �������Ʈ �Ҵ��ϱ�
	ConstructorHelpers::FClassFinder<UAnimInstance> BDThorAnimation(TEXT("/Script/Engine.AnimBlueprint'/Game/Bada/BDAnimation/ABP_BDThor.ABP_BDThor'"));
	if (BDThorAnimation.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(BDThorAnimation.Class);
	}
}

// Called when the game starts or when spawned
void ABDThor::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bOrientRotationToMovement = true; //�÷��̾� �������� �ٶ󺸱�

	//�ε��� �� ����� �ϴ� �㸮�� ���̰� �ϱ�
	visibleWeapon();
	
}

// Called every frame
void ABDThor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		UE_LOG(LogTemp, Warning, TEXT("Equip Weapon"));
	}
}


//���� ��ġ�� �㸮�� �����ϴ� �Լ�
void ABDThor::DrawWeapon()
{
	if (this) {
		IsWeaponHold = false;
		BDWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("BDMjolnirHips"));
		BDWeapon->SetVisibility(true);
	}
}

void ABDThor::HiddenWeapon()
{
	//UE_LOG(LogTemp, Warning, TEXT("Hidden"));
	if (this) {
		BDWeapon->SetVisibility(false); //�޽� �Ⱥ��̰� �ϱ�
	}
}

void ABDThor::visibleWeapon()
{
	if (this) {
		BDWeapon->SetVisibility(true); //�޽� �Ⱥ��̰� �ϱ�
		UE_LOG(LogTemp, Warning, TEXT("Visible"));
	}
}

void ABDThor::BDHammerThrowHit()
{
	FTransform t = GetMesh()->GetSocketTransform(TEXT("BDMjolnirHand"));
	ABDThorMjolnir* Mjolnir = GetWorld()->SpawnActor<ABDThorMjolnir>(MjolnirFactory, t);

	if (Mjolnir)
	{
		//FVector LaunchDirection = (fsm->Target->GetActorLocation() - t.GetLocation()).GetSafeNormal();
		FVector LaunchDirection = GetActorForwardVector();
		Mjolnir->FireInDirection(LaunchDirection);
		UE_LOG(LogTemp, Warning, TEXT("Mjolnir spawned successfully and fired"));
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


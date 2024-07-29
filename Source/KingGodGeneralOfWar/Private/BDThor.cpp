// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThor.h"
#include "BDThorFSM.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kratos.h"

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
	//BDWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BDWeapon"));
	//BDWeapon->SetupAttachment(GetMesh(), FName("BDMjolnirHips"));
	//BDWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision); //�ӽ� �ڵ�

	//ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("/Script/Engine.StaticMesh'/Game/Bada/Asset/Model/weapon/BDMjolnir.BDMjolnir'"));
	//if (WeaponMesh.Succeeded()) {
	//	BDWeapon->SetStaticMesh(WeaponMesh.Object);

	//}

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
	//BDWeapon->SetVisibility(true);

	if (GetWorld()) {
		CurrentWeapon = Cast <ABDThorMjolnir>(GetWorld()->SpawnActor<ABDThorMjolnir>(WeaponClass));
		if (CurrentWeapon) {
			CurrentWeapon->MjolnirMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("BDMjolnirHips"));
		}
	}
	
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
		CurrentWeapon->MjolnirMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("BDMjolnirHand"));
	}
}

void ABDThor::BDHammerThrowHit()
{
	FTransform t = GetMesh()->GetSocketTransform(TEXT("BDMjolnirHand"));
	ABDThorMjolnir* Mjolnir = GetWorld()->SpawnActor<ABDThorMjolnir>(MjolnirFactory, t);

	if (Mjolnir)
	{
		// �ʱ� �ӵ� �� ���� ����
		//Ÿ������ �������� �׳� ������ ���ư����� ����
		//FVector LaunchDirection = (fsm->Target->GetActorLocation() - t.GetLocation()).GetSafeNormal();
		FVector LaunchDirection = GetActorForwardVector();
		Mjolnir->FireInDirection(LaunchDirection);
		UE_LOG(LogTemp, Log, TEXT("Mjolnir spawned successfully and fired"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Mjolnir spawned failed"));
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorMjolnir.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kratos.h"
#include "BDThor.h"
#include "BDThorFSM.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABDThorMjolnir::ABDThorMjolnir()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�浹 ����
	MjoCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MjoCol"));
	SetRootComponent(MjoCol);
	MjoCol->SetCollisionProfileName(FName("BDThorMjonlr"));

	//�޽�����
	MjolnirMesh = CreateDefaultSubobject<UStaticMeshComponent>("MjolnirMesh");
	MjolnirMesh->SetupAttachment(RootComponent);
	//�޽� ����
	ConstructorHelpers::FObjectFinder <UStaticMesh> MeshComp(TEXT("/Script/Engine.StaticMesh'/Game/Bada/Asset/Model/weapon/BDMjolnir.BDMjolnir'"));

	if (MeshComp.Succeeded()) {
		MjolnirMesh->SetStaticMesh(MeshComp.Object);
		MjolnirMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		MjolnirMesh->SetRelativeLocation(FVector(0, 0, -30.0f));

	}

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(MjoCol);

	//���ǵ� ����
	MovementComp->InitialSpeed = 0.0f; //������ 1500
	MovementComp->MaxSpeed = 0.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bShouldBounce = false;
	MovementComp->ProjectileGravityScale = 0.0f; 

	//���忡�� �÷��̾� ã�ƿ���
	//auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKratos::StaticClass());
	////AKratos Ÿ������ ĳ����
	//Target = Cast<AKratos>(actor);
	////���� ��ü ��������
	//me = Cast<ABDThor>(GetOwner());

}

// Called when the game starts or when spawned
void ABDThorMjolnir::BeginPlay()
{
	Super::BeginPlay();
	
	//OnOverlap �̺�Ʈ ����
	MjoCol->OnComponentBeginOverlap.AddDynamic(this, &ABDThorMjolnir::OnOverlap);
}

// Called every frame
void ABDThorMjolnir::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� ����
	/*float Speed = 1000.0f;
	FVector Dir = GetActorForwardVector();
	FVector velo = Dir * Speed;
	SetActorLocation(GetActorLocation() + velo * DeltaTime);*/

	//float Speed = 500.0f;
	////�÷��̾ ���ؼ� ���󰡰� �ϱ�
	//FVector dir = Target->GetActorLocation() - me->GetActorLocation();
	//dir.Normalize();
	//FVector velo = dir * Speed;
	//SetActorLocation(GetActorLocation() + velo * DeltaTime);

	// 2�� �� �ڵ� ����
	//SetLifeSpan(2.0f);
}

void ABDThorMjolnir::FireInDirection(const FVector& ShootDirection)
{

	// MjoCol�� MjolnirMesh�� -90���� ȸ����Ŵ
	FRotator NewRotation = FRotator(0, -90, 90);
	MjoCol->SetRelativeRotation(NewRotation);
	MjolnirMesh->SetRelativeRotation(NewRotation);

	MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;

}

//���ϸ� �߻� �� �ٽ� �տ� ���ƿ��� �Լ�
void ABDThorMjolnir::BackMjolnir()
{
}

//���ϸ��� �ε����� �� ������ �Լ�
void ABDThorMjolnir::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Mjolnir BeginOverlap"));

	if (OtherActor && (OtherActor != this) && OtherComp) {
		//������ ó��
		auto* AttackTarget = Cast<AKratos>(OtherActor); //Ÿ���϶�
		if (AttackTarget) {
			//���� ���� ������
			AttackTarget->Damage(BDThrowDamage, EAttackType::Attack1);
			UE_LOG(LogTemp, Warning, TEXT("Kratos Attack!!"));
		}
	}
}


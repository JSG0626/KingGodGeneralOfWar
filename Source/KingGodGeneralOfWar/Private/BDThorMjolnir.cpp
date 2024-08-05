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
	MovementComp->InitialSpeed = 2000.0f; //������ 1500
	MovementComp->MaxSpeed = 2000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bShouldBounce = false;
	MovementComp->ProjectileGravityScale = 0.0f;

	//���忡�� �÷��̾� ã�ƿ���
	//auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKratos::StaticClass());
	////AKratos Ÿ������ ĳ����
	//Target = Cast<AKratos>(actor);
	////���� ��ü ��������

	//me = Cast<ABDThor>(GetOwner());
	Thor = nullptr; // null�� �ʱ�ȭ�ϰ� BeginPlay���� ����

}

// Called when the game starts or when spawned
void ABDThorMjolnir::BeginPlay()
{
	Super::BeginPlay();

	//OnOverlap �̺�Ʈ ����
	MjoCol->OnComponentBeginOverlap.AddDynamic(this, &ABDThorMjolnir::OnOverlap);

	// ���� ��ü ��������
	//me = Cast<ABDThor>(GetOwner());
	Thor = Cast<ABDThor>(UGameplayStatics::GetActorOfClass(GetWorld(), ABDThor::StaticClass()));

	if (Thor) {
		UE_LOG(LogTemp, Warning, TEXT("Thor found in BeginPlay"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Thor is null in BeginPlay"));
	}

	bReturning = false;
	bCreateTrue = false;
}

// Called every frame
void ABDThorMjolnir::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//���ư��� ��
	if (!bReturning && bCreateTrue) {
		MovementComp->Velocity = Thor->GetActorForwardVector() * MovementComp->InitialSpeed;
	}
	//��ġ ���ƿ�
	else if (bReturning && bCreateTrue)
	{
		FVector HandLocation = Thor->GetMesh()->GetSocketLocation(TEXT("BDMjolnirHand"));
		FVector CurrentLocation = GetActorLocation();
		FVector Direction = (HandLocation - CurrentLocation).GetSafeNormal();

		//SetActorLocation(GetActorLocation() + Direction * Speed * GetWorld()->DeltaTimeSeconds);

		MovementComp->Velocity = Direction * MovementComp->InitialSpeed;
		//UE_LOG(LogTemp, Warning, TEXT("call tick"));


		float Distance = FVector::Dist(GetActorLocation(), HandLocation);

		if (Distance < 200.0f) // �Ÿ� üũ
		{
			bReturning = false;
			bCreateTrue = false;
			this->Destroy(); // �ڽ��� �ı�
			UE_LOG(LogTemp, Warning, TEXT("Destroy"));
			Thor->EquipWeapon(); // ���� �ٽ� ���̰� �ϱ�
			UE_LOG(LogTemp, Warning, TEXT("Mjolnir Back"));

		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("bReturning: %s, bCreateTrue : %s "), bReturning ? TEXT("true") : TEXT("false"), bCreateTrue ? TEXT("true") : TEXT("false"));

}

void ABDThorMjolnir::FireInDirection(const FVector& ShootDirection)
{
	//���ϸ��� ���� �������� ���� �����϶���
	if (!bCreateTrue) { 
		// MjoCol�� MjolnirMesh�� -90���� ȸ����Ŵ
		FRotator NewRotation = FRotator(90, -90, 90);
		MjoCol->SetRelativeRotation(NewRotation);
		MjolnirMesh->SetRelativeRotation(NewRotation);

		Thor->HiddenWeapon();
		bCreateTrue = true;
	}

}

//���ϸ� �߻� �� �ٽ� �տ� ���ƿ��� �Լ�
void ABDThorMjolnir::BackMjolnir()
{
	UE_LOG(LogTemp, Warning, TEXT("BackMjolnir called"));
	bReturning = true;
	UE_LOG(LogTemp, Warning, TEXT("bReturing : %s"), bReturning ? TEXT("true") : TEXT("false"));
}

//���ϸ��� �ε����� �� ������ �Լ�
void ABDThorMjolnir::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mjolnir BeginOverlap"));
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//������ ó��
		auto* AttackTarget = Cast<AKratos>(OtherActor); //Ÿ���϶�
		if (AttackTarget) {
			AttackTarget->Damage(10, EHitType::STAGGER, false);
			//UE_LOG(LogTemp, Warning, TEXT("Kratos Attack!!"));
		}
	}
}


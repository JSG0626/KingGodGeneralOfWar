// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorFSM.h"
#include "BDThor.h"
#include "Kratos.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UBDThorFSM::UBDThorFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UBDThorFSM::BeginPlay()
{
	Super::BeginPlay();

	//���忡�� �÷��̾� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AKratos::StaticClass());
	//AKratos Ÿ������ ĳ����
	Target = Cast<AKratos>(actor);
	//ACharacter* tmp = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	/*if (tmp) {
		Target = tmp;
	}*/
	//���� ��ü ��������
	me = Cast<ABDThor>(GetOwner());

	//ü�� ����
	BDCurrentHP = BDMaxHp;

	
}


// Called every frame
void UBDThorFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState) {
		case BDThorGeneralState :: BDIdle:
			BDIdleState();
			break;
		case BDThorGeneralState :: BDMove:
			BDMoveState();
			break;
		case BDThorGeneralState::BDAttack:
			BDAttackState();
			break;
		case BDThorGeneralState::BDAvoidance:
			BDAvoidanceState();
			break;
		case BDThorGeneralState :: BDDamage:
			BDDamageState();
			break;
	}

	//����â�� ���� �޼��� ���
	FString logMessege = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMessege);
}

//���� ���� �Լ�
void UBDThorFSM::BDIdleState()
{
	//�ð��� �帣�鼭
	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	//���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (BDCurrentTime > BDidleDelayTime) {
		//3. �̵� ���·� ��ȯ�ϰ� �ʹ�.
		mState = BDThorGeneralState::BDMove;
		//��� �ð� �ʱ�ȭ
		BDCurrentTime = 0;
	}
}

void UBDThorFSM::BDMoveState()
{
	//�⺻�����δ� �÷��̾ ���� �̵�������
	//�ǰ� ���°� �Ǿ��ų� ������ �ϱ� ���� �÷��̾ �ٶ󺸸� �ڷ� �̵��ϰų� ������ ȸ�Ǹ� �Ѵ�.

	//�⺻ �̵� ����
	//1. Ÿ�� �������� �ʿ��ϴ�.
	FVector destination = Target->GetActorLocation();
	//2. ������ �ʿ��ϴ�.
	FVector dir = destination - me->GetActorLocation();
	//3. �������� �̵��ϰ� �ʹ�.
	me->AddMovementInput(dir.GetSafeNormal());

	//Ÿ��� ��������� ���� ���·� ��ȯ�ϰ� �ʹ�.
	//1. ���� �Ÿ��� ���� ���� �ȿ� ������
	if (dir.Size() < BDAttackRange) {
		//2. ���� ���·� ��ȯ�ϰ� �ʹ�.
		//���� �� ��üȭ
		mState = BDThorGeneralState::BDAttack;
	}
}

void UBDThorFSM::BDAvoidanceState()
{
	//ȸ�� ����
	//�÷��̾��� ������ �������� �ڷ� ���ϰų� ������ ���Ѵ�.
}



void UBDThorFSM::BDAttackState()
{
	//�����ð��� �پ��� �������� �Ѿ���Ѵ�.
	//�ϴ� ���� ����
	//1. �ð��� �帣��
	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	//2. ���� �ð��� �Ǹ�
	if (BDCurrentTime > BDAttackDelayTime) {
		//����, ���⿣ �پ��� ���� Ÿ���� �������� �����صд�.
 		UE_LOG(LogTemp, Warning, TEXT("player BDAttack!!"));
		//��� �ð� �ʱ�ȭ �� ��� �Ǵ� �̵����� ���� �����ϱ�
		BDCurrentTime = 0;

		//mState = BDThorGeneralState::BDIdle;  // ���¸� ���� ��ȯ
	}

	//Ÿ���� ������ ������ �ٽ� �̵����� ���ϰ� �ʹ�.

	//�ӽ� �ڵ�, �÷��̾��� Damage�� �߻����� ��� ���ʹ��� ���¸� ��� �Ǵ� �̵����� ����
	float distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());
	//Ÿ���� �Ÿ��� ���� ������ ������ϱ�
	if (distance > BDAttackRange) {
		//���¸� ��� ���� ��ȯ�Ѵ�.
		mState = BDThorGeneralState::BDMove;
	}

}

void UBDThorFSM::BDDamageState()
{
	//�ǰ� �ִϸ��̼� ���

	Damage(10); //������ 


	//ü���� �����ִٸ�
	if (BDCurrentHP > 0) {
		//���¸� ȸ�Ƿ� ��ȯ
		mState = BDThorGeneralState::BDMove;
	}
	else if (BDCurrentHP <= 0) {
		//2 ������� ��ȯ
	}
}

void UBDThorFSM::BDEndState()
{

}

void UBDThorFSM::SetState()
{
}

//�������� ���� �� �߻��ϴ� �Լ�
void UBDThorFSM::Damage(float DamageNum)
{
	BDCurrentHP -= DamageNum;
	UE_LOG(LogTemp, Warning, TEXT("BDThor damage!"));

}


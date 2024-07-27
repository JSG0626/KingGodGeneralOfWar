// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorFSM.h"
#include "BDThor.h"
#include "Kratos.h"
#include "BDThorAnim.h"
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
	//���� ��ü ��������
	me = Cast<ABDThor>(GetOwner());

	//�ִϸ��̼� �Ҵ�
	anim = Cast<UBDThorAnim>(me->GetMesh()->GetAnimInstance());

	//ü�� ����
	BDCurrentHP = BDMaxHp;


}


// Called every frame
void UBDThorFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState) {
	case BDThorGeneralState::BDIdle:
		BDIdleState(); //���
		break;
	case BDThorGeneralState::BDMove:
		BDMoveState(); //������
		break;
	case BDThorGeneralState::BDAttackModeChange:
		BDAttackModeChangeState(); //���� ���� ���� �Լ�
		break;
	case BDThorGeneralState::BDAvoidance:
		BDAvoidanceState(); //ȸ��
		break;
	case BDThorGeneralState::BDDamage:
		BDDamageState(); //�ǰ�
		break;
	case BDThorGeneralState::BDHammerThunder:
		BDHammerThunderState(); //���� ����
		break;
	case BDThorGeneralState::BDHammerWind:
		BDHammerWindState(); //���� ����
		break;
	case BDThorGeneralState::BDHammerThreeSwing:
		BDHammerThreeSwingState(); //���� ����
		break;
	case BDThorGeneralState::BDGiveUPFly:
		BDGiveUPFlyState(); //���� ����
		break;
	case BDThorGeneralState::BDHitDown:
		BDHittingDownState(); //���� ����
		break;
	}

	//����â�� ���� �޼��� ���
	FString logMessege = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMessege);
}

//�������� ���� �� �߻��ϴ� �Լ�
void UBDThorFSM::Damage(float DamageNum)
{
	BDCurrentHP -= DamageNum;
	UE_LOG(LogTemp, Warning, TEXT("BDThor damage!"));

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

		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
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
		mState = BDThorGeneralState::BDAttackModeChange;

		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		//���� �ִϸ��̼� ��� Ȱ��ȭ
		
		//���� ���� ��ȯ �� ���ð��� �ٷ� �������� ó��
	}
}

void UBDThorFSM::BDAvoidanceState()
{
	//ȸ�� ����
	//�÷��̾��� ������ �������� �ڷ� ���ϰų� ������ ���Ѵ�.
}



void UBDThorFSM::BDAttackModeChangeState()
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

//��ġ �����鼭 ���� ����
void UBDThorFSM::BDHammerThunderState()
{
}

//��ġ �ֵθ��鼭 �ٶ�����
void UBDThorFSM::BDHammerWindState()
{
}

//��ġ n�� �ֵθ���
void UBDThorFSM::BDHammerThreeSwingState()
{
}

//�÷��̾� ��Ƽ� ����ġ�� (ī�޶� �ƾ�)
void UBDThorFSM::BDGiveUPFlyState()
{
}

//�ٴ��� �ָ����� ����ġ��
void UBDThorFSM::BDHittingDownState()
{
}




//�ִϸ��̼� ������ ���� �� ���� Ȯ��
void UBDThorFSM::BDEndState()
{
}

//�ִϸ��̼� ���� ����
void UBDThorFSM::BDSetState()
{
}




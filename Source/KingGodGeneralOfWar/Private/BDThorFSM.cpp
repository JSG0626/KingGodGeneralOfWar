// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorFSM.h"
#include "BDThor.h"
#include "Kratos.h"
#include "BDThorAnim.h"
#include "Kismet/GameplayStatics.h"
#include "BDThorMjolnir.h"


// Sets default values for this component's properties
UBDThorFSM::UBDThorFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

	LastAttackState = BDThorGeneralState::BDIdle; // �ʱ�ȭ
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

	/*if (anim) {
		anim->OnMontageEnded.AddDynamic(this, &UBDThorFSM::BDEndState);
	}*/

}

// Called when the montage ends
//void UBDThorFSM::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
//{
//	if (Montage == anim->BDHammerThrowMontage || Montage == anim->BDHammerWindMontage || Montage == anim->BDHammerThreeSwingMontage) {
//		BDEndState();
//	}
//}


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
	case BDThorGeneralState::BDHammerThrow:
		BDHammerThrowState(); //���� ����
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
	//UE_LOG(LogTemp, Warning, TEXT("BDThor damage!"));

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

	//1. �ð��� �帣��
	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	//2. ���� �ð��� �Ǹ�
	if (BDCurrentTime > BDAttackDelayTime) {
		// �������� ���� ���� ����
		mState = RandomAttackState();
		//mState = BDThorGeneralState::BDHammerWind;  // �ӽ� ����
		anim->animState = mState;
		
		UE_LOG(LogTemp, Warning, TEXT("AttackModeChangeState: %s"), *UEnum::GetValueAsString(mState));
		
		BDCurrentTime = 0;
		
	}

	//Ÿ���� ������ ������ �ٽ� �̵����� ���ϰ� �ʹ�.

	////�ӽ� �ڵ�, �÷��̾��� Damage�� �߻����� ��� ���ʹ��� ���¸� ��� �Ǵ� �̵����� ����
	//float distance = FVector::Distance(Target->GetActorLocation(), me->GetActorLocation());
	////Ÿ���� �Ÿ��� ���� ������ ������ϱ�
	//if (distance > BDAttackRange) {
	//	//���¸� ��� ���� ��ȯ�Ѵ�.
	//	mState = BDThorGeneralState::BDIdle;
	//	//�ִϸ��̼� ���� ����ȭ
	//	anim->animState = mState;
	//}

}

//���� ������ �������� �����ϴ� �Լ�
BDThorGeneralState UBDThorFSM::RandomAttackState()
{
	// ������ ���µ��� �迭�� ����
	TArray<BDThorGeneralState> AttackStates = {
		BDThorGeneralState::BDHammerThrow,
		BDThorGeneralState::BDHammerWind,
		//BDThorGeneralState::BDHammerThreeSwing,
		//BDThorGeneralState::BDGiveUPFly,
		//BDThorGeneralState::BDHitDown
	};

	// ������ ���� ����
	AttackStates.Remove(LastAttackState);

	// �������� ����
	int32 RandomIndex = FMath::RandRange(0, AttackStates.Num() - 1);
	BDThorGeneralState NewState = AttackStates[RandomIndex];

	//���� ��ġ ������ ���
	if (NewState == BDThorGeneralState::BDHammerThreeSwing || NewState == BDThorGeneralState::BDHammerThrow || NewState == BDThorGeneralState::BDHammerWind) {
		//�տ� ��ġ�� ����
		me->EquipWeapon();
	}
	else {
		me->DrawWeapon(); //�㸮�� ��ġ�� �ξ��
	}

	// ������ ���� ������Ʈ
	LastAttackState = NewState;
	UE_LOG(LogTemp, Warning, TEXT("Random!!"));
	return NewState; //���� ����
}

void UBDThorFSM::BDDamageState()
{
	//�ǰ� �ִϸ��̼� ���

	Damage(10); //������ 


	//ü���� �����ִٸ�
	if (BDCurrentHP > 0) {
		//���¸� ȸ�Ƿ� ��ȯ
		//mState = BDThorGeneralState::BDMove;
	}
	else if (BDCurrentHP <= 0) {
		//2 ������� ��ȯ
	}
}


//��ġ �����鼭 ���� ����
void UBDThorFSM::BDHammerThrowState()
{
	// ���� ���°� �̹� Hammer Wind��� �Լ� ȣ���� �ǳʶڴ�.
	if (mState != BDThorGeneralState::BDHammerThrow) {
		return;
	}

	if (!anim->Montage_IsPlaying(anim->BDHammerThrowMontage)) {
		anim->playBDHammerThrow();
	}

	//mState = BDThorGeneralState::BDDamage;
	
}

//��ġ �ֵθ��鼭 �ٶ�����
void UBDThorFSM::BDHammerWindState()
{
	// ���� ���°� �̹� Hammer Wind��� �Լ� ȣ���� �ǳʶڴ�.
	if (mState != BDThorGeneralState::BDHammerWind) {
		return;
	}

	// �ִϸ��̼��� ����ǰ� �ִ��� Ȯ���ϰ�, ������� �ʾҴٸ� ����Ѵ�.
	if (!anim->Montage_IsPlaying(anim->BDHammerWindMontage))
	{
		anim->playBDHammerWind();
		UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Wind Animation"));
	}

	//mState = BDThorGeneralState::BDDamage;

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
	//���� ���� ���¶��
	if (mState == BDThorGeneralState::BDHammerThrow || mState == BDThorGeneralState::BDHammerThreeSwing || mState == BDThorGeneralState::BDHammerWind || mState == BDThorGeneralState::BDGiveUPFly || mState == BDThorGeneralState::BDHitDown) {
		//move�� ���ư���?
		BDSetState(BDThorGeneralState::BDMove);
		
		//GetWorld()->GetTimerManager().SetTimer(StateTimerHandle, this, &UBDThorFSM::BDTransitionToMoveState, 1.0f, false);
		UE_LOG(LogTemp, Warning, TEXT("End of Attack Animation, switching to Move after delay"));
	}
	//else if(mState == BDThorGeneralState::)
}

void UBDThorFSM::BDTransitionToMoveState()
{
	BDSetState(BDThorGeneralState::BDMove);
}

//�ִϸ��̼� ���� ����
void UBDThorFSM::BDSetState(BDThorGeneralState BDnewState)
{
	mState = BDnewState; //���� ����
	anim->animState = mState;
	UE_LOG(LogTemp, Warning, TEXT("State changed to: %s"), *UEnum::GetValueAsString(mState));
}




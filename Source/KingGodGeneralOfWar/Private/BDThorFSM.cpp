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


	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	if (BDCurrentTime > BDDelayTime) {
		mState = BDThorGeneralState::BDAttackModeChange;

		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		BDCurrentTime = 0;
	}

	//Ÿ��� ��������� ���� ���·� ��ȯ�ϰ� �ʹ�.
	//1. ���� �Ÿ��� ���� ���� �ȿ� ������
	//if (dir.Size() < BDAttackRange) {
	//	//2. ���� ���·� ��ȯ�ϰ� �ʹ�.
	//	//���� �� ��üȭ
	//	mState = BDThorGeneralState::BDAttackModeChange;

	//	//�ִϸ��̼� ���� ����ȭ
	//	anim->animState = mState;
	//}
}


void UBDThorFSM::BDAttackModeChangeState()
{
	//�����ð��� �پ��� �������� �Ѿ���Ѵ�.

	//�켱�� �ִϸ��̼��� ��� or ������ or ȸ�� �ִϸ��̼��� ����Ѵ�.
	//�켱�� ��� ���� �ִϸ��̼� ������� ����
	anim->playBDAttackRandomState();

	//1. �ð��� �帣��
	BDCurrentTime += GetWorld()->DeltaTimeSeconds;
	//2. ���� �ð��� �Ǹ�
	if (BDCurrentTime > BDAttackDelayTime) {
		// �������� ���� ���� ����
		mState = RandomAttackState();
		//mState = BDThorGeneralState::BDHammerWind;  // �ӽ� ����
		anim->animState = mState;
		
		//UE_LOG(LogTemp, Warning, TEXT("AttackModeChangeState: %s"), *UEnum::GetValueAsString(mState));
		
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
	//�ִϸ��̼� ���´� �켱 ��Ÿ�ָ� �����Ѵ�
	
	// ������ ���µ��� �迭�� ����
	TArray<BDThorGeneralState> AttackStates = {
		BDThorGeneralState::BDHammerThrow,
		BDThorGeneralState::BDHammerWind,
		BDThorGeneralState::BDHammerThreeSwing,
		//BDThorGeneralState::BDGiveUPFly,
		//BDThorGeneralState::BDHitDown
	};

	// ������ ���� ����
	AttackStates.Remove(LastAttackState);

	// �������� ����
	int32 RandomIndex = FMath::RandRange(0, AttackStates.Num() - 1);
	BDThorGeneralState NewState = AttackStates[RandomIndex];

	//���� ��ġ ������ ���
	if (NewState == BDThorGeneralState::BDHammerThrow || NewState == BDThorGeneralState::BDHammerWind) {
		//�տ� ��ġ�� ����
		me->EquipWeapon();
	}
	else if(NewState == BDThorGeneralState::BDHammerThreeSwing) {
		me->EquipRight(); //�����տ� ��ġ�� ����
	}
	else {
		me->DrawWeapon(); //�㸮�� ��ġ�� �ξ��
	}

	// ������ ���� ������Ʈ
	LastAttackState = NewState;
	//UE_LOG(LogTemp, Warning, TEXT("Random!!"));
	return NewState; //���� ����
}

//�������� ���� �� �߻��ϴ� �Լ�
void UBDThorFSM::Damage(float DamageNum)
{
	BDCurrentHP -= DamageNum;

	//�ǰ� ���·� �����Ѵ�.
	mState = BDThorGeneralState::BDDamage;

	UE_LOG(LogTemp, Warning, TEXT("BDThor damage!"));

}

void UBDThorFSM::BDDamageState()
{
	if (!anim->Montage_IsPlaying(anim->BDThorDamageMontage)) {
		UE_LOG(LogTemp, Warning, TEXT("Damage"));
		//�ǰ� �ִϸ��̼� ���
		anim->playBDDamage();
	}

	//UE_LOG(LogTemp, Warning, TEXT("DamageState"));
	
}

void UBDThorFSM::BDAvoidanceState()
{
	if (!anim->Montage_IsPlaying(anim->BDThorRightDodgeMontage)) {
		//�������� �޾��� �� ȸ��
		UE_LOG(LogTemp, Warning, TEXT("Avoidance"));
		anim->playBDRightDodge();
	}

	//���� ��ó ��ü ��ġ�� �޾Ƽ� ���������� ȸ���� ������ �������� ȸ���� �������� ���� �ڵ� �ۼ�

	//�÷��̾��� ������ �������� �ڷ� ���ϰų� ������ ���Ѵ�.
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

	
}

//��ġ �ֵθ��鼭 �ٶ�����
void UBDThorFSM::BDHammerWindState()
{
	if (me->IsWeaponHold == false) {
		me->EquipWeapon(); //�켱 ���⸦ ���.
	}

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

}

//��ġ n�� �ֵθ���
void UBDThorFSM::BDHammerThreeSwingState()
{
	if (!anim->Montage_IsPlaying(anim->BDHammerThreeSwingMontage))
	{
		anim->playBDHammerThreeSwing(); //3�� �ֵθ���
		UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Three Swing Animation"));
	}
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
// ������, ���, �̵��� EndState���� �������� ���櫓
void UBDThorFSM::BDEndState()
{
	UE_LOG(LogTemp, Warning, TEXT("End1"));

	//���� ���� ���� ���¶��
	if (mState == BDThorGeneralState::BDGiveUPFly || mState == BDThorGeneralState::BDHitDown) {
		//�÷��̾� ��ó�� �ֱ� ������ �ϴ� ȸ�� ����
		BDSetState(BDThorGeneralState::BDMove);
		
		//UE_LOG(LogTemp, Warning, TEXT("End of Attack Animation, switching to Move after delay"));
	}
	//��ġ�� �� ���� ���� ���¿����ٸ�
	else if (mState == BDThorGeneralState::BDHammerThreeSwing) {
		if (me->IsWeaponHold == false) {
			me->DrawWeapon(); //���⸦ ������.
		}
		BDSetState(BDThorGeneralState::BDMove);
	}
	//��ġ�� �� ���Ÿ� ���� ���¶��
	else if (mState == BDThorGeneralState::BDHammerThrow || mState == BDThorGeneralState::BDHammerWind) {
		BDSetState(BDThorGeneralState::BDMove);
	}
	//ȸ�Ǹ� ���´ٸ�
	else if (mState == BDThorGeneralState::BDAvoidance) {
		UE_LOG(LogTemp, Warning, TEXT("EndAvoidance"));
		BDSetState(BDThorGeneralState::BDMove); //������ ���� ���� �������� ������
	}
	//�������� �޾Ҵٸ�
	else if (mState == BDThorGeneralState::BDDamage) {
		UE_LOG(LogTemp, Warning, TEXT("EndDamage"));

		//ü���� �����ִٸ�
		if (BDCurrentHP > 0) {
			//���¸� ȸ�Ƿ� ��ȯ
			BDSetState(BDThorGeneralState::BDAvoidance);
		}
		else if (BDCurrentHP <= 0) {
			//2 ������� ��ȯ
		}
	}
}


//�ִϸ��̼� ���� ����
void UBDThorFSM::BDSetState(BDThorGeneralState BDnewState)
{
	mState = BDnewState; //���� ����
	anim->animState = mState;
	//UE_LOG(LogTemp, Warning, TEXT("State changed to: %s"), *UEnum::GetValueAsString(mState));
}




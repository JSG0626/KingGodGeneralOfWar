// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BDThorFSM.generated.h"

//��ü ��� ����
//�ǰ��� ����̾� / Attack, Move�� ���������� ����
UENUM(BlueprintType)
enum class BDThorGeneralState : uint8 {
	BDIdle,
	BDMove,
	BDAttack,
	BDDamage,
};

//���� ���� ���� ������
UENUM(BlueprintType)
enum class BDThorAttack : uint8 {
	BDHammerThunder,
	BDHammerWind,
	BDHammerThreeSwing,
	BDGiveUPFly,
	BDHittingDown,
};

//�̵� ���� ���� ������
UENUM(BlueprintType)
enum class BDThorMove : uint8 {
	BDBackMove,
	BDFrontMove,
	BDRightMove,
	BDLeftMove,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGGODGENERALOFWAR_API UBDThorFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBDThorFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(EditAnywhere)
	class ABDThor* me;

	UPROPERTY(EditAnywhere)
	class AKratos* Target;

	//���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	BDThorGeneralState mState = BDThorGeneralState::BDIdle;


	//���� �Ѱ� �Լ�
	void BDIdleState(); //��� ����
	void BDMoveState(); //������ ����
	void BDAttackState(); //���� ����
	void BDDamageState(); //�ǰ� ����

		
};

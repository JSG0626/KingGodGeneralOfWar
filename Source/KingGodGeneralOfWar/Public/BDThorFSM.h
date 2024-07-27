// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BDThorMjolnir.h"
#include "BDThorFSM.generated.h"

//��ü ��� ����
//�ǰ��� ����̾� / Attack, Move�� ���������� ����
//����, ȸ�ǵ� �̰����� ����
UENUM(BlueprintType)
enum class BDThorGeneralState : uint8 {
	BDIdle,
	BDMove,
	BDAvoidance,
	BDAttackModeChange,
	BDDamage,
	BDHammerThunder,
	BDHammerWind,
	BDHammerThreeSwing,
	BDGiveUPFly,
	BDHitDown,
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
	class ABDThor* me; //�� �ڽ�


	UPROPERTY(EditAnywhere, Category = FSM) 
	float BDAttackRange = 150.0f; // ���ʹ� ���� ����
	

	UPROPERTY(EditAnywhere)
	//class AKratos* Target; //Ÿ��
	class ACharacter* Target;

	//���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	BDThorGeneralState mState = BDThorGeneralState::BDIdle;


	// �� ���� �Ѱ� �Լ�
	UFUNCTION(BlueprintCallable, Category = State)
	void BDIdleState(); //��� ����
	UFUNCTION(BlueprintCallable, Category = State)
	void BDMoveState(); //������ ����
	UFUNCTION(BlueprintCallable, Category = State)
	void BDAvoidanceState(); //ȸ�� ����
	UFUNCTION(BlueprintCallable, Category = State)
	void BDAttackModeChangeState(); //���� ��� ���� ����
	UFUNCTION(BlueprintCallable, Category = State)
	void BDDamageState(); //�ǰ� ����

	//���� �Լ�
	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerThunderState(); //��ġ �����鼭 ���� ����
	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerWindState(); //��ġ �ֵθ��鼭 �ٶ������� ����
	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerThreeSwingState(); //��ġ n�� �ֵθ���

	//���� �ƾ� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	void BDGiveUPFlyState();
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	void BDHittingDownState();


	//������� �ִϸ��̼� �������Ʈ
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Anim)
	class UBDThorAnim* anim;

	//�ִϸ��̼� �Ѱ��� �Լ�, ��Ƽ���̸� �־ �� �Լ��� ȣ���Ѵ�. �� �Լ����� �̿��� ������Ʈ�� �����Ѵ�.
	UFUNCTION(BlueprintCallable, Category = SetState)
	void BDEndState();

	UFUNCTION(BlueprintCallable, Category = SetState)
	void BDSetState();

	//��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float BDidleDelayTime = 2.0f;
	//��� �ð�
	float BDCurrentTime = 0;

	//���� ��� �ð�, �丣�� �����ϴ� ������ �ð��� �پ��ϴ� ���� ����
	UPROPERTY(EditAnywhere, Category = FSM)
	float BDAttackDelayTime = 2.0f;

	//�ǰ� �� ������ �Լ�, DamageNum ������ ��ġ
	UFUNCTION()
	void Damage(float DamageNum);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HP)
	float BDMaxHp = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HP)
	float BDCurrentHP;
		
};

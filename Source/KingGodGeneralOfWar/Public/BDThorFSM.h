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
	BDBackDodge,
	BDAttackModeChange,
	BDDamage,
	BDHammerThrow,
	BDHammerWind,
	BDHammerThreeSwing,
	BDGiveUPFly,
	BDHitDown,
	BDClap,
	BDKick,
	BDRandomChange,
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


	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float BDAttackRange = 300.0f; // ���ʹ� ���� ����
	

	UPROPERTY(EditDefaultsOnly)
	class AKratos* Target; //Ÿ��
	//class ACharacter* Target;

	//���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	BDThorGeneralState mState = BDThorGeneralState::BDIdle;


	// �� ���� �Ѱ� �Լ�
	UFUNCTION(BlueprintCallable, Category = State)
	void BDIdleState(); //��� ����
	UFUNCTION(BlueprintCallable, Category = State)
	void BDMoveState(); //������ ����
	UFUNCTION(BlueprintCallable, Category = State)
	void BDAvoidanceState(); //������ ȸ�� ����
	UFUNCTION(BlueprintCallable, Category = State)
	void BDBackDodgeState(); //�ڷ� ȸ�� ����
	UFUNCTION(BlueprintCallable, Category = State)
	void BDAttackModeChangeState(); //���� ��� ���� ����
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	BDThorGeneralState RandomAttackState(); // ���� ���� ���� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = State)
	void BDRandomChangeState(); //���� ���� ���� ���� -> ���, �ڷ� ȸ��, ������ ȸ��, ���� ���� ������ ����.
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	BDThorGeneralState RandomChange(); // ���� ���� ���� ���� �Լ�

	UFUNCTION(BlueprintCallable, Category = State)
	void BDDamageState(); //�ǰ� ����

	//���� �Լ�
	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerThrowState(); //��ġ �����鼭 ����
	//UFUNCTION(BlueprintCallable, Category = Attack)
	//void BDHammerThrowHit(); //��ġ�� Fire �ϴ� �ɷ�

	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerWindState(); //��ġ �ֵθ��鼭 �ٶ������� ����
	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerThreeSwingState(); //��ġ n�� �ֵθ���

	//���� �ƾ� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	void BDGiveUPFlyState();
	UFUNCTION(BlueprintCallable, Category = AttackScene)
	void BDHittingDownState();

	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDClapState();

	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDKickState();



	UFUNCTION()
	void BDHitShock(); //�丣�� �ָ��� ����ġ�� ����� ����� ���� �Լ�

	UFUNCTION()
	void BDSphereOverlap(float Damage, EHitType HitType, bool IsMelee);

	UFUNCTION()
	void BDInitializeThorAreaDecal(float Radi);

	void BDClapSphereOverlap(FVector loc, float ZoneSize, float Damage, EHitType HitType, bool IsMelee);

	UFUNCTION()
	void BDClapAttack();
	UFUNCTION()
	void BDKickAttack();

	float Radius = 550.0f;

	//��ġ �ֵθ��⿡�� ���� üũ
	bool bBDAttackCheck;


	UPROPERTY(EditDefaultsOnly)
	BDThorGeneralState LastAttackState; // ������ ���� ���¸� �����ϴ� ����


	//������� �ִϸ��̼� �������Ʈ
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Anim)
	class UBDThorAnim* anim;

	//�ִϸ��̼� �Ѱ��� �Լ�, ��Ƽ���̸� �־ �� �Լ��� ȣ���Ѵ�. �� �Լ����� �̿��� ������Ʈ�� �����Ѵ�.
	UFUNCTION(BlueprintCallable, Category = SetState)
	void BDEndState();

	UFUNCTION(BlueprintCallable, Category = SetState)
	void BDSetState(BDThorGeneralState BDnewState);

	//��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float BDidleDelayTime = 1.0f;
	//��� �ð�
	float BDCurrentTime = 0;

	//���� ������ �����ϱ� ���� ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
	float BDAttackDelayTime = 0.3f;

	//���� ������ �����ϱ� ���� ��� �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
	float BDStateDelayTime = 0.2f;

	//�丣�� �����ϴ� ������ �ð��� �پ��ϴ� ���� ����
	UPROPERTY(EditAnywhere, Category = FSM)
	float BDDelayTime = 0.5f;

	//�ǰ� �� ������ �Լ�, DamageNum ������ ��ġ
	UFUNCTION()
	void Damage(float DamageNum, EAttackDirectionType AtkDir);

	//���� ���� ���
	UFUNCTION()
	void BDGetHitDirectionString(EAttackDirectionType AtkDir);

	UPROPERTY()
	FString Str; //������ ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HP)
	float BDMaxHp = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HP)
	float BDCurrentHP;


	//����Ʈ
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="VFX")
	class UNiagaraSystem* BDClapVFX; //�ڼ� VFX

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="VFX")
	class UNiagaraSystem* BDKickVFX; //������ VFX

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="VFX")
	class UNiagaraSystem* BDHitDownVFX; //�ָ����� ����ġ�� VFX

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="VFX")
	class UNiagaraSystem* BDThreeSwingVFX; //���� �ֵθ��� VFX

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess), Category="VFX")
	class UNiagaraSystem* BDEmberVFX; //����


	//BGM

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* ThreeSound1; //��ġ �ֵθ��� 1��

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* ThreeSound2; //��ġ �ֵθ��� 2��

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* ThreeSound3; //��ġ �ֵθ��� 3��

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* ClapSound; //�ڼ� ġ��

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* KickSound; //������

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* HammerThrowSound1; //��ġ ������

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* HammerThrowSound2; //��ġ ������ 2

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* WindSound; // �ٶ� ������ �Ҹ�

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* Damage1; // ������ �Ҹ� 1

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* Damage2; // ������ �Ҹ� 1

	UPROPERTY(EditDefaultsOnly)
	class USoundWave* Damage3; // ������ �Ҹ� 1


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BDThorFSM.h"
#include "BDThorAnim.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UBDThorAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BDThorFSM)
	class UBDThorFSM* bdThorFSM; //FSM

	UPROPERTY(EditDefaultsOnly, Category = BDThor)
	class ABDThor* bdThor;

	UPROPERTY(EditDefaultsOnly, Category = Mjolnir)
	class ABDThorMjolnir* Mjolnir;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	BDThorGeneralState animState; //FSM ����


	//BDHammerThrow ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* BDHammerThrowMontage;

	//BDHammerWind ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* BDHammerWindMontage;

	//�ڷ� ȸ���ϴ� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* BDThorBackDodgeMontage;

	//���������� ȸ�� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* BDThorRightDodgeMontage;

	//�ǰ� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* BDThorDamageMontage;

	//���� ���� ���� ���� �� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* BDThorAttackRandomMontage;

	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* BDHammerThreeSwingMontage;
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	class UAnimMontage* BDHitDownMontage;



	//��Ÿ�� ����
	UFUNCTION()
	void playBDHammerThrow();
	UFUNCTION()
	void playBDHammerWind();
	UFUNCTION()
	void playBDBackDodge();
	UFUNCTION()
	void playBDRightDodge();
	UFUNCTION()
	void playBDDamage();
	UFUNCTION()
	void playBDAttackRandomState();
	UFUNCTION()
	void playBDHammerThreeSwing();
	UFUNCTION()
	void playBDHitDown();



	UFUNCTION()
	void AnimNotify_AnimEnd(); //�ִϸ��̼��� ������ �� ���¸� �ٲ� ��Ƽ����

	//BDHammerThrow ��Ÿ��
	UFUNCTION()
	void AnimNotify_ThrowTiming();

	UFUNCTION()
	void AnimNotify_SlashWind();

	UFUNCTION()
	void AnimNotify_BackMjolnir();


	void BDJumpToHitSection(const FString& Section);
	UFUNCTION()
	void AnimNotify_BDHitDown();

};

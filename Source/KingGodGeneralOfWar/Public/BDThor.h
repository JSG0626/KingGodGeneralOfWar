// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BDThor.generated.h"

UCLASS()
class KINGGODGENERALOFWAR_API ABDThor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABDThor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	
	//�� ���� ������Ʈ Ŭ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
	class UBDThorFSM* fsm; //�丣 FSM

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = weapon)
	class UStaticMeshComponent* BDWeapon; //���� ������Ʈ

	//������ �� ������ ��ġ �ٲٱ�
	UFUNCTION()
	void EquipWeapon();

	bool IsWeaponHold; //���Ⱑ �տ� ������ true

	//��ġ ������ �ƴҶ� �׻� �㸮�㿡 ��ġ �ޱ�
	UFUNCTION()
	void DrawWeapon();

	UFUNCTION()
	void EquipRight();

	UFUNCTION()
	void HiddenWeapon(); //���� ��𼭵� �Ⱥ��̰� �ϱ� : ���⸦ ������ �� ȣ��
	void visibleWeapon(); //���⸦ �Ⱥ��̰� ������ �ٽ� ���̰� �ϱ� : ���� ������ �� �ڿ� ȣ��

	//�ٶ� ������ �� ���� ���� Ŭ����
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWindSlash> SlashFat;

	//��ġ ������ �� ����
	UPROPERTY(EditDefaultsOnly, Category = weapon)
	TSubclassOf<class ABDThorMjolnir> MjolnirFactory;

	UPROPERTY()
	class ABDThorMjolnir* FlyWeapon;


	UFUNCTION(BlueprintCallable, Category = weapon)
	void BDHammerThrowHit(); //��ġ�� Fire �ϴ� �ɷ�

	//�ٶ��� ������ ������ �ֱ�
	UFUNCTION(BlueprintCallable, Category = weapon)
	void BDHammerWindSlash(); //�ٶ� ������ �ɷ�


};

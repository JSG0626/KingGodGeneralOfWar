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


	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = weapon)
	class UStaticMeshComponent* BDWeapon;*/

	UPROPERTY(EditAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABDThorMjolnir> WeaponClass;

	UPROPERTY(VisibleAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ABDThorMjolnir> CurrentWeapon;

	//������ �� ������ ��ġ �ٲٱ�
	UFUNCTION()
	void EquipWeapon();

	//�ٶ� ������ �� ���� ���� Ŭ����
	/*UPROPERTY(EditAnywhere)
	TSubclassOf<class ASlashActor>SlashFat;*/

	//��ġ ������ �� ����
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABDThorMjolnir> MjolnirFactory;

	UFUNCTION(BlueprintCallable, Category = Attack)
	void BDHammerThrowHit(); //��ġ�� Fire �ϴ� �ɷ�
};

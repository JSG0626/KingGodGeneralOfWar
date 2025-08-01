// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS()
class KINGGODGENERALOFWAR_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float MaxHP = 100;
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float MaxStunGuage = 50;
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* HPUIComp ;
	UPROPERTY()
	TSubclassOf<class UEnemyHPUI> HPUIFactory ;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* LockOnUIComp;
	UPROPERTY()
	TSubclassOf<class UEnemyHPUI> LockOnUIFactory;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class ACharacter* CurrentTarget ;

	float CurHP;
	float CurStunGuage;
private:
	UPROPERTY()
	class UEnemyHPUI* HPUI ;

	UPROPERTY()
	class UUserWidget* LockOnUI ;

	

	virtual void SetHP(float Damage);
	virtual void SetStunGuage(float StunDamage);

	void DieProcess();
	void StunProcess();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Other Actor Interfaces
	UFUNCTION(BlueprintCallable)
	void ActiveLockOnUI(bool ActiveState);

	UFUNCTION(BlueprintCallable)
	virtual bool GetDamage(const struct FGenericAttackParams& params);
};

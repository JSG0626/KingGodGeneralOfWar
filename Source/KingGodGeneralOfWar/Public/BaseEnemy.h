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
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UEnemyHPUI> HPUIFactory ;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class ACharacter* CurrentTarget ;

private:
	UPROPERTY(EditDefaultsOnly)
	class UEnemyHPUI* HPUI ;
	float CurHP;
	float CurStunGuage;

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
	UFUNCTION(BlueprintImplementableEvent)
	void SetBillboardVisible(bool Visible, class UCameraComponent* Camera = nullptr);

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* PlayerCamera ;
	
	UFUNCTION(BlueprintCallable)
	virtual bool GetDamage(const struct FGenericAttackParams& params);
};

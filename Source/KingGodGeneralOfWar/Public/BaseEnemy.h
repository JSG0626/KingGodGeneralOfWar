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

	UPROPERTY(EditDefaultsOnly)
	float MaxHP = 100;
	UPROPERTY(EditDefaultsOnly)
	float MaxStunGuage = 50;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
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
	void SetBillboardVisible(bool Visible, class UCameraComponent* Camera);

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* PlayerCamera ;

	virtual bool TakeDamage(const struct FGenericAttackParams& params);
};

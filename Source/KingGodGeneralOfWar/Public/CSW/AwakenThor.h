﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "AwakenThor.generated.h"

UCLASS()
class KINGGODGENERALOFWAR_API AAwakenThor : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAwakenThor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> LightBPClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> PoundCameraShake;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class UProjectileMovementComponent* GetMjolnirMoveComp() const;
	UStaticMeshComponent* GetMjolnir() const;

	void Move(FVector NewLoc);

	UFUNCTION(BlueprintCallable)
	void ThrowForTeleport(FVector Direction);

	UFUNCTION(BlueprintCallable)
	void Teleport(FVector Target);

	void EquipWeapon();
	void DrawWeapon();

	class UAwakenThorFSM* getFSM() const;
	
	float GetHpPercent() const;
	virtual bool GetDamage(const struct FGenericAttackParams& params) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	class UAwakenThorFSM* Fsm;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UStaticMeshComponent* Mjolnir;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UCapsuleComponent* HeadCapsule;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UCapsuleComponent* BodyCapsule;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UCapsuleComponent* LArmCapsule;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UCapsuleComponent* LForeArmCapsule;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UCapsuleComponent* RArmCapsule;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UCapsuleComponent* RForeArmCapsule;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UCapsuleComponent* RLegCapsule;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UCapsuleComponent* RUpLegCapsule;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UCapsuleComponent* LLegCapsule;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UCapsuleComponent* LUpLegCapsule;
	

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	class UProjectileMovementComponent* MjolnirMoveComp;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	class UPointLightComponent* LEyeLight;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	class UPointLightComponent* REyeLight;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UPointLightComponent* LMjolnirLight;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	UPointLightComponent* RMjolnirLight;
	
	
	UFUNCTION()
	void SetThorLocation(FVector NewLoc);
};

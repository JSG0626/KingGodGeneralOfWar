// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponInterface.h"
#include "SG_Shield.generated.h"

UCLASS()
class KINGGODGENERALOFWAR_API ASG_Shield : public AActor, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASG_Shield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	
	class UArrowComponent* LightPosition;

	UFUNCTION()
	void OnShieldAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Init(class AKratos* _Me);

	virtual void ActiveHitCollision(bool active) override;
	void SetTargetScale(const bool Bigger);
private:
	UPROPERTY()
	class AKratos* Me ;

	
	const float MAX_SCALE = 0.08f;
	float TargetScale = 0.0f;
	float CurrentScale = 0.0f;
	bool bBigger = false;
	void LerpScale(float DeltaTime);
};

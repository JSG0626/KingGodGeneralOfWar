// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BDThorMjolnir.generated.h"

UCLASS()
class KINGGODGENERALOFWAR_API ABDThorMjolnir : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABDThorMjolnir();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UStaticMeshComponent> MjolnirMesh; //�޽�

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UCapsuleComponent> MjoCol; //�浹ü

	//�߻�ü ������Ʈ
	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(EditDefaultsOnly)
	class AKratos* Target; //Ÿ��


	// ���� ����, ���� ���� ����
	UPROPERTY(EditDefaultsOnly)
	class ABDThor* Thor; //�� �ڽ�

	UPROPERTY(EditDefaultsOnly)
	int32 BDThrowDamage = 10;


	UFUNCTION()
	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION()
	void BackMjolnir(); //���ϸ� �߻� �� ���ƿ��� �Լ�

	bool bReturning; //BDHammerThrow�� ����
	bool bCreateTrue; //false�϶��� ���� ����, true �϶��� ���� �Ұ���


	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

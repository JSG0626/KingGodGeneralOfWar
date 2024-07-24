// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorFSM.h"
#include "BDThor.h"

// Sets default values for this component's properties
UBDThorFSM::UBDThorFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UBDThorFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBDThorFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState) {
		case BDThorGeneralState :: BDIdle:
			BDIdleState();
			break;
		case BDThorGeneralState :: BDMove:
			BDMoveState();
			break;
		case BDThorGeneralState :: BDDamage:
			BDDamageState();
			break;
	}

	//����â�� ���� �޼��� ���
	FString logMessege = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMessege);
}

//���� ���� �Լ�
void UBDThorFSM::BDIdleState()
{
}

void UBDThorFSM::BDMoveState()
{
}

void UBDThorFSM::BDAttackState()
{
}

void UBDThorFSM::BDDamageState()
{
}


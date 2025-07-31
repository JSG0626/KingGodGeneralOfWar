// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SpawnActor.h"
#include <Engine\World.h>

void UAN_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 1. 유효성 검사: 스폰할 액터 클래스(Template)와 오너 액터, 월드가 유효한지 확인합니다.
	if (!Template)
	{
		return;
	}

	AActor* OwnerActor = MeshComp->GetOwner();
	UWorld* World = OwnerActor ? OwnerActor->GetWorld() : nullptr;

	if (!World)
	{
		return;
	}

	// 2. 스폰 파라미터 설정
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = OwnerActor->GetInstigator();

	// 필요에 따라 충돌 처리 방식을 설정할 수 있습니다.
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 3. 스폰 위치 및 회전 계산
	// SocketName은 UAnimNotify에서 상속된 프로퍼티로, 애니메이션 에디터의 노티파이 디테일 패널에서 설정해야 합니다.
	const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
	const FTransform OffsetTransform(RotationOffset, LocationOffset);
	const FTransform SpawnTransform = OffsetTransform * SocketTransform;

	// 4. 액터 스폰
	AActor* NewActor = World->SpawnActor<AActor>(Template, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator(), SpawnParams);

	// 5. 부착 (Attach)
	// bAttached가 true일 경우, 스폰된 액터를 지정된 소켓에 붙입니다.
	if (NewActor && bAttached)
	{
		// 월드 트랜스폼을 유지하면서 붙여, 이미 계산된 위치와 회전값을 그대로 사용합니다.
		NewActor->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepWorldTransform, SocketName);
	}
}

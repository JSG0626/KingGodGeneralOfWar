// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SpawnActor.h"
#include <Engine\World.h>

void UAN_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 1. ��ȿ�� �˻�: ������ ���� Ŭ����(Template)�� ���� ����, ���尡 ��ȿ���� Ȯ���մϴ�.
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

	// 2. ���� �Ķ���� ����
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = OwnerActor->GetInstigator();

	// �ʿ信 ���� �浹 ó�� ����� ������ �� �ֽ��ϴ�.
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 3. ���� ��ġ �� ȸ�� ���
	// SocketName�� UAnimNotify���� ��ӵ� ������Ƽ��, �ִϸ��̼� �������� ��Ƽ���� ������ �гο��� �����ؾ� �մϴ�.
	const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
	const FTransform OffsetTransform(RotationOffset, LocationOffset);
	const FTransform SpawnTransform = OffsetTransform * SocketTransform;

	// 4. ���� ����
	AActor* NewActor = World->SpawnActor<AActor>(Template, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator(), SpawnParams);

	// 5. ���� (Attach)
	// bAttached�� true�� ���, ������ ���͸� ������ ���Ͽ� ���Դϴ�.
	if (NewActor && bAttached)
	{
		// ���� Ʈ�������� �����ϸ鼭 �ٿ�, �̹� ���� ��ġ�� ȸ������ �״�� ����մϴ�.
		NewActor->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepWorldTransform, SocketName);
	}
}

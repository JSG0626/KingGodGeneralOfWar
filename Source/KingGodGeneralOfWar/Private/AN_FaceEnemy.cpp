// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_FaceEnemy.h"
#include "Kratos.h"

void UAN_FaceEnemy::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld())
	{
		return;
	}
	UAnimInstance* anim = MeshComp->GetAnimInstance();
	if (anim == nullptr) return;

	APawn* pawn = anim->TryGetPawnOwner();
	if (pawn == nullptr) return;

	if (AKratos* kratos = CastChecked<AKratos>(pawn))
	{
		kratos->bFaceEnemy = bFaceEnemy;
	}
}


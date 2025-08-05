// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_TraceEnemy.h"
#include "Kratos.h"

void UANS_TraceEnemy::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
		if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld())
	{
		return;
	}
	UAnimInstance* anim = MeshComp->GetAnimInstance();
	if (anim == nullptr) return;

	APawn* Pawn = anim->TryGetPawnOwner();
	if (Pawn == nullptr) return;

	if (AKratos* Kratos = Cast<AKratos>(Pawn))
	{
		Kratos->bTraceEnemy = true;
	}
}

void UANS_TraceEnemy::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld())
	{
		return;
	}
	UAnimInstance* anim = MeshComp->GetAnimInstance();
	if (anim == nullptr) return;
	
	APawn* Pawn = anim->TryGetPawnOwner();
	if (Pawn == nullptr) return;
	
	if (AKratos* Kratos = Cast<AKratos>(Pawn))
	{
		Kratos->bTraceEnemy = false;
	}
}

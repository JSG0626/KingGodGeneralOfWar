// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_ActiveAxeTrail.h"
#include "Kratos.h"

void UANS_ActiveAxeTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
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
		kratos->ActiveAxeTrail(true);
	}
}

void UANS_ActiveAxeTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
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
		kratos->ActiveAxeTrail(false);
	}
}

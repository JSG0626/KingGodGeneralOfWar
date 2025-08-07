// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_TakeAxeLeftHand.h"
#include "Kratos.h"

void UANS_TakeAxeLeftHand::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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

	Kratos = Cast<AKratos>(Pawn);
	if (Kratos == nullptr) return;

	Kratos->SwapAxeHands(false);
}
void UANS_TakeAxeLeftHand::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld() || Kratos == nullptr)
	{
		return;
	}

	Kratos->SwapAxeHands(true);
}

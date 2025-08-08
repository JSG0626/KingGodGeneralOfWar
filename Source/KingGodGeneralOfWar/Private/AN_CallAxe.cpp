// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_CallAxe.h"
#include "Kratos.h"

void UAN_CallAxe::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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

	AKratos* Kratos = CastChecked<AKratos>(pawn);
	if (Kratos)
	{
		Kratos->SwapAxeHands(bRightHand);
		Kratos->CallAxe(MaxReturnDuration, MinReturnDuration, bImmediateReturn, RadiusScale, bRightHand);
	}
}

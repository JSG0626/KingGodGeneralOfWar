// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_ThrowAxe.h"
#include "Kratos.h"

void UAN_ThrowAxe::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
		Kratos->ThrowAxe(bIsHeavy);
		Kratos->HideHoldingAxe();
	}
}

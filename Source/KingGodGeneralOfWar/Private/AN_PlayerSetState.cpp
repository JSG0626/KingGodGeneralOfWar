// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_PlayerSetState.h"

void UAN_PlayerSetState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld())
	{
		return;
	}
	UAnimInstance* anim = MeshComp->GetAnimInstance();
	if (anim == nullptr) return;

	APawn* pawn = anim->TryGetPawnOwner();
	if (pawn == nullptr) return;

	AKratos* kratos = CastChecked<AKratos>(pawn);
	kratos->SetState(NewState);
	kratos->SetKratosState(NewState);
}

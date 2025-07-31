// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_SetAxeRotation.h"
#include "Kratos.h"
#include "Axe.h"

void UANS_SetAxeRotation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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

	AKratos* Kratos = Cast<AKratos>(Pawn);
	if (Kratos == nullptr) return;

	Axe = Kratos->Axe;
	if (Axe == nullptr) return;

	Axe->SetActorRelativeRotation(NewAxeRotation);
}

void UANS_SetAxeRotation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld() || Axe == nullptr)
	{
		return;
	}
	Axe->SetActorRelativeRotation(FRotator(0));
}

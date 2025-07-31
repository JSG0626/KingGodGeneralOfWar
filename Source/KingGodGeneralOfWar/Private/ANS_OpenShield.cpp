// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_OpenShield.h"
#include "Kratos.h"
#include "SG_Shield.h"


void UANS_OpenShield::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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

	Shield = Kratos->Shield;
	if (Shield == nullptr) return;

	Shield->SetTargetScale(true);
}

void UANS_OpenShield::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld() || Shield == nullptr)
	{
		return;
	}
	Shield->SetTargetScale(false);
}

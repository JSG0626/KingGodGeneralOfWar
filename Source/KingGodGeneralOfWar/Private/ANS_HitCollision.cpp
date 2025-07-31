// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_HitCollision.h"
#include "Animation/AnimInstance.h"
#include "Kratos.h"
#include "SG_Shield.h"
#include "Axe.h"

void UANS_HitCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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

	if (WeaponType == EWeaponType::SHIELD)
	{
		Shield = TScriptInterface<IWeaponInterface>(Kratos->Shield);
		Shield->ActiveHitCollision(true);
	}
	else if (WeaponType == EWeaponType::AXE)
	{
		Axe = TScriptInterface<IWeaponInterface>(Kratos->Axe);
		Axe->ActiveHitCollision(true);
	}
}


void UANS_HitCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld())
	{
		return;
	}
	if (WeaponType == EWeaponType::SHIELD && Shield)
	{
		Shield->ActiveHitCollision(false);
	}
	else if (WeaponType == EWeaponType::AXE && Axe)
	{
		Axe->ActiveHitCollision(false);
	}
}

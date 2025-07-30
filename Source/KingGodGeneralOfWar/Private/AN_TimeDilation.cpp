// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_TimeDilation.h"
#include "Kratos.h"

void UAN_TimeDilation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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
	if (kratos == nullptr) return;

	kratos->SetGlobalTimeDilation(Duration, SlowScale);
}

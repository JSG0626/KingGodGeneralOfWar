// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_AN_CharacterMove.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
void USG_AN_CharacterMove::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld())
	{
		return;
	}
	UAnimInstance* anim = MeshComp->GetAnimInstance();
	if (anim == nullptr) return;

	Pawn = anim->TryGetPawnOwner();
	if (Pawn == nullptr) return;

	CMC = Cast<UCharacterMovementComponent>(Pawn->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (CMC == nullptr) return;

	OriginWalkSpeed = CMC->MaxWalkSpeed;
	CMC->MaxWalkSpeed = NewWalkSpeed;

	Direction.Normalize();
	FTransform ActorTransform = CMC->GetActorTransform();
	WorldDirection = UKismetMathLibrary::TransformDirection(ActorTransform, Direction);
	CMC->Velocity = WorldDirection * NewWalkSpeed;
}

void USG_AN_CharacterMove::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld() || CMC == nullptr)
	{
		return;
	}

	Pawn->AddMovementInput(WorldDirection, 1.0f);
}

void USG_AN_CharacterMove::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld())
	{
		return;
	}
	if (CMC == nullptr) return;
	CMC->MaxWalkSpeed = OriginWalkSpeed;
}

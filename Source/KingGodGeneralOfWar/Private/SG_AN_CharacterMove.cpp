// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_AN_CharacterMove.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
void USG_AN_CharacterMove::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UAnimInstance* anim = MeshComp->GetAnimInstance();
	if (anim == nullptr) return;

	APawn* pawn = anim->TryGetPawnOwner();
	if (pawn == nullptr) return;

	CMC = Cast<UCharacterMovementComponent>(pawn->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (CMC == nullptr) return;

	OriginWalkSpeed = CMC->MaxWalkSpeed;
	CMC->MaxWalkSpeed = NewWalkSpeed;

	Direction.Normalize();
	UE_LOG(LogTemp, Display, TEXT("Begin"));
}

void USG_AN_CharacterMove::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (CMC == nullptr) return;
	FTransform ActorTransform = CMC->GetActorTransform(); // 현재 액터의 월드 트랜스폼
	FVector WorldDirection = UKismetMathLibrary::TransformDirection(ActorTransform, Direction);

	if (CMC == nullptr) return;
	CMC->AddInputVector(WorldDirection);
	UE_LOG(LogTemp, Display, TEXT("CMC->AddInputVector(%s);"), *WorldDirection.ToString());
}

void USG_AN_CharacterMove::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (CMC == nullptr) return;
	CMC->MaxWalkSpeed = OriginWalkSpeed;
	UE_LOG(LogTemp, Display, TEXT("End"));
}

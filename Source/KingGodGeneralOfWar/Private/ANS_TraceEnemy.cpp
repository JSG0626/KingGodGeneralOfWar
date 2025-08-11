// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_TraceEnemy.h"
#include "Kratos.h"
#include <GameFramework/CharacterMovementComponent.h>
void UANS_TraceEnemy::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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

	Kratos->bTraceEnemy = true;
	Kratos->SetMaxWalkSpeed(NewWalkSpeed);
	Kratos->CurTargetEnemy = Kratos->FindTargetEnemy();

	UCharacterMovementComponent* CMC = Cast<UCharacterMovementComponent>(Pawn->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (CMC == nullptr) return;

	if (nullptr != Kratos->CurTargetEnemy)
	{
		const FVector ToTarget = (Kratos->CurTargetEnemy->GetActorLocation() - Kratos->GetActorLocation()).GetSafeNormal();
		CMC->Velocity = ToTarget * NewWalkSpeed;
	}
}

void UANS_TraceEnemy::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (nullptr == Kratos) return;

	if (nullptr != Kratos->CurTargetEnemy)
	{
		const FVector ToTarget = (Kratos->CurTargetEnemy->GetActorLocation() - Kratos->GetActorLocation()).GetSafeNormal();
		if (Kratos->bFaceEnemy)
		{
			FRotator rotate = ToTarget.Rotation();
			rotate.Pitch = 0.0f;
			Kratos->SetActorRotation(rotate);
		}

		float DistSquared = FVector::DistSquared(Kratos->CurTargetEnemy->GetActorLocation(), Kratos->GetActorLocation());
		if (DistSquared >= Kratos->GetAttackRangeSquared())
		{
			Kratos->AddMovementInput(ToTarget, 1.0f);
		}
	}
	else
	{
		Kratos->CurTargetEnemy = Kratos->FindTargetEnemy();
		if (Kratos->CurTargetEnemy == nullptr)
		{
			Kratos->AddMovementInput(Kratos->GetActorForwardVector(), 1.0f);
		}
	}


}

void UANS_TraceEnemy::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UE_LOG(LogTemp, Display, TEXT("TraceEnemy NotifyEnd"));
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp || !MeshComp->GetWorld() || !MeshComp->GetWorld()->IsGameWorld() || nullptr == Kratos)
	{
		return;
	}
	Kratos->InitMaxWalkSpeed();
	Kratos->bTraceEnemy = false;
}

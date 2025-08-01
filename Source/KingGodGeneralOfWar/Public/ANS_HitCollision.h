// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "KingGodGeneralOfWar.h"
#include "ANS_HitCollision.generated.h"

/**
 * 
 */
UCLASS()
class KINGGODGENERALOFWAR_API UANS_HitCollision : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
	TScriptInterface<class IWeaponInterface> Shield;
	TScriptInterface<class IWeaponInterface> Axe;
public:
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	float AttackScale = 1.0f;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

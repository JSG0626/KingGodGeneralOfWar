#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Kratos.h"
#include "WeaponInterface.generated.h"

/*
*/
UINTERFACE(MinimalAPI, Blueprintable)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};


/* 실제 인터페이스 선언. */
class KINGGODGENERALOFWAR_API IWeaponInterface
{
	GENERATED_BODY()

protected:
	float BaseAttackPower = 10.0f;
	float CurrentAttackScale = 1.0f;
	float CurrentStunAttackScale = 10.0f;

	
	virtual TObjectPtr<class USoundCue> GetBaseHitSound() const = 0;

public:
	virtual void ActiveHitCollision(bool ActiveState) = 0;
	void SetAttackScale(const float AttackScale);
	void DealDamage(class ABaseEnemy* Target, const FGenericAttackParams& params);
};
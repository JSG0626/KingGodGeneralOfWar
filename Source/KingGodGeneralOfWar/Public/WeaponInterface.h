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

struct FGenericAttackParams
{
	class ACharacter* Instigator;
	float Damage;
	float StunDamage;
	EAttackDirectionType Direction;

	FGenericAttackParams(class ACharacter* _Instigator, float _Damage, float _StunDamage, EAttackDirectionType _Direction) : Instigator(_Instigator), Damage(_Damage), StunDamage(_StunDamage), Direction(_Direction) {}
};

/* ���� �������̽� ����. */
class KINGGODGENERALOFWAR_API IWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void ActiveHitCollision(bool ActiveState) = 0;

	virtual void DealDamage(class ABaseEnemy* Target, const FGenericAttackParams& params);
};
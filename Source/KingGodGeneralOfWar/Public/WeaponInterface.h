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

USTRUCT(BlueprintType)
struct FGenericAttackParams
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	class ACharacter* Instigator;
	UPROPERTY(BlueprintReadWrite)
	float Damage;
	UPROPERTY(BlueprintReadWrite)
	float StunDamage;
	UPROPERTY(BlueprintReadWrite)
	EAttackDirectionType Direction;

	FGenericAttackParams() : Instigator(nullptr), Damage(0.0f), StunDamage(0.0f), Direction(EAttackDirectionType::LEFT) {}
	FGenericAttackParams(class ACharacter* _Instigator, float _Damage, float _StunDamage, EAttackDirectionType _Direction) : Instigator(_Instigator), Damage(_Damage), StunDamage(_StunDamage), Direction(_Direction) {}
};

/* 실제 인터페이스 선언. */
class KINGGODGENERALOFWAR_API IWeaponInterface
{
	GENERATED_BODY()

protected:
	float BaseAttackPower = 10.0f;
	float CurrentAttackScale = 1.0f;
	float CurrentStunAttackScale = 10.0f;
public:
	virtual void ActiveHitCollision(bool ActiveState) = 0;
	void SetAttackScale(const float AttackScale);
	void DealDamage(class ABaseEnemy* Target, const FGenericAttackParams& params);
};
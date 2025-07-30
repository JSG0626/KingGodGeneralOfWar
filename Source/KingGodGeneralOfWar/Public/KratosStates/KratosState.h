// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KingGodGeneralOfWar.h"
#include "KratosState.generated.h"

/**
 *
 */

 // // �÷��̾� ���¸� ��Ÿ���� Enum (AKratos Ŭ������ ���� ����� ����)
 //UENUM(BlueprintType)
 //enum class EKratosStateType : uint8
 //{
 //	Idle			UMETA(DisplayName = "Idle"),
 //	Move			UMETA(DisplayName = "Move"),
 //	Run				UMETA(DisplayName = "Run"),
 //	Dodge			UMETA(DisplayName = "Dodge"),
 //	Roll			UMETA(DisplayName = "Roll"),
 //	Attack			UMETA(DisplayName = "Attack"),
 //	Aim				UMETA(DisplayName = "Aim"),
 //	Parry			UMETA(DisplayName = "Parry"),
 //	Guard			UMETA(DisplayName = "Guard"),
 //	GuardHit		UMETA(DisplayName = "GuardHit"),
 //	Hit				UMETA(DisplayName = "Hit"),
 //	Stop			UMETA(DisplayName = "Stop"),
 //	Dead			UMETA(DisplayName = "Dead"),
 //};

struct FEnemyAttackParams
{
	class AActor* Attacker;
	int Damage;
	EHitType HitType;
	bool bMelee;
	bool bGuardable;
	FEnemyAttackParams() = default;
	FEnemyAttackParams(class AActor* _Attacker, int _Damage, EHitType _HitType, bool _bMelee, bool _bGuardable = true) :
		Attacker(_Attacker), Damage(_Damage), HitType(_HitType), bMelee(_bMelee), bGuardable(_bGuardable) {}
};

struct FGenericStateParams
{
	int Integer;
	float Float;
	FString String;
	FVector Vector;
	FVector2D Vector2D;
	FTransform Transform;
	FEnemyAttackParams AttackParams;

	FGenericStateParams() = default;

	FGenericStateParams(int v) : Integer(v) {}
	FGenericStateParams(float v) : Float(v) {}
	FGenericStateParams(const FString& v) : String(v) {}
	FGenericStateParams(const FVector2D& v) : Vector2D(v) {}
	FGenericStateParams(const FVector& v) : Vector(v) {}
	FGenericStateParams(const FTransform& v) : Transform(v) {}
	FGenericStateParams(const FEnemyAttackParams& v) : AttackParams(v) {}
};
UINTERFACE(MinimalAPI, Blueprintable)
class UKratosState : public UInterface
{
	GENERATED_BODY()

};


class KINGGODGENERALOFWAR_API IKratosState
{
	GENERATED_BODY()

protected:
	class AKratos* Me;
	class USG_KratosAnim* Anim;
	bool InputOn;

	// Debug Setting
	bool bShowLog = true;
	bool is_uelog = true;
	bool is_dm = true;
	bool bShowTickLog = false;
	void StateLog(const FString& message, bool isTickLog = false) const;

	virtual void LookAtProcess(FRotator offset = FRotator(0));

public:
	virtual void SetUp(class AKratos* kratos);
	virtual void EnterState(const FGenericStateParams& params) = 0;
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) = 0;
	virtual void ExitState(const FGenericStateParams& params) = 0;
	virtual void SetInputOn() { InputOn = true; }

	// �ش� ���°� Ư�� �ൿ�� ó���� �� �ִ��� ����
	virtual bool CanHandleMove() const { return false; }
	virtual bool CanHandleIdle() const { return false; }
	virtual bool CanHandleWAttack() const { return false; }
	virtual bool CanHandleSAttack() const { return false; }
	virtual bool CanHandleAim() const { return false; }
	virtual bool CanHandleDodge() const { return false; }
	virtual bool CanHandleRoll() const { return false; }
	virtual bool CanHandleGuard() const { return false; }
	virtual bool CanHandleParry() const { return false; }

	virtual bool CanHandleHit() const { return true; }

	// ���¸� ó���ϴ� �Լ�. �⺻������ �ƹ� �ൿ�� ���� ����
	virtual void HandleIdle(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleMove(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleGuard(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleParry(const FGenericStateParams& params = FGenericStateParams()) {};
	virtual void HandleAim(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleWAttack(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleSAttack(const FGenericStateParams& params = FGenericStateParams()) {};

	// �⺻������ ��� �̵��� ������ ���µ��� �ٷ�� �޼ҵ�
	virtual void HandleHit(const FGenericStateParams& params = FGenericStateParams());
};

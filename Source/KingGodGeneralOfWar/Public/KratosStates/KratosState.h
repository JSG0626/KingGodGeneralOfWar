#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KingGodGeneralOfWar.h"
#include "KratosState.generated.h"



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
	EPlayerState PrevState;
	FGenericStateParams() = default;

	FGenericStateParams(int v) : Integer(v) {}
	FGenericStateParams(float v) : Float(v) {}
	FGenericStateParams(const FString& v) : String(v) {}
	FGenericStateParams(const FVector2D& v) : Vector2D(v) {}
	FGenericStateParams(const FVector& v) : Vector(v) {}
	FGenericStateParams(const FTransform& v) : Transform(v) {}
	FGenericStateParams(const FEnemyAttackParams& v) : AttackParams(v) {}
};

UCLASS(Blueprintable, BlueprintType)
class KINGGODGENERALOFWAR_API UKratosState : public UObject
{
	GENERATED_BODY()

protected:
	class AKratos* Me;
	class USG_KratosAnim* Anim;
	bool InputOn;
	FVector TargetLookAtPos;

	// Debug Setting
	bool bShowLog = true;
	bool is_uelog = true;
	bool is_dm = true;
	bool bShowTickLog = false;
	void StateLog(const FString& message, bool isTickLog = false) const;

	virtual void LookAtProcess(float DeltaTime, FRotator offset = FRotator(0));

public:
	virtual void SetUp(class AKratos* kratos);
	virtual void EnterState(const FGenericStateParams& params) {};
	virtual void TickState(const FGenericStateParams& params, float DeltaTime) {};
	virtual void ExitState(const FGenericStateParams& params) {};
	virtual void SetInputOn() { InputOn = true; }

	// 해당 상태가 특정 행동을 처리할 수 있는지 여부
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

	// 상태를 처리하는 함수. 기본적으로 아무 행동도 하지 않음
	virtual void HandleIdle(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleMove(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleDodge(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleGuard(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleParry(const FGenericStateParams& params = FGenericStateParams()) {};
	virtual void HandleAim(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleWAttack(const FGenericStateParams& params = FGenericStateParams()) {}
	virtual void HandleSAttack(const FGenericStateParams& params = FGenericStateParams()) {};

	// 기본적으로 즉시 전환이 가능한 상태들을 다루는 메소드
	virtual void HandleHit(const FGenericStateParams& params = FGenericStateParams());
	virtual void HandleDie(const FGenericStateParams& params = FGenericStateParams());
};

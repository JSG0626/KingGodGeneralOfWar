// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Dodge UMETA(DisplayName = "Dodge"),
	LAttack UMETA(DisplayName = "LAttack"),
	HAttack UMETA(DisplayName = "HAttack"),
	LRunicAttack UMETA(DisplayName = "LRunicAttack"),
	HRunicAttack UMETA(DisplayName = "HRunicAttack"),
	Aim UMETA(DisplayName = "Aim"),
	Parry UMETA(DisplayName = "Parry"),
	Guard UMETA(DisplayName = "Guard"),
	Ability UMETA(DisplayName = "Ability"),
	GrabAxe UMETA(DisplayName = "GrabAxe"),
	Artifact UMETA(DisplayName = "Artifact"),
	Hit UMETA(DisplayName = "Hit"),
	Die UMETA(DisplayName = "Die"),
	LDashAttack UMETA(DisplayName = "LDashAttack"),
	HDashAttack UMETA(DisplayName = "HDashAttack"),
	CombatIdle UMETA(DisplayName = "CombatIdle"),
	NoneMovable UMETA(DisplayName = "NoneMovable"),
	LChargeAttack UMETA(DisplayName = "LChargeAttack"),
	HChargeAttack UMETA(DisplayName = "HChargeAttack"),
	HChargeAttackHit UMETA(DisplayName = "HChargeAttackHit"),
	AxeReturnLAttack UMETA(DisplayName = "AxeReturnLAttack"),
	AxeReturnHAttack UMETA(DisplayName = "AxeReturnHAttack"),
	DodgeFrontAttack UMETA(DisplayName = "DodgeFrontAttack"),
	DodgeBackAttack UMETA(DisplayName = "DodgeBackAttack"),
};

UENUM(BlueprintType)
enum class EPlayerMontage : uint8
{
	Dodge UMETA(DisplayName = "Dodge"),
	Roll UMETA(DisplayName = "Roll"),
	LAttack UMETA(DisplayName = "LAttack"),
	HAttack UMETA(DisplayName = "HAttack"),
	LRunicAttack UMETA(DisplayName = "LRunicAttack"),
	HRunicAttack UMETA(DisplayName = "HRunicAttack"),
	Parry UMETA(DisplayName = "Parry"),
	Guard UMETA(DisplayName = "Guard"),
	Ability UMETA(DisplayName = "Ability"),
	LightThrowAxe UMETA(DisplayName = "LightThrowAxe"),
	HeavyThrowAxe UMETA(DisplayName = "HeavyThrowAxe"),
	CallAxe UMETA(DisplayName = "CallAxe"),
	GrabAxe UMETA(DisplayName = "GrabAxe"),
	Artifact UMETA(DisplayName = "Artifact"),
	Hit UMETA(DisplayName = "Hit"),
	Die UMETA(DisplayName = "Die"),
	LDashAttack UMETA(DisplayName = "LDashAttack"),
	HDashAttack UMETA(DisplayName = "HDashAttack"),
	CombatIdle UMETA(DisplayName = "CombatIdle"),
	HChargeAttack UMETA(DisplayName = "HChargeAttack"),
	HChargeAttackHit UMETA(DisplayName = "HChargeAttackHit"),
	LChargeAttack UMETA(DisplayName = "LChargeAttack"),
	AxeReturnLAttack UMETA(DisplayName = "AxeReturnLAttack"),
	AxeReturnHAttack UMETA(DisplayName = "AxeReturnHAttack"),
	DodgeFrontAttack UMETA(DisplayName = "DodgeFrontAttack"),
	DodgeBackAttack UMETA(DisplayName = "DodgeBackAttack"),
};

UENUM(BlueprintType)
enum class EPlayerWeaponType : uint8
{
	Unarmed UMETA(DisplayName = "Unarmed"),
	Axe UMETA(DisplayName = "Axe"),
	Blade UMETA(DisplayName = "Blade"),
	Spear UMETA(DisplayName = "Spear"),
};


UENUM(BlueprintType)
enum class EHitType : uint8
{
	STAGGER UMETA(DisplayName = "STAGGER"),
	NB_HIGH UMETA(DisplayName = "NB_HIGH"),
	NB_MEDIUM UMETA(DisplayName = "NB_MEDIUM"),
	NB_LOW UMETA(DisplayName = "NB_LOW"),
	STUN UMETA(DisplayName = "STUN"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AXE UMETA(DisplayName = "AXE"),
	SHIELD UMETA(DisplayName = "SHIELD"),
	HAND UMETA(DisplayName = "HAND"),
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None UMETA(DisplayName = "None"),
	LAttack UMETA(DisplayName = "LAttack"),
	HAttack UMETA(DisplayName = "HAttack"),
};



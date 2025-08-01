// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericAttackParams.h"

UENUM(BlueprintType)
enum class EHitType : uint8
{
	STAGGER UMETA(DisplayName = "STAGGER"),
	NB_HIGH UMETA(DisplayName = "NB_HIGH"),
	NB_MEDIUM UMETA(DisplayName = "STAGGER"),
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
enum class EPlayerState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Run UMETA(DisplayName = "Run"),
	Dodge UMETA(DisplayName = "Dodge"),
	Roll UMETA(DisplayName = "Roll"),
	WAttack UMETA(DisplayName = "WAttack"),
	SAttack UMETA(DisplayName = "SAttack"),
	RuneWAttack UMETA(DisplayName = "RuneWAttack"),
	RuneSAttack UMETA(DisplayName = "RuneSAttack"),
	Aim UMETA(DisplayName = "Aim"),
	//GuardStart UMETA(DisplayName = "GuardStart"),
	Parry UMETA(DisplayName = "Parry"),
	Guard UMETA(DisplayName = "Guard"),
	//GuardHit UMETA(DisplayName = "GuardHit"),
	//NoneMovable UMETA(DisplayName = "NoneMovable"),
	Hit UMETA(DisplayName = "Hit"),
	Die UMETA(DisplayName = "Die"),
};

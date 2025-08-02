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
	Dodge UMETA(DisplayName = "Dodge"),
	WAttack UMETA(DisplayName = "WAttack"),
	SAttack UMETA(DisplayName = "SAttack"),
	RuneWAttack UMETA(DisplayName = "RuneWAttack"),
	RuneSAttack UMETA(DisplayName = "RuneSAttack"),
	Aim UMETA(DisplayName = "Aim"),
	Parry UMETA(DisplayName = "Parry"),
	Guard UMETA(DisplayName = "Guard"),
	Ability UMETA(DisplayName = "Ability"),
	GrabAxe UMETA(DisplayName = "GrabAxe"),
	Artifact UMETA(DisplayName = "Artifact"),
	Hit UMETA(DisplayName = "Hit"),
	Die UMETA(DisplayName = "Die"),
};

UENUM(BlueprintType)
enum class EPlayerMontage : uint8
{
	Dodge UMETA(DisplayName = "Dodge"),
	Roll UMETA(DisplayName = "Roll"),
	WAttack UMETA(DisplayName = "WAttack"),
	SAttack UMETA(DisplayName = "SAttack"),
	RuneWAttack UMETA(DisplayName = "RuneWAttack"),
	RuneSAttack UMETA(DisplayName = "RuneSAttack"),
	Parry UMETA(DisplayName = "Parry"),
	Guard UMETA(DisplayName = "Guard"),
	Ability UMETA(DisplayName = "Ability"),
	ThrowAxe UMETA(DisplayName = "ThrowAxe"),
	CallAxe UMETA(DisplayName = "CallAxe"),
	GrabAxe UMETA(DisplayName = "GrabAxe"),
	Artifact UMETA(DisplayName = "Artifact"),
	Hit UMETA(DisplayName = "Hit"),
	Die UMETA(DisplayName = "Die"),
};


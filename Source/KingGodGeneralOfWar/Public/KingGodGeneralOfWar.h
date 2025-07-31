// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

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
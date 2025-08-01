// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericAttackParams.generated.h" // <<-- USTRUCT가 정의된 헤더의 맨 마지막에 위치해야 함!


UENUM()
enum class EAttackDirectionType : uint8
{
    DOWN = 0 UMETA(DisplayName = "DOWN"),
    UP  UMETA(DisplayName = "UP"),
    LEFT UMETA(DisplayName = "LEFT"),
    RIGHT UMETA(DisplayName = "RIGHT"),
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
    UPROPERTY(BlueprintReadWrite)
    bool bSuperAttack;

    FGenericAttackParams() : Instigator(nullptr), Damage(0.0f), StunDamage(0.0f), Direction(EAttackDirectionType::LEFT), bSuperAttack(false) {}

    FGenericAttackParams(class ACharacter* _Instigator, float _Damage, float _StunDamage, EAttackDirectionType _Direction = EAttackDirectionType::LEFT, bool _bSuperAttack = false)
        : Instigator(_Instigator), Damage(_Damage), StunDamage(_StunDamage), Direction(_Direction), bSuperAttack(_bSuperAttack) {
    }
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraSettingParams.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FCameraSettingParams
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float FieldOfView = 90;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float FocalDistance = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Location")
    FVector SocketOffset = FVector(0, 50, 45);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Location")
    FRotator CameraRotation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Location")
    float TargetArmLength = 147;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blend")
    float BlendSpeed = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
    TSubclassOf<UCameraShakeBase> CameraShakeClass = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
    float ShakeScale = 1.0f;

    FString ToString() const
    {
        return FString::Printf(TEXT("FOV: %f, FocalDist: %f, SocketOffset: %s, CameraRotation: %s, TargetArmLength: %f, BlendSpeed: %f"), 
            FieldOfView, FocalDistance, *(SocketOffset.ToString()), *(CameraRotation.ToString()), TargetArmLength, BlendSpeed);
    }
};
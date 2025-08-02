// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_KratosAnim.h"

#include "KingGodGeneralOfWar.h"
#include "RuneAttackField.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "SG_Shield.h"

USG_KratosAnim::USG_KratosAnim()
{
	
}

void USG_KratosAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	UpdatePlayerState();

}

void USG_KratosAnim::UpdatePlayerState()
{
	if (Me)
	{
		FVector velocity = Me->GetVelocity();
		FVector forwardVector = Me->GetActorForwardVector();

		PlayerState = Me->State;
		Speed = FVector::DotProduct(forwardVector, velocity);
		Direction = FVector::DotProduct(Me->GetActorRightVector(), velocity);
	}
}

void USG_KratosAnim::PlayMontage(const EPlayerMontage State, bool bJumpSection, const FString SectionName)
{
	UAnimMontage* Montage = MontageMap[State];
	if (Montage)
	{
		Montage_Play(Montage);
		if (bJumpSection)
		{
			Montage_JumpToSection(FName(*SectionName), Montage);
		}
	}
}

void USG_KratosAnim::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSection(NewSection), nullptr);
}

void USG_KratosAnim::ActiveLookAt(bool Active)
{
	if (Active == bActiveLookAt)
	{
		return;
	}

	if (!Active)
	{
		bActiveLookAt = false;
		return;
	}
	FRotator ActorRotation = Me->GetActorRotation();
	FRotator ControlRotation = Me->GetControlRotation();
	FRotator RelativeControlRotation = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation);
	float ClampedRelativePitch = FMath::ClampAngle(RelativeControlRotation.Pitch, -20.0f, 20.0f); // 상하 제한
	float ClampedRelativeYaw = FMath::ClampAngle(RelativeControlRotation.Yaw, -60.0f, 60.0f);   // 좌우 제한

	// 5. 제한된 상대적인 Pitch와 Yaw를 사용하여 새로운 회전 값을 만듭니다.
	// Roll은 보통 0으로 유지합니다 (고개가 옆으로 기울어지는 것은 드물기 때문).
	FRotator ClampedRelativeRotator = FRotator(ClampedRelativePitch, ClampedRelativeYaw, 0.0f);

	// 6. 이 상대 회전을 캐릭터의 ActorRotation에 다시 적용하여,
	// 월드 공간에서 목이 실제로 바라볼 '제한된' 방향을 계산합니다.
	FRotator LookAtWorldRotation = ActorRotation + ClampedRelativeRotator;

	// 7. 'neck_01' 본의 월드 위치를 가져옵니다. 이것이 시선의 '시작점'이 됩니다.
	FVector NeckBoneLocation = FVector::ZeroVector;
	if (Me && Me->GetMesh())
	{
		NeckBoneLocation = Me->GetMesh()->GetBoneLocation(FName(TEXT("neck_01")));
		// 만약 neck_01 본이 없다면 적절한 다른 본 (예: head)을 사용하세요.
	}
	else
	{
		// 메시가 없으면 캐릭터의 위치에서 시작하도록 폴백 (예: 눈높이)
		NeckBoneLocation = Me->GetActorLocation() + FVector(0, 0, Me->GetDefaultHalfHeight());
	}


	// 8. LookAtWorldRotation이 가리키는 방향으로 일정 거리 떨어진 목표 지점을 계산합니다.
	float LookAtDistance = 500.0f; // 목이 바라볼 가상의 목표 지점까지의 거리
	LookAtTarget = NeckBoneLocation + LookAtWorldRotation.Vector() * LookAtDistance;
	bActiveLookAt = true;
}

void USG_KratosAnim::AnimNotify_AbleNextAttack()
{
	Me->CanComboAttack = true;
}
void USG_KratosAnim::AnimNotify_ThrowAxe()
{
	if (Me)
	{
		Me->HideHoldingAxe();
		Me->ThrowAxe();
	}
}

void USG_KratosAnim::AnimNotify_FieldSpawn()
{
	//GetWorld()->SpawnActor<ARuneAttackField>(RuneAttackFieldFactory, Me->GetActorTransform());
}

void USG_KratosAnim::AnimNotify_RuneAttackGroundShake()
{
	Me->CameraShakeOnAttack(EAttackDirectionType::DOWN);
}

void USG_KratosAnim::AnimNotify_RuneAttackCameraSet()
{
	Me->OnMyRuneAttackCameraSet();
}

void USG_KratosAnim::AnimNotify_GetUPCameraSet()
{
	Me->OnMyGetUPCameraSet();
}

void USG_KratosAnim::AnimNotify_RuneReady()
{
	//Me->OnMyRuneReady();
}

void USG_KratosAnim::AnimNotify_RuneAttackEnd()
{
	//Me->OnMyRuneAttackEnd();
	//Montage_SetPlayRate(GuardMontage, 0.5f);
}

void USG_KratosAnim::AnimNotify_SpawnEarthCrack()
{
	Me->OnMySpawnEarthCrack();
}


void USG_KratosAnim::AnimNotify_InitAttackType()
{
	Me->OnMyInitAttackType();
}

void USG_KratosAnim::AnimNotify_AttackProgress()
{
	Me->OnMyAttackProgress();
}


void USG_KratosAnim::AnimNotify_LaunchCharacterInStrongAttack()
{
	Me->OnMyLaunchCharacterInStrongAttack();
}

void USG_KratosAnim::AnimNotify_EndWithFail()
{
	Me->OnMyEndWithFail();
}

void USG_KratosAnim::AnimNotify_JumpCharacterInStrongAttack()
{
	Me->OnMyJumpCharacterInStrongAttack();
}


void USG_KratosAnim::AnimNotify_EndDodge()
{
	Me->bEvade = false;
}

FName USG_KratosAnim::GetAttackMontageSection(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

FName USG_KratosAnim::GetRollMontageSection(int32 Section)
{
	TArray<FString> RollSectionName = { TEXT("Left") , TEXT("Right") , TEXT("Forward") ,TEXT("Back") };
	FName SectionName(*FString::Printf(TEXT("Roll_%s"), *RollSectionName[Section]));
	return SectionName;
}

FName USG_KratosAnim::GetGuardMontageSection(int32 Section)
{
	return FName();
}

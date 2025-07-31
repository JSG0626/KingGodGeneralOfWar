// Fill out your copyright notice in the Description page of Project Settings.


#include "SG_KratosAnim.h"

#include "RuneAttackField.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "SG_Shield.h"

USG_KratosAnim::USG_KratosAnim()
{
	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempAttackMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_WeakAttack.AM_Kratos_WeakAttack'")
	);
	if (TempAttackMontage.Succeeded())	WeakAttackMontage = TempAttackMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempStrongAttackMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_MeleeAttack.AM_Kratos_MeleeAttack'")
	);
	if (TempStrongAttackMontage.Succeeded())	StrongAttackMontage = TempStrongAttackMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempDodgeMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_Dodge.AM_Kratos_Dodge'")
	);
	if (TempDodgeMontage.Succeeded())	DodgeMontage = TempDodgeMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempRollMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_Roll.AM_Kratos_Roll'")
	);
	if (TempRollMontage.Succeeded())	RollMontage = TempRollMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempGuardMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_Guard.AM_Kratos_Guard'")
	);
	if (TempGuardMontage.Succeeded())	GuardMontage = TempGuardMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempAxeThrowMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_AxeThrow.AM_AxeThrow'")
	);
	if (TempAxeThrowMontage.Succeeded())	AxeThrowMontage = TempAxeThrowMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempAxeWithdrawMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_AxeWithdraw.AM_Kratos_AxeWithdraw'")
	);
	if (TempAxeWithdrawMontage.Succeeded())	AxeWithdrawMontage = TempAxeWithdrawMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempDashAttackMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_DashAttack.AM_Kratos_DashAttack'")
	);
	if (TempDashAttackMontage.Succeeded())	DashAttackMontage = TempDashAttackMontage.Object;
	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempRuneBaseMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_RuneBase.AM_Kratos_RuneBase'")
	);
	if (TempRuneBaseMontage.Succeeded())	RuneBaseMontage = TempRuneBaseMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempRuneAttackMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/JSG/Animations/AM_Hells_Touch.AM_Hells_Touch'")
	);
	if (TempRuneAttackMontage.Succeeded())	RuneWAttackMontage = TempRuneAttackMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempHitMontage(
		TEXT("Script / Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_Hit.AM_Kratos_Hit'")
	);
	if (TempHitMontage.Succeeded())	HitMontage = TempHitMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempParryAttackMontage(
		TEXT("/ Script / Engine.AnimMontage'/Game/JSG/Animations/AM_Kratos_ParryAttack.AM_Kratos_ParryAttack'")
	);

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempParryMontage(
		TEXT("/ Script / Engine.AnimMontage'/Game/JSG/Animations/AM_Parry.AM_Parry'")
	);
	if (TempParryMontage.Succeeded())	ParryMontage = TempParryMontage.Object;

	static ConstructorHelpers::FObjectFinder <UAnimMontage> TempDieMontage(
		TEXT("/ Script / Engine.AnimMontage'/Game/JSG/Animations/AM_Die.AM_Die'")
	);
	
	if (TempDieMontage.Succeeded())	DieMontage = TempDieMontage.Object;
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
void USG_KratosAnim::PlayWeakAttackMontage()
{
	if (!Montage_IsPlaying(WeakAttackMontage))
		Montage_Play(WeakAttackMontage, 1.0f);

}
void USG_KratosAnim::PlayStrongAttackMontage()
{
	if (!Montage_IsPlaying(StrongAttackMontage))
		Montage_Play(StrongAttackMontage, 1.0f);
}


void USG_KratosAnim::PlayDodgeMontage()
{
	Montage_Play(DodgeMontage, 1.0f);
}

void USG_KratosAnim::PlayRollMontage()
{
	Montage_Play(RollMontage, 1.0f);
}

void USG_KratosAnim::PlayGuardMontage()
{
	Montage_Play(GuardMontage, 1.0f);
}

bool USG_KratosAnim::PlayAxeThrowMontage()
{
	if (!Montage_IsPlaying(AxeThrowMontage))
	{
		Montage_Play(AxeThrowMontage, 1.0f);
		//TargetRotation = Me->GetControlRotation();
		return true;
	}
	return false;
}

void USG_KratosAnim::PlayAxeWithdrawMontage()
{
	if (!Me) return;
	Montage_Play(AxeWithdrawMontage);
}

void USG_KratosAnim::PlayRuneBaseMontage()
{
	if (!Montage_IsPlaying(RuneBaseMontage))
		Montage_Play(RuneBaseMontage);
}

void USG_KratosAnim::PlayHitMontage()
{
	if (!Montage_IsPlaying(HitMontage))
		Montage_Play(HitMontage);
}

void USG_KratosAnim::PlayDashAttackMontage()
{
	Montage_Play(DashAttackMontage);
}

void USG_KratosAnim::PlayRuneAttackMontage()
{
	if (!Montage_IsPlaying(RuneWAttackMontage))
		Montage_Play(RuneWAttackMontage);
}

void USG_KratosAnim::PlayParryMontage()
{
	Montage_Play(ParryMontage);
}

void USG_KratosAnim::PlayParryAttackMontage()
{
	if (!Montage_IsPlaying(ParryAttackMontage))
		Montage_Play(ParryAttackMontage);
}

void USG_KratosAnim::PlayDieMontage()
{
	Montage_Play(DieMontage);
}

void USG_KratosAnim::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSection(NewSection), nullptr);

	//if (Montage_IsPlaying(StrongAttackMontage))
	//{
	//	Montage_JumpToSection(GetAttackMontageSection(NewSection), StrongAttackMontage);
	//	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("StrongAttackMontage"));
	//}
	//else if (Montage_IsPlaying(WeakAttackMontage))
	//{
	//	Montage_JumpToSection(GetAttackMontageSection(NewSection), WeakAttackMontage);
	//	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("StrongAttackMontage"));
	//}
	//else if (Montage_IsPlaying(RuneWAttackMontage))
	//{
	//	Montage_JumpToSection(GetAttackMontageSection(NewSection), RuneWAttackMontage);
	//	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("StrongAttackMontage"));
	//}
}

void USG_KratosAnim::JumpToDodgeMontageSection(FString SectionName)
{
	Montage_JumpToSection(FName(*SectionName), DodgeMontage);
}

void USG_KratosAnim::JumpToRollMontageSection(FString SectionName)
{
	Montage_JumpToSection(FName(*SectionName), RollMontage);
}

void USG_KratosAnim::JumpToGuardMontageSection(FString SectionName)
{
	Montage_JumpToSection(FName(*SectionName), GuardMontage);
}

void USG_KratosAnim::JumpToHitMontageSection(FString SectionName)
{
	Montage_JumpToSection(FName(*SectionName), HitMontage);
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
	float ClampedRelativePitch = FMath::ClampAngle(RelativeControlRotation.Pitch, -20.0f, 20.0f); // ���� ����
	float ClampedRelativeYaw = FMath::ClampAngle(RelativeControlRotation.Yaw, -60.0f, 60.0f);   // �¿� ����

	// 5. ���ѵ� ������� Pitch�� Yaw�� ����Ͽ� ���ο� ȸ�� ���� ����ϴ�.
	// Roll�� ���� 0���� �����մϴ� (���� ������ �������� ���� �幰�� ����).
	FRotator ClampedRelativeRotator = FRotator(ClampedRelativePitch, ClampedRelativeYaw, 0.0f);

	// 6. �� ��� ȸ���� ĳ������ ActorRotation�� �ٽ� �����Ͽ�,
	// ���� �������� ���� ������ �ٶ� '���ѵ�' ������ ����մϴ�.
	FRotator LookAtWorldRotation = ActorRotation + ClampedRelativeRotator;

	// 7. 'neck_01' ���� ���� ��ġ�� �����ɴϴ�. �̰��� �ü��� '������'�� �˴ϴ�.
	FVector NeckBoneLocation = FVector::ZeroVector;
	if (Me && Me->GetMesh())
	{
		NeckBoneLocation = Me->GetMesh()->GetBoneLocation(FName(TEXT("neck_01")));
		// ���� neck_01 ���� ���ٸ� ������ �ٸ� �� (��: head)�� ����ϼ���.
	}
	else
	{
		// �޽ð� ������ ĳ������ ��ġ���� �����ϵ��� ���� (��: ������)
		NeckBoneLocation = Me->GetActorLocation() + FVector(0, 0, Me->GetDefaultHalfHeight());
	}


	// 8. LookAtWorldRotation�� ����Ű�� �������� ���� �Ÿ� ������ ��ǥ ������ ����մϴ�.
	float LookAtDistance = 500.0f; // ���� �ٶ� ������ ��ǥ ���������� �Ÿ�
	LookAtTarget = NeckBoneLocation + LookAtWorldRotation.Vector() * LookAtDistance;
	bActiveLookAt = true;
}

void USG_KratosAnim::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void USG_KratosAnim::AnimNotify_AttackEndCheck()
{
	OnAttackEndCheck.Broadcast();
}

void USG_KratosAnim::AnimNotify_AbleNextAttack()
{
	Me->CanComboAttack = true;
}

void USG_KratosAnim::AnimNotify_NextWeakAttackCheck()
{
	Me->CanComboAttack = true;
}

void USG_KratosAnim::AnimNotify_MovableCheck()
{
	OnMovableCheck.Broadcast();
}

void USG_KratosAnim::AnimNotify_HideAxe()
{
	UE_LOG(LogTemp, Display, TEXT("HideAxe"));
	if (Me)
	{
		Me->OnHideAxe();
		Me->ThrowAxe();
	}
}

void USG_KratosAnim::AnimNotify_TimeDilation()
{
	Me->SetGlobalTimeDilation(0.02f, 0.06f);
}

void USG_KratosAnim::AnimNotify_FieldSpawn()
{
	GetWorld()->SpawnActor<ARuneAttackField>(RuneAttackFieldFactory, Me->GetActorTransform());
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

void USG_KratosAnim::AnimNotify_GuardDisappear()
{
	Me->OnMyGuardDisappear();
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

void USG_KratosAnim::AnimNotify_DelayStart()
{
	Montage_SetPlayRate(AxeThrowMontage, 0.5f);
}

void USG_KratosAnim::AnimNotify_DelayEnd()
{
	Montage_SetPlayRate(AxeThrowMontage, 1.0f);

}

void USG_KratosAnim::AnimNotify_EndDodge()
{
	Me->bEvade = false;
}

void USG_KratosAnim::AnimNotify_AppearShield()
{
	Me->Shield->SetTargetScale(true);
}

void USG_KratosAnim::AnimNotify_DisappearShield()
{
	Me->Shield->SetTargetScale(false);
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

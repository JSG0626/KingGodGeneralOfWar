// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KratosState.h"
#include "Kratos.h"
#include <Kismet/KismetMathLibrary.h>
#include "SG_KratosAnim.h"

void UKratosState::StateLog(const FString& message, bool isTickLog) const
{
	if (bShowLog && (!isTickLog || bShowTickLog))
	{
		FString log;
		FDateTime UtcNow = FDateTime::UtcNow();

		FDateTime Now = FDateTime::Now();

		FString DefaultTimeString = Now.ToString();

		FString CustomTimeStringMilliseconds = Now.ToString(TEXT("%Y-%m-%d %H:%M:%S.%s\t:"));
		log = CustomTimeStringMilliseconds + message;
		// 예시 출력: "2025-07-29 23:29:27.123"
		if (is_uelog)
		{
			UE_LOG(LogTemp, Display, TEXT("%s"), *log);
		}
		if (is_dm)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Cyan, FString::Printf(TEXT("%s"), *log));
		}
	}
}

void UKratosState::LookAtProcess(float DeltaTime, FRotator offset)
{
	FRotator ActorRotation = Me->GetActorRotation();
	FRotator ControlRotation = Me->GetControlRotation() + offset;
	FRotator RelativeControlRotation = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation);
	if (RelativeControlRotation.Yaw <= -30 || RelativeControlRotation.Yaw >= 90) return;
		
	float ClampedRelativePitch = FMath::ClampAngle(RelativeControlRotation.Pitch, -20.0f, 20.0f); // 상하 제한

	// 5. 제한된 상대적인 Pitch와 Yaw를 사용하여 새로운 회전 값을 만듭니다.
	// Roll은 보통 0으로 유지합니다 (고개가 옆으로 기울어지는 것은 드물기 때문).
	FRotator ClampedRelativeRotator = FRotator(ClampedRelativePitch, RelativeControlRotation.Yaw, 0.0f);

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
	TargetLookAtPos = NeckBoneLocation + LookAtWorldRotation.Vector() * LookAtDistance;
	Anim->LookAtTarget = FMath::Lerp(Anim->LookAtTarget, TargetLookAtPos, DeltaTime * 3);
}

void UKratosState::SetUp(AKratos* kratos)
{
	Me = kratos; 
	check(Me);
	Anim = Me->Anim; 
}

void UKratosState::HandleHit(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Hit, params);
}

void UKratosState::HandleDie(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Die, params);
}

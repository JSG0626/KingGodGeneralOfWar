// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosStates/KratosState.h"
#include "Kratos.h"
#include <Kismet/KismetMathLibrary.h>
#include "SG_KratosAnim.h"

void IKratosState::StateLog(const FString& message, bool isTickLog) const
{
	if (bShowLog && (!isTickLog || bShowTickLog))
	{
		FString log;
		FDateTime UtcNow = FDateTime::UtcNow();

		FDateTime Now = FDateTime::Now();

		FString DefaultTimeString = Now.ToString();

		FString CustomTimeStringMilliseconds = Now.ToString(TEXT("%Y-%m-%d %H:%M:%S.%s\t:"));
		log = CustomTimeStringMilliseconds + message;
		// ���� ���: "2025-07-29 23:29:27.123"
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

void IKratosState::LookAtProcess(FRotator offset)
{
	FRotator ActorRotation = Me->GetActorRotation();
	FRotator ControlRotation = Me->GetControlRotation() + offset;
	FRotator RelativeControlRotation = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation);
	float ClampedRelativePitch = FMath::ClampAngle(RelativeControlRotation.Pitch, -20.0f, 20.0f); // ���� ����
	float ClampedRelativeYaw = FMath::ClampAngle(RelativeControlRotation.Yaw, -45.0f, 45.0f);   // �¿� ����

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
	Anim->LookAtTarget = NeckBoneLocation + LookAtWorldRotation.Vector() * LookAtDistance;
}

void IKratosState::SetUp(AKratos* kratos)
{
	Me = kratos; 
	check(Me);
	Anim = Me->Anim; 
}

void IKratosState::HandleHit(const FGenericStateParams& params)
{
	Me->SetKratosState(EPlayerState::Hit, params);
}

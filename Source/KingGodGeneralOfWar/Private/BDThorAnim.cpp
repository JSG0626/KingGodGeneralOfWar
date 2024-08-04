// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThorAnim.h"
#include "BDThor.h"
#include "BDThorFSM.h"
#include "BDThorMjolnir.h"
#include "Kismet/GameplayStatics.h"

void UBDThorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	/*if (bdThor == nullptr || bdThor->IsValidLowLevel()) {
		auto ownerPawn = TryGetPawnOwner();
		bdThor = Cast<ABDThor>(ownerPawn);

	}

	if (bdThorFSM == nullptr || bdThorFSM->IsValidLowLevel()) {
		bdThorFSM = Cast<UBDThorFSM>(bdThor->fsm);
	}*/

	if (!bdThor) {
		auto ownerPawn = TryGetPawnOwner();
		//auto* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ABDThor::StaticClass());
		bdThor = Cast<ABDThor>(ownerPawn);
	}

	if (bdThor && !bdThorFSM) {
		bdThorFSM = Cast<UBDThorFSM>(bdThor->fsm);
	}

	if (!Mjolnir) {
		auto* actor = UGameplayStatics::GetActorOfClass(GetWorld(), ABDThorMjolnir::StaticClass());
		Mjolnir = Cast<ABDThorMjolnir>(actor);
	}
}


void UBDThorAnim::playBDHammerThrow()
{
	if (!Montage_IsPlaying(BDHammerThrowMontage)) {
		Montage_Play(BDHammerThrowMontage);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Throw Animation"));
}

void UBDThorAnim::playBDHammerWind()
{
	if (!Montage_IsPlaying(BDHammerWindMontage)) {
		Montage_Play(BDHammerWindMontage);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Playing Hammer Wind Animation"));
}

void UBDThorAnim::playBDBackDodge()
{
	if (!Montage_IsPlaying(BDThorBackDodgeMontage)) {
		Montage_Play(BDThorBackDodgeMontage); //�ڷ� ȸ��
	}
}

void UBDThorAnim::playBDRightDodge()
{
	if (!Montage_IsPlaying(BDThorRightDodgeMontage)) {
		Montage_Play(BDThorRightDodgeMontage); //���������� ȸ��
	}
}

void UBDThorAnim::playBDDamage()
{
	if (!Montage_IsPlaying(BDThorDamageMontage)) {
		Montage_Play(BDThorDamageMontage); //�������� �߻��� �� ����
	}
}

void UBDThorAnim::playBDAttackRandomState()
{
	if (!Montage_IsPlaying(BDThorAttackRandomMontage)) {
		Montage_Play(BDThorAttackRandomMontage); //���� ���� ���� ���°� �߻��� ��
	}
}

void UBDThorAnim::playBDHammerThreeSwing()
{
	if (!Montage_IsPlaying(BDHammerThreeSwingMontage)) {
		Montage_Play(BDHammerThreeSwingMontage); //���� ���� ���� ���°� �߻��� ��
	}
}

void UBDThorAnim::AnimNotify_AnimEnd()
{
	//UE_LOG(LogTemp, Warning, TEXT("EndAnim"));
	if (bdThorFSM) {
		bdThorFSM->BDEndState(); // �ִϸ��̼��� ���� �� ����
		//UE_LOG(LogTemp, Warning, TEXT("Succed"));
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("Faild"));
	}
}

//BDHammerThrow ��Ƽ����
void UBDThorAnim::AnimNotify_ThrowTiming()
{
	bdThor->BDHammerThrowHit();
}

void UBDThorAnim::AnimNotify_SlashWind()
{
	//������ ������
	bdThor->BDHammerWindSlash();
}

void UBDThorAnim::AnimNotify_BackMjolnir()
{
	Mjolnir->BackMjolnir(); //���ϸ��� ���ƿ����� ȣ��
	UE_LOG(LogTemp, Warning, TEXT("Notify Back1"));
}

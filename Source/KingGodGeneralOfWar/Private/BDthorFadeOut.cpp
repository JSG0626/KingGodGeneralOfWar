// Fill out your copyright notice in the Description page of Project Settings.


#include "BDthorFadeOut.h"
#include "Kismet/GameplayStatics.h"

void UBDthorFadeOut::NativeConstruct()
{
	Super::NativeConstruct();

	FadeOutAnimStart();

}

void UBDthorFadeOut::FadeOutAnimStart()
{
	PlayAnimationForward(StartAnim); //���̵� �ƿ� �ִϸ��̼� ����

	//UGameplayStatics::OpenLevel(GetWorld(), FName("CSWMap")); //���� ����

	// 2�� �Ŀ� OnFadeOutCompleted �Լ��� ȣ���ϴ� Ÿ�̸Ӹ� �����մϴ�.
	GetWorld()->GetTimerManager().SetTimer(handle, this, &UBDthorFadeOut::OnFadeOutCompleted, 3.0f, false);

}

void UBDthorFadeOut::OnFadeOutCompleted()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("CSWMap")); // ���� ����
}


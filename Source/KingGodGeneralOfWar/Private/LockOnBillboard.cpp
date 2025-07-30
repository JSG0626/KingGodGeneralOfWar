// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnBillboard.h"

ULockOnBillboard::ULockOnBillboard()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UTexture2D> DefaultSpriteAsset(TEXT("/Script/Engine.Texture2D'/Game/JSG/UI/Images/Lock_On_UI_IMG.Lock_On_UI_IMG'"));

	if (DefaultSpriteAsset.Succeeded())
	{
		Sprite = DefaultSpriteAsset.Object; // UBillboardComponent�� Sprite ��� ������ �Ҵ�
		// ��������Ʈ ũ�� ���� (���� ����)
		SetSprite(Sprite); // SetSprite �Լ� ȣ�� (Sprite ������ �Ҵ� �Ŀ��� �ѹ� �� ȣ�����ִ� ���� ����)
		//SetSpriteSize(50.0f, true); // ��������Ʈ ũ�� ���� (��: 50 ����, ���� ���μ��� ����)
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load default sprite for LockOnBillboard!"));
	}
}

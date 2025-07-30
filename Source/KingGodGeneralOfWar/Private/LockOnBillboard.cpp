// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnBillboard.h"

ULockOnBillboard::ULockOnBillboard()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UTexture2D> DefaultSpriteAsset(TEXT("/Script/Engine.Texture2D'/Game/JSG/UI/Images/Lock_On_UI_IMG.Lock_On_UI_IMG'"));

	if (DefaultSpriteAsset.Succeeded())
	{
		Sprite = DefaultSpriteAsset.Object; // UBillboardComponent의 Sprite 멤버 변수에 할당
		// 스프라이트 크기 조절 (선택 사항)
		SetSprite(Sprite); // SetSprite 함수 호출 (Sprite 변수에 할당 후에도 한번 더 호출해주는 것이 안전)
		//SetSpriteSize(50.0f, true); // 스프라이트 크기 설정 (예: 50 유닛, 고정 가로세로 비율)
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load default sprite for LockOnBillboard!"));
	}
}

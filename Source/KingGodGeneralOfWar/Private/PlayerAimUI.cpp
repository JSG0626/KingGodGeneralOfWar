// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAimUI.h"
#include <Components/Image.h>
void UPlayerAimUI::ChangeColor(bool Red)
{
    if (IMG_CenterUI)
    {
        // 1. 현재 브러시를 가져와 복사합니다.
        FSlateBrush CurrentBrush = IMG_CenterUI->GetBrush();

        // 2. 복사된 브러시의 TintColor를 수정합니다.
        CurrentBrush.TintColor = FSlateColor(Red ? FColor::Red : FColor::White);

        // 3. 수정된 브러시를 UImage에 다시 설정합니다.
        IMG_CenterUI->SetBrush(CurrentBrush);
    }
}

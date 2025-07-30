// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAimUI.h"
#include <Components/Image.h>
void UPlayerAimUI::ChangeColor(bool Red)
{
    if (IMG_CenterUI)
    {
        // 1. ���� �귯�ø� ������ �����մϴ�.
        FSlateBrush CurrentBrush = IMG_CenterUI->GetBrush();

        // 2. ����� �귯���� TintColor�� �����մϴ�.
        CurrentBrush.TintColor = FSlateColor(Red ? FColor::Red : FColor::White);

        // 3. ������ �귯�ø� UImage�� �ٽ� �����մϴ�.
        IMG_CenterUI->SetBrush(CurrentBrush);
    }
}

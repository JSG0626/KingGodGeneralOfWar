#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

/*
�� Ŭ������ ������ �ʿ䰡 �����ϴ�.
���÷��� �ý��� ��������Ƽ�� ���� �� Ŭ�����Դϴ�.
UINTERFACE ��ũ�θ� ����մϴ�.
UInterface���� ����մϴ�.
*/
UINTERFACE(MinimalAPI, Blueprintable)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/* ���� �������̽� ����. */
class KINGGODGENERALOFWAR_API IWeaponInterface
{
	GENERATED_BODY()

	// �� Ŭ������ �������̽� �Լ��� �߰��մϴ�. �� Ŭ������ �� �������̽����� �����ϵ��� ��ӵ� Ŭ�����Դϴ�.
public:
	void ActiveHitCollision(bool ActiveState);
	// ���⿡ �������̽� �Լ� ������ �߰��մϴ�
};
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

/*
이 클래스는 수정할 필요가 없습니다.
리플렉션 시스템 비저빌리티에 대한 빈 클래스입니다.
UINTERFACE 매크로를 사용합니다.
UInterface에서 상속합니다.
*/
UINTERFACE(MinimalAPI, Blueprintable)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/* 실제 인터페이스 선언. */
class KINGGODGENERALOFWAR_API IWeaponInterface
{
	GENERATED_BODY()

	// 이 클래스에 인터페이스 함수를 추가합니다. 이 클래스는 이 인터페이스에서 구현하도록 상속될 클래스입니다.
public:
	void ActiveHitCollision(bool ActiveState);
	// 여기에 인터페이스 함수 선언을 추가합니다
};
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AbilitySystemHUD.generated.h"

UCLASS()
class AAbilitySystemHUD : public AHUD
{
	GENERATED_BODY()

public:
	AAbilitySystemHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};


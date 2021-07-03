// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystemGameMode.h"
#include "AbilitySystemHUD.h"
#include "AbilitySystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAbilitySystemGameMode::AAbilitySystemGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAbilitySystemHUD::StaticClass();
}

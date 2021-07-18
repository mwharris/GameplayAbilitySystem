#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityTypes.generated.h"

UENUM(BlueprintType)
enum class EAbilityCostType : uint8
{
	Health,
	Mana,
	Strength
};

USTRUCT(BlueprintType)
struct FGameplayAbilityInfo
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityInfo")
	float CooldownDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityInfo")
	float Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityInfo")
	EAbilityCostType CostType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityInfo")
	UMaterialInstance* UIMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityInfo")
	TSubclassOf<class UGameplayAbilityBase> AbilityClass;

	FGameplayAbilityInfo();
	FGameplayAbilityInfo(
		float InDuration, 
		float InCost, 
		EAbilityCostType InCostType, 
		UMaterialInstance* InUIMaterial, 
		TSubclassOf<class UGameplayAbilityBase> InAbilityClass
	);

};
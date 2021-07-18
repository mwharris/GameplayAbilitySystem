#include "Abilities/AbilityTypes.h"

FGameplayAbilityInfo::FGameplayAbilityInfo() 
	: CooldownDuration(0), Cost(0), CostType(EAbilityCostType::Mana), UIMaterial(nullptr), AbilityClass(nullptr)
{
	
}

FGameplayAbilityInfo::FGameplayAbilityInfo(float InDuration, 
		float InCost, 
		EAbilityCostType InCostType, 
		UMaterialInstance* InUIMaterial, 
		TSubclassOf<class UGameplayAbilityBase> InAbilityClass) 
{
	CooldownDuration = InDuration;
	Cost = InCost;
	CostType = InCostType;
	UIMaterial = InUIMaterial;
	AbilityClass = InAbilityClass;
}
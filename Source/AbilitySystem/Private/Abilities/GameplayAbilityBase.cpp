#include "Abilities/GameplayAbilityBase.h"

FGameplayAbilityInfo UGameplayAbilityBase::GetAbilityInfo() 
{
    UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
    UGameplayEffect* CostEffect = GetCostGameplayEffect();

    if (CooldownEffect && CostEffect) 
    {
        // Get Cooldown Gameplay Effect information
        float CooldownDuration = 0;
        CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, CooldownDuration);
        
        // Get Cost Gameplay Effect information
        float Cost = 0;
        EAbilityCostType CostType;
        // Cost Gameplay Effects can have an Array of modifiers - we're only using 1 though
        if (CostEffect->Modifiers.Num() > 0) 
        {
            // Get the Cost Magnitude from the Gameplay Effect
            FGameplayModifierInfo EffectInfo = CostEffect->Modifiers[0];
            // This call only succeeds if Magnitude Calculation Type = Scalable Float
            EffectInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost);
            // Set our Cost Type by checking the Attribute Name
            FGameplayAttribute CostAttribute = EffectInfo.Attribute;
            FString CostAttributeName = CostAttribute.AttributeName();
            if (CostAttributeName == "Health") { CostType = EAbilityCostType::Health; }
            else if (CostAttributeName == "Mana") { CostType = EAbilityCostType::Mana; }
            else if (CostAttributeName == "Strength") { CostType = EAbilityCostType::Strength; }
        }
        
        return FGameplayAbilityInfo(CooldownDuration, Cost, CostType, UIMaterial, GetClass());
    }

    return FGameplayAbilityInfo();
}
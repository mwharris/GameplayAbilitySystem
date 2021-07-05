#include "AttributeSets/AttributeSetBase.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"

UAttributeSetBase::UAttributeSetBase() : Health(200.f), MaxHealth(200.f)
{
	
}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) 
{
    // Get the FProperty that the GameplayEffect has modified
    FProperty* DataProperty = Data.EvaluatedData.Attribute.GetUProperty();
    // Get the FProperty representing the Health property of the UAttributeSetBase class
    FProperty* HealthProperty = FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health));
    // Check if the modified value is the Health property
    if (DataProperty == HealthProperty)
    {
        // Make sure our HP doesn't go below 0
        Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
        Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetBaseValue()));
        // Broadcast to anyone listening for Health Changed events
        OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
        UE_LOG(LogTemp, Warning, TEXT("Ouch, I took damage, my HP is: %f"), Health.GetCurrentValue());
    }
}
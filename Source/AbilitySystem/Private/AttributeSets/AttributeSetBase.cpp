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
        UE_LOG(LogTemp, Warning, TEXT("Ouch, I took damage, my HP is: %f"), Health.GetCurrentValue());
        OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
    }
}
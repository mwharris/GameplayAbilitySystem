#include "AttributeSets/AttributeSetBase.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/Public/Characters/CharacterBase.h"

UAttributeSetBase::UAttributeSetBase() : 
    Health(200.f), MaxHealth(200.f), 
    Mana(100.f), MaxMana(100.f), 
    Strength(250.f), MaxStrength(250.f)
{
	
}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) 
{
    // Check if FProperty that the GameplayEffect modified is Health
    if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)))
    {
        // Make sure our HP doesn't go below 0
        Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
        Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetBaseValue()));
        // Broadcast to anyone listening for Health Changed events
        OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
        // Add/Remove our Full Health Tag appropriately
        if (OwningCharacter == nullptr) { GetOwningCharacterRef(); }
        if (Health.GetCurrentValue() == MaxHealth.GetCurrentValue()) 
        {
            OwningCharacter->AddGameplayTag(OwningCharacter->FullHealthTag);
        }
        else 
        {
            OwningCharacter->RemoveGameplayTag(OwningCharacter->FullHealthTag);
        }
    }

    // Check if FProperty that the GameplayEffect modified is Mana
    if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Mana)))
    {
        // Make sure our Mana doesn't go below 0
        Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.f, MaxMana.GetCurrentValue()));
        Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.f, MaxMana.GetBaseValue()));
        // Broadcast to anyone listening for Health Changed events
        OnManaChange.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
    }

    // Check if FProperty that the GameplayEffect modified is Strength
    if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Strength)))
    {
        // Make sure our Strength doesn't go below 0
        Strength.SetCurrentValue(FMath::Clamp(Strength.GetCurrentValue(), 0.f, MaxStrength.GetCurrentValue()));
        Strength.SetBaseValue(FMath::Clamp(Strength.GetBaseValue(), 0.f, MaxStrength.GetBaseValue()));
        // Broadcast to anyone listening for Health Changed events
        OnStrengthChange.Broadcast(Strength.GetCurrentValue(), MaxStrength.GetCurrentValue());
    }
}

void UAttributeSetBase::GetOwningCharacterRef() 
{
	if (OwningCharacter != nullptr) return;
    OwningCharacter = Cast<ACharacterBase>(GetOwningActor());
    if (OwningCharacter == nullptr) 
    {
        UE_LOG(LogTemp, Error, TEXT("Couldn't find Owning Character!"));
    }
}
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AttributeSetBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangeDelegate, float, Health, float, MaxHealth);

UCLASS()
class ABILITYSYSTEM_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSetBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute Set Base")
	FGameplayAttributeData Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute Set Base")
	FGameplayAttributeData MaxHealth;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	FOnHealthChangeDelegate OnHealthChange;

};
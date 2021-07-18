#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/AbilityTypes.h"
#include "GameplayAbilityBase.generated.h"

UCLASS()
class ABILITYSYSTEM_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AbilityBase")
	class UMaterialInstance* UIMaterial;

	UFUNCTION(BlueprintCallable, Category="AbilityBase")
	FGameplayAbilityInfo GetAbilityInfo();

};
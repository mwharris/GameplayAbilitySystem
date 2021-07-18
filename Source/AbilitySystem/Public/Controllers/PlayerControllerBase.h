#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Abilities/AbilityTypes.h"
#include "PlayerControllerBase.generated.h"

UCLASS()
class ABILITYSYSTEM_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="PlayerControllerBase")
	void AddAbilityToUI(FGameplayAbilityInfo AbilityToAdd);

};

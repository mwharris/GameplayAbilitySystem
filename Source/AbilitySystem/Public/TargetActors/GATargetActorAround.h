#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATargetActorAround.generated.h"

UCLASS()
class ABILITYSYSTEM_API AGATargetActorAround : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn=true), Category="Find Around")
	float Radius;

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

private:
	APawn* MasterPawn;

};

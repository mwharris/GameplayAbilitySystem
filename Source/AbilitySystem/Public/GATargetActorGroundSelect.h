#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATargetActorGroundSelect.generated.h"

UCLASS()
class ABILITYSYSTEM_API AGATargetActorGroundSelect : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn=true), Category="Ground Select")
	float Radius;
		
	UFUNCTION(BlueprintCallable, Category="Ground Select")
	bool GetPlayerLookPoint(FVector& OutViewPoint);

	AGATargetActorGroundSelect();
	virtual void Tick(float DeltaTime) override;
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

private:
	APawn* MasterPawn;

};
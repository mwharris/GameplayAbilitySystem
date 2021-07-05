#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterBase.generated.h"

class UAttributeSetBase;

UCLASS()
class ABILITYSYSTEM_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAttributeSetBase* AttributeSetComp;

	UFUNCTION(BlueprintCallable, Category="CharacterBase")
	void AcquireAblity(TSubclassOf<UGameplayAbility> AbilityToAcquire);
	UFUNCTION(BlueprintImplementableEvent, Category="CharacterBase", meta=(DisplayName="OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);
	UFUNCTION(BlueprintImplementableEvent, Category="CharacterBase", meta=(DisplayName="Die"))
	void BP_Die();
	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth);

protected:
	bool bIsDead = false;

	virtual void BeginPlay() override;

};


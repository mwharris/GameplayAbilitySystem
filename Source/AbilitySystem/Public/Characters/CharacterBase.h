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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAttributeSetBase* AttributeSetComp;

	UFUNCTION(BlueprintCallable, Category="CharacterBase")
	void AcquireAblity(TSubclassOf<UGameplayAbility> AbilityToAcquire);
	UFUNCTION(BlueprintCallable, Category="CharacterBase")
	bool IsOtherHostile(ACharacterBase* Other);

	UFUNCTION(BlueprintImplementableEvent, Category="CharacterBase", meta=(DisplayName="Die"))
	void BP_Die();
	
	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth);
	UFUNCTION(BlueprintImplementableEvent, Category="CharacterBase", meta=(DisplayName="OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);
	
	UFUNCTION()
	void OnManaChanged(float Mana, float MaxMana);
	UFUNCTION(BlueprintImplementableEvent, Category="CharacterBase", meta=(DisplayName="OnManaChanged"))
	void BP_OnManaChanged(float Mana, float MaxMana);
	
	UFUNCTION()
	void OnStrengthChanged(float Strength, float MaxStrength);
	UFUNCTION(BlueprintImplementableEvent, Category="CharacterBase", meta=(DisplayName="OnStrengthChanged"))
	void BP_OnStrengthChanged(float Strength, float MaxStrength);

	ACharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	uint8 GetTeamId() const { return TeamId; }

protected:
	bool bIsDead = false;
	uint8 TeamId;

	virtual void BeginPlay() override;
	void AutoDetermineTeamIdByControllerType();
	void Dead();

};


#include "Characters/CharacterBase.h"
#include "AIController.h"
#include "AttributeSets/AttributeSetBase.h"
#include "BrainComponent.h"
#include "GameFramework/PlayerController.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("Ability System Component");

	AttributeSetComp = CreateDefaultSubobject<UAttributeSetBase>("Attribute Set Component");
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	// Identify us as either the Player or the Enemy
	AutoDetermineTeamIdByControllerType();
	// Make sure to add our FullHealthTag when we start with Full Health
	AddGameplayTag(FullHealthTag);
	// Subscribe to AttributeSetBase event delegates
	AttributeSetComp->OnHealthChange.AddDynamic(this, &ACharacterBase::OnHealthChanged);
	AttributeSetComp->OnManaChange.AddDynamic(this, &ACharacterBase::OnManaChanged);
	AttributeSetComp->OnStrengthChange.AddDynamic(this, &ACharacterBase::OnStrengthChanged);
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::AcquireAblity(TSubclassOf<UGameplayAbility> AbilityToAcquire) 
{
	if (!AbilitySystemComp) return;
	if (HasAuthority() && AbilityToAcquire) 
	{
		// Create an ability (with spec) to give our AbilitySystemComponent
		FGameplayAbilitySpecDef SpecDef = FGameplayAbilitySpecDef();
		SpecDef.Ability = AbilityToAcquire;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(SpecDef, 1);
		AbilitySystemComp->GiveAbility(AbilitySpec);
	}
	// Initializes the Abilitiy's ActorInfo
	// Argument 1 is OwnerActor (the actor that logically owns the component)
	// Argument 2 is AvatarActor (what physical actor we are effecting in the world)
	AbilitySystemComp->InitAbilityActorInfo(this, this);
}

void ACharacterBase::AddGameplayTag(FGameplayTag& TagToAdd) 
{
	AbilitySystemComp->AddLooseGameplayTag(TagToAdd);
	AbilitySystemComp->SetTagMapCount(TagToAdd, 1);
}

void ACharacterBase::RemoveGameplayTag(FGameplayTag& TagToRemove) 
{
	AbilitySystemComp->RemoveLooseGameplayTag(TagToRemove);
}

void ACharacterBase::OnHealthChanged(float Health, float MaxHealth) 
{
	if (Health <= 0.f && !bIsDead) 
	{
		bIsDead = true;
		Dead();
		BP_Die();
	}
	// Call our Blueprint version of his function
	BP_OnHealthChanged(Health, MaxHealth);
}

void ACharacterBase::OnManaChanged(float Mana, float MaxMana) 
{
	BP_OnManaChanged(Mana, MaxMana);
}

void ACharacterBase::OnStrengthChanged(float Strength, float MaxStrength) 
{
	BP_OnStrengthChanged(Strength, MaxStrength);
}

void ACharacterBase::Dead() 
{
	if (APlayerController* PC = Cast<APlayerController>(GetController())) 
	{
		PC->DisableInput(PC);
		return;
	}
	else if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->GetBrainComponent()->StopLogic("Dead");
		return;
	}
}

void ACharacterBase::AutoDetermineTeamIdByControllerType() 
{
	if (GetController() && GetController()->IsPlayerController()) 
	{
		TeamId = 0;
	}
	else
	{
		TeamId = 1;
	}
}

bool ACharacterBase::IsOtherHostile(ACharacterBase* Other) 
{
	return TeamId != Other->GetTeamId();	
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}
#include "Characters/CharacterBase.h"
#include "AttributeSets/AttributeSetBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("Ability System Component");

	AttributeSetComp = CreateDefaultSubobject<UAttributeSetBase>("Attribute Set Component");
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	// Subscribe to AttributeSetBase::FOnHealthChangeDelegate
	AttributeSetComp->OnHealthChange.AddDynamic(this, &ACharacterBase::OnHealthChanged);
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

void ACharacterBase::OnHealthChanged(float Health, float MaxHealth) 
{
	if (Health <= 0.f && !bIsDead) 
	{
		bIsDead = true;
		BP_Die();
	}
	// Call our Blueprint version of his function
	BP_OnHealthChanged(Health, MaxHealth);
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}
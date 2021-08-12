#include "GATargetActorGroundSelect.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/PlayerController.h"

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	AGameplayAbilityTargetActor::StartTargeting(Ability);
    MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGameplayAbilityTargetActor::ConfirmTargetingAndContinue()
{
    // First get the owning Actor's viewpoint
    FVector OutLocation;
    FRotator OutRotator;
    MasterPC->GetPlayerViewPoint(OutLocation, OutRotator);

    // Setup variables for the a line trace
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = true;
    if (MasterPC->GetPawn()) {
        QueryParams.AddIgnoredActor(MasterPC->GetPawn());
    }

    // Perform the line trace in the direction of the viewpoint
    FVector LookAtPoint;
    FVector End = OutLocation + OutRotator.Vector() * 10000.f; 
    bool TraceResult = GetWorld()->LineTraceSingleByChannel(HitResult, OutLocation, End, ECC_Visibility, QueryParams);
    if (TraceResult) {
        LookAtPoint = HitResult.ImpactPoint;
    }

    // TODO: 
}
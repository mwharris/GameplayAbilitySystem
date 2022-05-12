#include "TargetActors/GATargetActorAround.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

void AGATargetActorAround::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
    MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
    MasterPawn = MasterPC->GetPawn();
    if (!MasterPawn) {
        UE_LOG(LogTemp, Error, TEXT("GATargetActorGoundSelect: Could not find Master Pawn!"));
    }
}

void AGATargetActorAround::ConfirmTargetingAndContinue() 
{
	TArray<FOverlapResult> Overlaps;
    TArray<TWeakObjectPtr<AActor>> OverlappedActors;
    // Don't execute this function if we don't have a Pawn
    if (!MasterPawn) 
    {
        UE_LOG(LogTemp, Error, TEXT("ConfirmTargetingAndContinue: Could not find Master Pawn!"));
        TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
        return;
    }
    // Create collision query params for sphere trace
    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = false;
    QueryParams.bReturnPhysicalMaterial = false;
    if (MasterPawn) {
        QueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());
    }
    // Overlap sphere with a given radius checking for Pawns
    bool OverlapResult = GetWorld()->OverlapMultiByObjectType(
        Overlaps, 
        MasterPawn->GetActorLocation(), 
        FQuat::Identity, 
        FCollisionObjectQueryParams(ECC_Pawn), 
        FCollisionShape::MakeSphere(Radius),
        QueryParams
    );
    // Loop through our results (if any)
    if (OverlapResult) {
        for (int32 i = 0; i < Overlaps.Num(); i++)
        {
            // Keep track of all the pawns that we overlapped
            APawn* OverlappedPawn = Cast<APawn>(Overlaps[i].GetActor()); 
            if (OverlappedPawn && !OverlappedActors.Contains(OverlappedPawn)) {
                OverlappedActors.Add(OverlappedPawn);
            }
        }
    }
    // Determine if we hit any actors
    if (OverlappedActors.Num() > 0) {
        // Create an FGameplayAbilityTargetDataHandle with OverlappedActors as the data
        FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappedActors);
        // Perform the broadcast to trigger Wait Target Data blueprint
        TargetDataReadyDelegate.Broadcast(TargetData);
    }
    else
    {
        TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
    }
}
#include "GATargetActorGroundSelect.h"
#include "Abilities/GameplayAbility.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

AGATargetActorGroundSelect::AGATargetActorGroundSelect() 
{
    PrimaryActorTick.bCanEverTick = true;	
}

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
    MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
    MasterPawn = MasterPC->GetPawn();
    if (!MasterPawn) {
        UE_LOG(LogTemp, Error, TEXT("GATargetActorGoundSelect: Could not find Master Pawn!"));
    }
}

void AGATargetActorGroundSelect::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
    // Constantly ddraw a debug sphere to visualize our targeting
    FVector ViewPoint;
    GetPlayerLookPoint(ViewPoint);
	DrawDebugSphere(GetWorld(), ViewPoint, Radius, 32, FColor::Red, false, -1, 0, 3.0f);
}

void AGATargetActorGroundSelect::ConfirmTargetingAndContinue()
{
    TArray<FOverlapResult> Overlaps;
    TArray<TWeakObjectPtr<AActor>> OverlappedActors;
    // Get our viewpoint
    FVector ViewLocation;
    GetPlayerLookPoint(ViewLocation);
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
        ViewLocation, 
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
    // Broadcast our targeting results and overlapped actors
    if (OverlappedActors.Num() > 0) {
        FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappedActors);
        TargetDataReadyDelegate.Broadcast(TargetData);
    }
    else
    {
        TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
    }
}

bool AGATargetActorGroundSelect::GetPlayerLookPoint(OUT FVector& OutViewPoint) 
{
	// First get the owning Actor's viewpoint
    FVector OutLocation;
    FRotator OutRotator;
    MasterPC->GetPlayerViewPoint(OutLocation, OutRotator);
    // Setup variables for the line trace
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = true;
    if (MasterPawn) {
        QueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());
    }
    // Perform the line trace in the direction of the viewpoint
    FVector End = OutLocation + OutRotator.Vector() * 10000.f; 
    bool TraceResult = GetWorld()->LineTraceSingleByChannel(HitResult, OutLocation, End, ECC_Visibility, QueryParams);
    if (TraceResult) {
        OutViewPoint = HitResult.ImpactPoint;
    }
    else 
    {
        OutViewPoint = FVector();
    }
    // Return if our trace was successful or not
    return TraceResult;
}
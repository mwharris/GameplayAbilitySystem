#include "GATargetActorGroundSelect.h"
#include "Abilities/GameplayAbility.h"
#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

AGATargetActorGroundSelect::AGATargetActorGroundSelect() 
{
    PrimaryActorTick.bCanEverTick = true;	
    // Setup root scene component and decal
    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(RootComp);
    Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
    Decal->SetupAttachment(RootComp);
    // Initialize radius
    Radius = 200.0f;
}

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
    // Cache references to the owner's PlayerController and Pawn
    MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
    MasterPawn = MasterPC->GetPawn();
    if (!MasterPawn) {
        UE_LOG(LogTemp, Error, TEXT("GATargetActorGoundSelect: Could not find Master Pawn!"));
    }
    // Initialize decal size
    Decal->DecalSize = FVector(Radius);
}

void AGATargetActorGroundSelect::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
    // Draw a decal at the point the player is targeting
    FVector ViewPoint;
    GetPlayerLookPoint(ViewPoint);
    Decal->SetWorldLocation(ViewPoint);
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
    // Create location information to add to the Target Data
    FGameplayAbilityTargetData_LocationInfo* CenterLocation = new FGameplayAbilityTargetData_LocationInfo();
    CenterLocation->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
    CenterLocation->TargetLocation.LiteralTransform = Decal->GetComponentTransform();
    // Determine if we hit any actors
    if (OverlappedActors.Num() > 0) {
        // Create an FGameplayAbilityTargetDataHandle with OverlappedActors as the data
        FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappedActors);
        TargetData.Add(CenterLocation);
        // Perform the broadcast to trigger Wait Target Data blueprint
        TargetDataReadyDelegate.Broadcast(TargetData);
    }
    else
    {
        TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle(CenterLocation));
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
// Copyright Dr. Matthias Hölzl

#include "Interaction/InteractionSourceComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "InkLab/InkLab.h"
#include "Interaction/InteractionTargetComponent.h"

static TAutoConsoleVariable<int32> CVarDebugInteractionSource(
    TEXT("debug.InteractionSource"), 0, TEXT("Show a debug line for the trace towards interaction targets.")
);

UInteractionSourceComponent::UInteractionSourceComponent(const FObjectInitializer& ObjectInitializer)
    : Super{ObjectInitializer}
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionSourceComponent::TickComponent(
    float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction
)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    PerformInteractionTrace();
}

void UInteractionSourceComponent::TriggerInteraction()
{
    OnInteractionTriggered.Broadcast(CurrentTarget);
    if (CurrentTarget)
    {
        CurrentTarget->OnInteract(this);
    }
}

void UInteractionSourceComponent::BeginPlay() { Super::BeginPlay(); }

void UInteractionSourceComponent::PerformInteractionTrace()
{
    const APawn* OwningPawn = Cast<APawn>(GetOwner());
    if (!OwningPawn)
    {
        return;
    }

    const APlayerController* PC = Cast<APlayerController>(OwningPawn->GetController());
    if (!PC)
    {
        return;
    }

    const auto [TraceStart, TraceEnd] = ComputeInteractionTraceEndpoints(*PC);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwningPawn);

    FHitResult HitResult;
    const UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    if (TraceRadius > 0)
    {
        World->SweepSingleByChannel(
            HitResult, TraceStart, TraceEnd, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(TraceRadius),
            QueryParams
        );
    }
    else
    {
        World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
    }

#if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
    if (CVarDebugInteractionSource.GetValueOnGameThread() > 0)
    {
        // We offset the debug line by a small offset, since we are tracing from the eye position and would
        // therefore not see the debug line very well.
        FVector EyeLocation;
        FRotator EyeRotation;
        PC->GetPlayerViewPoint(EyeLocation, EyeRotation);
        const FVector DebugLineOffset = EyeRotation.RotateVector(FVector::RightVector) * 5.f;
        DrawDebugLine(World, TraceStart + DebugLineOffset, TraceEnd, FColor::Green, false, -1, 0, 0.5);
    }
#endif

    UInteractionTargetComponent* NewTarget = nullptr;
    if (HitResult.GetActor())
    {
        NewTarget = HitResult.GetActor()->FindComponentByClass<UInteractionTargetComponent>();
    }

    if (NewTarget != CurrentTarget)
    {
        if (CurrentTarget)
        {
            CurrentTarget->OnEndFocus();
            OnInteractionTargetLost.Broadcast();
        }

        CurrentTarget = NewTarget;

        if (CurrentTarget)
        {
            CurrentTarget->OnBeginFocus(this);
            OnInteractionTargetFound.Broadcast(CurrentTarget);
        }
    }
}

TPair<FVector, FVector> UInteractionSourceComponent::ComputeInteractionTraceEndpoints(const APlayerController& PC) const
{
    // Get viewport size
    int32 ViewportSizeX, ViewportSizeY;
    PC.GetViewportSize(ViewportSizeX, ViewportSizeY);

    // Get screen center
    // ReSharper disable CppTooWideScopeInitStatement
    FVector WorldLocation, WorldDirection;
    // ReSharper restore CppTooWideScopeInitStatement

    // Convert screen center to world position and direction
    if (PC.DeprojectScreenPositionToWorld(
            ViewportSizeX * AimReticleXPositionMultiplier, ViewportSizeY * AimReticleYPositionMultiplier, WorldLocation,
            WorldDirection
        ))
    {
        FVector TraceStart = WorldLocation;
        FVector TraceEnd   = TraceStart + (WorldDirection * InteractionDistance);
        return {TraceStart, TraceEnd};
    }

    GEngine->AddOnScreenDebugMessage(
        123, 5.0f, FColor::Red,
        "UInteractionSourceComponent::ComputeInteractionTraceEndpoints could not deproject screen position. "
        "Using fallback."
    );
    // Fallback to your original method if deprojection fails
    FVector EyeLocation;
    FRotator EyeRotation;
    PC.GetPlayerViewPoint(EyeLocation, EyeRotation);
    FVector TraceStart = EyeLocation;
    FVector TraceEnd   = TraceStart + (EyeRotation.Vector() * InteractionDistance);
    return {TraceStart, TraceEnd};
}

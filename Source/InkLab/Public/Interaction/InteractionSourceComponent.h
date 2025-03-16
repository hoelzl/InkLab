// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionSourceComponent.generated.h"

class UInteractionTargetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionTriggered, UInteractionTargetComponent*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionTargetFound, UInteractionTargetComponent*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionTargetLost);

UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class INKLAB_API UInteractionSourceComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    explicit UInteractionSourceComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void
    TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void TriggerInteraction();

    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnInteractionTriggered OnInteractionTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnInteractionTargetFound OnInteractionTargetFound;

    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnInteractionTargetLost OnInteractionTargetLost;

protected:
    virtual void BeginPlay() override;

    /** The distance we trace for interactions.
     * Note that this includes the length of the camera boom for third person games. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    float InteractionDistance = 800.0f;

    /** The size of the sphere we use to trace for interactions. If 0, we use a line trace. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    float TraceRadius = 20.0f;

    UPROPERTY()
    TObjectPtr<UInteractionTargetComponent> CurrentTarget = nullptr;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void PerformInteractionTrace();

    TPair<FVector, FVector>
    ComputeInteractionTraceEndpoints(const APlayerController& PC) const;
};

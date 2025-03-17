// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionTargetComponent.generated.h"

class UInteractionSourceComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, UInteractionSourceComponent*, Source);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginFocus, UInteractionSourceComponent*, Source);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndFocus);

UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class INKLAB_API UInteractionTargetComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    explicit UInteractionTargetComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual void OnInteract(UInteractionSourceComponent* Source);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual void OnBeginFocus(UInteractionSourceComponent* Source);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual void OnEndFocus();

    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnInteract OnInteractDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnBeginFocus OnBeginFocusDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnEndFocus OnEndFocusDelegate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText ObjectDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    FText ActionDescription;

protected:
    virtual void BeginPlay() override;
};

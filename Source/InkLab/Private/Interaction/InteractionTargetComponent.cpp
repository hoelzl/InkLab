// Copyright Dr. Matthias Hölzl

// InteractionTargetComponent.cpp

#include "Interaction/InteractionTargetComponent.h"

UInteractionTargetComponent::UInteractionTargetComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    InteractionPrompt                 = NSLOCTEXT("Interaction", "DefaultPrompt", "Interact");
}

void UInteractionTargetComponent::BeginPlay() { Super::BeginPlay(); }

void UInteractionTargetComponent::OnInteract(UInteractionSourceComponent* Source)
{
    OnInteractDelegate.Broadcast(Source);
}

void UInteractionTargetComponent::OnBeginFocus(UInteractionSourceComponent* Source)
{
    OnBeginFocusDelegate.Broadcast(Source);
}

void UInteractionTargetComponent::OnEndFocus() { OnEndFocusDelegate.Broadcast(); }

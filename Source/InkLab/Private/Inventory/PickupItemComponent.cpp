// Copyright Dr. Matthias Hölzl

#include "Inventory/PickupItemComponent.h"

#include "Interaction/InteractionSourceComponent.h"
#include "Inventory/InventoryComponent.h"

UPickupItemComponent::UPickupItemComponent(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
    InteractionPrompt = NSLOCTEXT("Interaction", "PickupPrompt", "Pick Up");
}

void UPickupItemComponent::BeginPlay()
{
    Super::BeginPlay();

    if (ItemToPickup)
    {
        InteractionPrompt =
            FText::Format(NSLOCTEXT("Interaction", "PickupPromptWithName", "Pick Up {0}"), ItemToPickup->Name);
    }
}

void UPickupItemComponent::OnInteract(UInteractionSourceComponent* Source)
{
    Super::OnInteract(Source);

    if (!ItemToPickup || ItemCount <= 0)
    {
        return;
    }

    const AActor* SourceActor = Source->GetOwner();
    if (!SourceActor)
    {
        return;
    }

    if (UInventoryComponent* InventoryComponent = SourceActor->FindComponentByClass<UInventoryComponent>())
    {
        if (InventoryComponent->AddItem(ItemToPickup, ItemCount))
        {
            // Item successfully added to inventory, destroy the pickup
            GetOwner()->Destroy();
        }
    }
}

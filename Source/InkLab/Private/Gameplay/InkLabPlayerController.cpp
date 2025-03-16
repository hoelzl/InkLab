// Copyright Dr. Matthias Hölzl


#include "Gameplay/InkLabPlayerController.h"

#include "Characters/InkLabCharacter.h"
#include "Interaction/InteractionSourceComponent.h"
#include "Inventory/InventoryComponent.h"
#include "UI/HUDControllerComponent.h"
AInkLabPlayerController::AInkLabPlayerController(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
    HUDController = CreateDefaultSubobject<UHUDControllerComponent>("HUDController");
}

UHUDControllerComponent* AInkLabPlayerController::GetHUDController() const { return HUDController; }

void AInkLabPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (!HUDController)
    {
        return;
    }

    if (const AInkLabCharacter* InkLabCharacter = GetPawn<AInkLabCharacter>())
    {
        if (UInteractionSourceComponent* InteractionSource =
                InkLabCharacter->FindComponentByClass<UInteractionSourceComponent>())
        {
            HUDController->SetupInteractionListeners(InteractionSource);
        }

        if (UInventoryComponent* InventoryComponent = InkLabCharacter->FindComponentByClass<UInventoryComponent>())
        {
            HUDController->SetupInventoryListeners(InventoryComponent);
        }
    }
}

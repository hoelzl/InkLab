// Copyright Dr. Matthias Hölzl


#include "Gameplay/InkLabPlayerController.h"

#include "Characters/InkLabCharacter.h"
#include "Interaction/InteractionSourceComponent.h"
#include "Inventory/InventoryComponent.h"
#include "UI/InkLabHUD.h"

void AInkLabPlayerController::BeginPlay()
{
    Super::BeginPlay();

    AInkLabHUD* InkLabHUD                   = Cast<AInkLabHUD>(GetHUD());
    const AInkLabCharacter* InkLabCharacter = GetPawn<AInkLabCharacter>();

    if (!ensure(InkLabHUD) || !ensure(InkLabCharacter))
    {
        return;
    }

    if (UInteractionSourceComponent* InteractionSource =
            InkLabCharacter->FindComponentByClass<UInteractionSourceComponent>();
        ensure(InteractionSource))
    {
        InkLabHUD->SetupInteractionListeners(InteractionSource);
    }

    if (UInventoryComponent* InventoryComponent = InkLabCharacter->FindComponentByClass<UInventoryComponent>();
        ensure(InventoryComponent))
    {
        InkLabHUD->SetupInventoryListeners(InventoryComponent);
    }
}

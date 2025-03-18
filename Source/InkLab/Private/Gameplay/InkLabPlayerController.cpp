// Copyright Dr. Matthias Hölzl


#include "Gameplay/InkLabPlayerController.h"

#include "Characters/InkLabCharacter.h"
#include "Dialogue/DialogueComponent.h"
#include "Interaction/InteractionSourceComponent.h"
#include "Inventory/InventoryComponent.h"
#include "UI/InkLabHUD.h"

AInkLabPlayerController::AInkLabPlayerController(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
    DialogueComponent = CreateDefaultSubobject<UDialogueComponent>("DialogueComponent");
}

void AInkLabPlayerController::BeginPlay()
{
    Super::BeginPlay();

    AInkLabHUD* InkLabHUD                   = GetHUD<AInkLabHUD>();
    const AInkLabCharacter* InkLabCharacter = GetPawn<AInkLabCharacter>();

    if (!ensure(InkLabHUD) || !ensure(InkLabCharacter))
    {
        return;
    }

    InkLabHUD->CreateHUD();

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

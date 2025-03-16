// Copyright Dr. Matthias Hölzl

// MerchantComponent.cpp
#include "Inventory/MerchantComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UMerchantComponent::UMerchantComponent()
{
    InteractionPrompt = NSLOCTEXT("Interaction", "MerchantPrompt", "Trade");

    // Create an inventory component for the merchant
    MerchantInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("MerchantInventory"));
}

void UMerchantComponent::BeginPlay()
{
    Super::BeginPlay();

    // Set up the merchant inventory
    if (!MerchantInventory)
    {
        MerchantInventory = NewObject<UInventoryComponent>(GetOwner());
        MerchantInventory->RegisterComponent();
    }
}

void UMerchantComponent::OnInteract(UInteractionSourceComponent* Source)
{
    Super::OnInteract(Source);

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC || !MerchantUIClass)
    {
        return;
    }

    // Create the merchant UI
    if (!CurrentMerchantUI)
    {
        CurrentMerchantUI = CreateWidget<UUserWidget>(PC, MerchantUIClass);
    }

    if (CurrentMerchantUI && !CurrentMerchantUI->IsInViewport())
    {
        CurrentMerchantUI->AddToViewport();
    }
}

bool UMerchantComponent::BuyItemFromPlayer(UInventoryComponent* PlayerInventory, int32 PlayerSlotIndex, int32 Count)
{
    if (!PlayerInventory || !MerchantInventory || PlayerSlotIndex < 0
        || PlayerSlotIndex >= PlayerInventory->Slots.Num())
    {
        return false;
    }

    FInventorySlot PlayerSlot;
    if (!PlayerInventory->GetSlot(PlayerSlotIndex, PlayerSlot) || !PlayerSlot.Item || PlayerSlot.Count < Count)
    {
        return false;
    }

    UInventoryItemBase* ItemToBuy = PlayerSlot.Item;
    int32 BuyPrice                = GetBuyPriceForItem(ItemToBuy, Count);

    // TODO: Add currency system to pay the player
    // For now, just transfer items

    if (MerchantInventory->CanAddItem(ItemToBuy, Count))
    {
        if (PlayerInventory->RemoveItem(ItemToBuy, Count))
        {
            MerchantInventory->AddItem(ItemToBuy, Count);
            return true;
        }
    }

    return false;
}

bool UMerchantComponent::SellItemToPlayer(UInventoryComponent* PlayerInventory, int32 MerchantSlotIndex, int32 Count)
{
    if (!PlayerInventory || !MerchantInventory || MerchantSlotIndex < 0
        || MerchantSlotIndex >= MerchantInventory->Slots.Num())
    {
        return false;
    }

    FInventorySlot MerchantSlot;
    if (!MerchantInventory->GetSlot(MerchantSlotIndex, MerchantSlot) || !MerchantSlot.Item
        || MerchantSlot.Count < Count)
    {
        return false;
    }

    UInventoryItemBase* ItemToSell = MerchantSlot.Item;
    int32 SellPrice                = GetSellPriceForItem(ItemToSell, Count);

    // TODO: Check if player has enough currency
    // For now, just transfer items

    if (PlayerInventory->CanAddItem(ItemToSell, Count))
    {
        if (MerchantInventory->RemoveItem(ItemToSell, Count))
        {
            PlayerInventory->AddItem(ItemToSell, Count);
            return true;
        }
    }

    return false;
}

int32 UMerchantComponent::GetBuyPriceForItem(UInventoryItemBase* Item, int32 Count) const
{
    if (!Item)
    {
        return 0;
    }

    return FMath::FloorToInt(Item->Value * BuyValueMultiplier * Count);
}

int32 UMerchantComponent::GetSellPriceForItem(UInventoryItemBase* Item, int32 Count)
{
    if (!Item)
    {
        return 0;
    }

    return FMath::CeilToInt(Item->Value * SellValueMultiplier * Count);
}

// Copyright Dr. Matthias Hölzl

// InventorySlotWidgetBase.cpp

#include "UI/InventorySlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Inventory/InventorySlotData.h"

UInventorySlotWidget::UInventorySlotWidget(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
    ConstructorHelpers::FObjectFinder<UTexture2D> DefaultItemTexture{
        TEXT("/Game/UI/Images/DefaultInventoryItem.DefaultInventoryItem")
    };
    if (DefaultItemTexture.Succeeded())
    {
        ItemIcon = DefaultItemTexture.Object;
    }
}
void UInventorySlotWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();
    UpdateButtonStyle();
}

void UInventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnSlotButtonClicked);
        UpdateButtonStyle();
    }
}

void UInventorySlotWidget::UseItem()
{
    if (SlotData && SlotData->InventoryComponent)
    {
        SlotData->InventoryComponent->UseItem(SlotData->SlotIndex);
    }
}

void UInventorySlotWidget::OnSlotButtonClicked() { UseItem(); }

bool UInventorySlotWidget::StartDrag()
{
    // Can only drag if the slot has an item
    return SlotData && SlotData->Item && SlotData->Count > 0;
}

bool UInventorySlotWidget::ReceiveDrop(UInventorySlotWidget* DraggedSlot)
{
    if (!DraggedSlot || !IsValidSlotData(DraggedSlot->SlotData) || !IsValidSlotData(SlotData))
    {
        return false;
    }

    UInventoryComponent* TargetInventoryComponent  = SlotData->InventoryComponent;
    const int32 TargetSlotIndex                    = SlotData->SlotIndex;
    UInventoryComponent* DraggedInventoryComponent = DraggedSlot->SlotData->InventoryComponent;
    const int32 DraggedSlotIndex                   = DraggedSlot->SlotData->SlotIndex;
    const int32 DraggedSlotCount                   = DraggedSlot->SlotData->Count;

    // If we're in the same inventory, simply swap slots
    if (TargetInventoryComponent == DraggedInventoryComponent)
    {
        return TargetInventoryComponent->SwapSlots(DraggedSlotIndex, TargetSlotIndex);
    }
    else
    {
        return DraggedInventoryComponent->TransferItem(TargetInventoryComponent, DraggedSlotIndex, DraggedSlotCount);
    }
}

void UInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
    if (UInventorySlotData* NewSlotData = Cast<UInventorySlotData>(ListItemObject))
    {
        SlotData = NewSlotData;

        if (NewSlotData->Item && NewSlotData->Item->Icon)
        {
            ItemIcon = NewSlotData->Item->Icon;
            UpdateButtonStyle();
        }

        if (ensure(ItemCount))
        {
            if (SlotData->Item && SlotData->Count > 1)
            {
                ItemCount->SetText(FText::AsNumber(SlotData->Count));
                ItemCount->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                ItemCount->SetVisibility(ESlateVisibility::Hidden);
            }
        }

        OnSlotUpdated();
    }
}

void UInventorySlotWidget::UpdateButtonStyle() const
{
    if (SlotButton && ItemIcon)
    {
        FButtonStyle Style = SlotButton->GetStyle();
        UpdateButtonBrush(Style.Normal);
        UpdateButtonBrush(Style.Hovered);
        UpdateButtonBrush(Style.Pressed);
        UpdateButtonBrush(Style.Disabled);
        SlotButton->SetStyle(Style);
    }
}

void UInventorySlotWidget::UpdateButtonBrush(FSlateBrush& Brush) const
{
    Brush.SetResourceObject(ItemIcon);
    Brush.ImageSize = FVector2D::ZeroVector;
    Brush.DrawAs    = ESlateBrushDrawType::Image;
}

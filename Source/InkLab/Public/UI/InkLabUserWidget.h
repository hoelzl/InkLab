// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InkLabUserWidget.generated.h"

/**
 * User widget that provides some additional useful methods
 */
UCLASS()
class INKLAB_API UInkLabUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void Show();
    virtual void Hide();
    virtual bool IsVisible() const;
};

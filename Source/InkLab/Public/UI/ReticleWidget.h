// Copyright Dr. Matthias Hölzl
// ReticleWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReticleWidget.generated.h"

class UImage;

/**
 * Widget for displaying an aiming reticle
 */
UCLASS()
class INKLAB_API UReticleWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    explicit UReticleWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;
    virtual void NativePreConstruct() override;

    // Change reticle color
    UFUNCTION(BlueprintCallable, Category = "Reticle")
    void SetReticleColor(const FLinearColor& NewColor) const;

    // Change reticle size
    UFUNCTION(BlueprintCallable, Category = "Reticle")
    void SetReticleSize(float NewSize);

protected:
    // The actual reticle image
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UImage> ReticleImage;

    // Default reticle color
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reticle")
    FLinearColor DefaultColor = FLinearColor::White;

    // Reticle size multiplier
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reticle")
    float Size = 1.0f;

    // Position the reticle at the correct screen location
    void PositionReticle();
};

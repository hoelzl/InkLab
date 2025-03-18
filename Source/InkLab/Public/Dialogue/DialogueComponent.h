// Copyright Dr. Matthias Hölzl

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoryCreated, UInkpotStory*, InkpotStory);

class UInkpotStoryAsset;
class UInkpotStory;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INKLAB_API UDialogueComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    explicit UDialogueComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    UInkpotStoryAsset* GetStoryAsset() { return StoryAsset; };

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    UInkpotStory* GetStory() { return Story; };

    UPROPERTY(BlueprintAssignable, Category = "Dialogue")
    FOnStoryCreated OnStoryCreated;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInkpotStoryAsset> StoryAsset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInkpotStory> Story;
};

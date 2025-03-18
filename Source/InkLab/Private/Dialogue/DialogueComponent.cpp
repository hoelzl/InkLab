// Copyright Dr. Matthias Hölzl

#include "Dialogue/DialogueComponent.h"

UDialogueComponent::UDialogueComponent(const FObjectInitializer& ObjectInitializer)
{
    StoryAsset = CreateDefaultSubobject<UInkpotStoryAsset>("InkpotStoryAsset");
    Story      = CreateDefaultSubobject<UInkpotStory>("InkpotStory");
}

void UDialogueComponent::BeginPlay()
{
    Super::BeginPlay();

    if (StoryAsset)
    {
        if (UInkpot* InkpotSubsystem = GEngine->GetEngineSubsystem<UInkpot>(); ensure(InkpotSubsystem))
        {
            // Not sure whether we need this (or should even call it).
            InkpotSubsystem->InitializeStoryFactory();
            Story = InkpotSubsystem->BeginStory(StoryAsset);

            if (Story)
            {
                OnStoryCreated.Broadcast(Story);
            }
        }
    }
}

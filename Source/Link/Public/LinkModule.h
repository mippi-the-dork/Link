// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "Modules/ModuleManager.h"

class FLevelEditorViewportClient;
class SLevelViewport;
class SWidget;
struct FToolMenuContext;

class FLinkModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void RegisterMenus();
    TSharedRef<SWidget> MakeToolbarWidget(const FToolMenuContext& Context);
    TSharedRef<SWidget> BuildLinkMenu(TWeakPtr<SLevelViewport> WeakViewport);

    bool TickLink(float DeltaTime);
    void RegisterViewport(const TSharedPtr<SLevelViewport>& Viewport);
    void PruneInvalidViewports();
    FLevelEditorViewportClient* GetViewportClient(FName ViewportKey) const;
    FName GetViewportKey(const TSharedPtr<SLevelViewport>& Viewport) const;

    void ToggleEnabled();
    void ToggleSource(FName ViewportKey);
    void ToggleTarget(FName ViewportKey);
    void ClearLinkSetup();
    void ResetSourceSample();

    bool CanUseAsTarget(FName ViewportKey) const;
    bool IsSource(FName ViewportKey) const;
    bool IsTarget(FName ViewportKey) const;
    FText GetToolbarLabel(FName ViewportKey) const;
    FText GetToolbarTooltip(FName ViewportKey) const;

private:
    TMap<FName, TWeakPtr<SLevelViewport>> RegisteredViewports;
    FName SourceViewportKey = NAME_None;
    TSet<FName> TargetViewportKeys;

    bool bLinkEnabled = false;
    bool bLinkPositionX = true;
    bool bLinkPositionY = true;
    bool bLinkPositionZ = true;
    bool bLinkPitch = false;
    bool bLinkYaw = false;
    bool bLinkRoll = false;

    bool bHasPreviousSourceSample = false;
    bool bApplyingLinkedUpdate = false;
    FVector PreviousSourceLocation = FVector::ZeroVector;
    FRotator PreviousSourceRotation = FRotator::ZeroRotator;

    FTSTicker::FDelegateHandle TickerHandle;
};

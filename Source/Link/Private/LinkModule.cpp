// Copyright Epic Games, Inc. All Rights Reserved.

#include "LinkModule.h"

#include "Editor.h"
#include "Editor/UnrealEdTypes.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "LevelEditorMenuContext.h"
#include "LevelEditorViewport.h"
#include "SLevelViewport.h"
#include "ToolMenus.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "Link"

void FLinkModule::StartupModule()
{
    TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
        FTickerDelegate::CreateRaw(this, &FLinkModule::TickLink));

    UToolMenus::RegisterStartupCallback(
        FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FLinkModule::RegisterMenus));
}

void FLinkModule::ShutdownModule()
{
    if (TickerHandle.IsValid())
    {
        FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
        TickerHandle.Reset();
    }

    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);

    RegisteredViewports.Reset();
    TargetViewportKeys.Reset();
    SourceViewportKey = NAME_None;
}

void FLinkModule::RegisterMenus()
{
    FToolMenuOwnerScoped OwnerScoped(this);

    UToolMenu* ViewportToolbar = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.ViewportToolbar"));
    if (!ViewportToolbar)
    {
        return;
    }

    FToolMenuSection& RightSection = ViewportToolbar->FindOrAddSection(TEXT("Right"));
    FToolMenuEntry Entry = FToolMenuEntry::InitWidget(
        TEXT("LinkViewportControl"),
        SNullWidget::NullWidget,
        LOCTEXT("LinkToolbarLabel", "Link"),
        true,
        false);

    Entry.MakeCustomWidget = FNewToolMenuCustomWidget::CreateLambda(
        [this](const FToolMenuContext& Context, const FToolMenuCustomWidgetContext&) -> TSharedRef<SWidget>
        {
            return MakeToolbarWidget(Context);
        });

    // Match Lux's camera-area placement without depending on Lux itself.
    Entry.InsertPosition = FToolMenuInsert(TEXT("Camera"), EToolMenuInsertType::Before);
    RightSection.AddEntry(Entry);
}

TSharedRef<SWidget> FLinkModule::MakeToolbarWidget(const FToolMenuContext& Context)
{
    TSharedPtr<SLevelViewport> Viewport = ULevelViewportContext::GetLevelViewport(Context);
    if (!Viewport.IsValid())
    {
        return SNullWidget::NullWidget;
    }

    RegisterViewport(Viewport);
    const FName ViewportKey = GetViewportKey(Viewport);
    const TWeakPtr<SLevelViewport> WeakViewport = Viewport;

    return SNew(SComboButton)
        .ContentPadding(FMargin(5.0f, 1.0f))
        .ToolTipText_Lambda([this, ViewportKey]()
        {
            return GetToolbarTooltip(ViewportKey);
        })
        .OnGetMenuContent_Lambda([this, WeakViewport]()
        {
            return BuildLinkMenu(WeakViewport);
        })
        .ButtonContent()
        [
            SNew(STextBlock)
            .Text_Lambda([this, ViewportKey]()
            {
                return GetToolbarLabel(ViewportKey);
            })
        ];
}

TSharedRef<SWidget> FLinkModule::BuildLinkMenu(TWeakPtr<SLevelViewport> WeakViewport)
{
    const TSharedPtr<SLevelViewport> Viewport = WeakViewport.Pin();
    if (!Viewport.IsValid())
    {
        return SNullWidget::NullWidget;
    }

    RegisterViewport(Viewport);
    const FName ViewportKey = GetViewportKey(Viewport);

    FMenuBuilder MenuBuilder(true, nullptr);

    MenuBuilder.BeginSection(TEXT("LinkState"), LOCTEXT("LinkStateSection", "Link"));
    MenuBuilder.AddMenuEntry(
        LOCTEXT("EnableLink", "Link Enabled"),
        LOCTEXT("EnableLinkTooltip", "Enable or suspend viewport linking. Disabling Link preserves the current source, targets, and axis selections."),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateRaw(this, &FLinkModule::ToggleEnabled),
            FCanExecuteAction(),
            FIsActionChecked::CreateLambda([this]() { return bLinkEnabled; })),
        NAME_None,
        EUserInterfaceActionType::ToggleButton);

    MenuBuilder.AddMenuEntry(
        LOCTEXT("SetSource", "This Viewport Is Source"),
        LOCTEXT("SetSourceTooltip", "Use this viewport as Link's one-way source. Camera movement from the source is applied as relative deltas to all linked targets."),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateLambda([this, ViewportKey]() { ToggleSource(ViewportKey); }),
            FCanExecuteAction(),
            FIsActionChecked::CreateLambda([this, ViewportKey]() { return IsSource(ViewportKey); })),
        NAME_None,
        EUserInterfaceActionType::ToggleButton);

    MenuBuilder.AddMenuEntry(
        LOCTEXT("SetTarget", "This Viewport Is Target"),
        LOCTEXT("SetTargetTooltip", "Add or remove this viewport as a Link target. Targets preserve their existing offset and receive only the enabled source movement and rotation deltas."),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateLambda([this, ViewportKey]() { ToggleTarget(ViewportKey); }),
            FCanExecuteAction::CreateLambda([this, ViewportKey]() { return CanUseAsTarget(ViewportKey); }),
            FIsActionChecked::CreateLambda([this, ViewportKey]() { return IsTarget(ViewportKey); })),
        NAME_None,
        EUserInterfaceActionType::ToggleButton);
    MenuBuilder.EndSection();

    MenuBuilder.BeginSection(TEXT("LinkPosition"), LOCTEXT("LinkPositionSection", "Position"));
    auto AddAxisToggle = [&MenuBuilder](const FText& Label, const FText& Tooltip, bool& Value)
    {
        bool* ValuePtr = &Value;
        MenuBuilder.AddMenuEntry(
            Label,
            Tooltip,
            FSlateIcon(),
            FUIAction(
                FExecuteAction::CreateLambda([ValuePtr]() { *ValuePtr = !*ValuePtr; }),
                FCanExecuteAction(),
                FIsActionChecked::CreateLambda([ValuePtr]() { return *ValuePtr; })),
            NAME_None,
            EUserInterfaceActionType::ToggleButton);
    };

    AddAxisToggle(
        LOCTEXT("PositionX", "X"),
        LOCTEXT("PositionXTooltip", "Apply the source viewport's X-axis movement delta to linked targets."),
        bLinkPositionX);
    AddAxisToggle(
        LOCTEXT("PositionY", "Y"),
        LOCTEXT("PositionYTooltip", "Apply the source viewport's Y-axis movement delta to linked targets."),
        bLinkPositionY);
    AddAxisToggle(
        LOCTEXT("PositionZ", "Z"),
        LOCTEXT("PositionZTooltip", "Apply the source viewport's Z-axis movement delta to linked targets."),
        bLinkPositionZ);
    MenuBuilder.EndSection();

    MenuBuilder.BeginSection(TEXT("LinkRotation"), LOCTEXT("LinkRotationSection", "Rotation"));
    AddAxisToggle(
        LOCTEXT("Pitch", "Pitch"),
        LOCTEXT("PitchTooltip", "Apply the source viewport's Pitch rotation delta to linked perspective targets."),
        bLinkPitch);
    AddAxisToggle(
        LOCTEXT("Yaw", "Yaw"),
        LOCTEXT("YawTooltip", "Apply the source viewport's Yaw rotation delta to linked perspective targets."),
        bLinkYaw);
    AddAxisToggle(
        LOCTEXT("Roll", "Roll"),
        LOCTEXT("RollTooltip", "Apply the source viewport's Roll rotation delta to linked perspective targets."),
        bLinkRoll);
    MenuBuilder.EndSection();

    MenuBuilder.BeginSection(TEXT("LinkActions"), LOCTEXT("LinkActionsSection", "Actions"));
    MenuBuilder.AddMenuEntry(
        LOCTEXT("ClearLinkSetup", "Clear Link Setup"),
        LOCTEXT("ClearLinkSetupTooltip", "Disable Link and clear the current source and all targets. Axis selections return to their defaults."),
        FSlateIcon(),
        FUIAction(FExecuteAction::CreateRaw(this, &FLinkModule::ClearLinkSetup)));
    MenuBuilder.EndSection();

    return MenuBuilder.MakeWidget();
}

void FLinkModule::RegisterViewport(const TSharedPtr<SLevelViewport>& Viewport)
{
    if (!Viewport.IsValid())
    {
        return;
    }

    const FName Key = GetViewportKey(Viewport);
    if (!Key.IsNone())
    {
        RegisteredViewports.Add(Key, Viewport);
    }
}

FName FLinkModule::GetViewportKey(const TSharedPtr<SLevelViewport>& Viewport) const
{
    if (!Viewport.IsValid())
    {
        return NAME_None;
    }

    // GetConfigKey is the public Level Editor viewport identity in UE 5.8.
    // Do not fall back to GetViewportTypeWithinLayout(), which is deprecated
    // and has moved to internal FViewportTabContent handling.
    return Viewport->GetConfigKey();
}

void FLinkModule::PruneInvalidViewports()
{
    for (auto It = RegisteredViewports.CreateIterator(); It; ++It)
    {
        if (!It.Value().IsValid())
        {
            It.RemoveCurrent();
        }
    }

    if (!SourceViewportKey.IsNone() && !RegisteredViewports.Contains(SourceViewportKey))
    {
        // Keep the key so a rebuilt viewport with the same config identity can reconnect.
        ResetSourceSample();
    }

    for (auto It = TargetViewportKeys.CreateIterator(); It; ++It)
    {
        if (It->IsNone())
        {
            It.RemoveCurrent();
        }
    }
}

FLevelEditorViewportClient* FLinkModule::GetViewportClient(FName ViewportKey) const
{
    const TWeakPtr<SLevelViewport>* WeakViewport = RegisteredViewports.Find(ViewportKey);
    if (!WeakViewport)
    {
        return nullptr;
    }

    const TSharedPtr<SLevelViewport> Viewport = WeakViewport->Pin();
    if (!Viewport.IsValid())
    {
        return nullptr;
    }

    return &Viewport->GetLevelViewportClient();
}

bool FLinkModule::TickLink(float DeltaTime)
{
    if (bApplyingLinkedUpdate)
    {
        return true;
    }

    PruneInvalidViewports();

    if (!bLinkEnabled || !GEditor || SourceViewportKey.IsNone() || TargetViewportKeys.IsEmpty())
    {
        ResetSourceSample();
        return true;
    }

    // Keep editor camera linking out of PIE/SIE. Link is an editing aid, not runtime behavior.
    if (GEditor->PlayWorld != nullptr)
    {
        ResetSourceSample();
        return true;
    }

    FLevelEditorViewportClient* SourceClient = GetViewportClient(SourceViewportKey);
    if (!SourceClient || SourceClient->GetViewportType() != LVT_Perspective || SourceClient->IsAnyActorLocked() || SourceClient->IsLockedToCinematic())
    {
        ResetSourceSample();
        return true;
    }

    const FVector CurrentSourceLocation = SourceClient->GetViewLocation();
    const FRotator CurrentSourceRotation = SourceClient->GetViewRotation();

    if (!bHasPreviousSourceSample)
    {
        PreviousSourceLocation = CurrentSourceLocation;
        PreviousSourceRotation = CurrentSourceRotation;
        bHasPreviousSourceSample = true;
        return true;
    }

    const FVector LocationDelta = CurrentSourceLocation - PreviousSourceLocation;
    const FRotator RotationDelta = (CurrentSourceRotation - PreviousSourceRotation).GetNormalized();

    PreviousSourceLocation = CurrentSourceLocation;
    PreviousSourceRotation = CurrentSourceRotation;

    const bool bHasPositionDelta = !LocationDelta.IsNearlyZero();
    const bool bHasRotationDelta = !RotationDelta.IsNearlyZero(KINDA_SMALL_NUMBER);
    if (!bHasPositionDelta && !bHasRotationDelta)
    {
        return true;
    }

    TGuardValue<bool> ApplyingGuard(bApplyingLinkedUpdate, true);

    for (const FName TargetKey : TargetViewportKeys)
    {
        if (TargetKey == SourceViewportKey)
        {
            continue;
        }

        FLevelEditorViewportClient* TargetClient = GetViewportClient(TargetKey);
        if (!TargetClient || TargetClient->IsAnyActorLocked() || TargetClient->IsLockedToCinematic())
        {
            continue;
        }

        bool bChangedTarget = false;

        if (bHasPositionDelta && (bLinkPositionX || bLinkPositionY || bLinkPositionZ))
        {
            FVector TargetLocation = TargetClient->GetViewLocation();
            if (bLinkPositionX) TargetLocation.X += LocationDelta.X;
            if (bLinkPositionY) TargetLocation.Y += LocationDelta.Y;
            if (bLinkPositionZ) TargetLocation.Z += LocationDelta.Z;
            TargetClient->SetViewLocation(TargetLocation);
            bChangedTarget = true;
        }

        // Orthographic views have no meaningful free camera rotation. Preserve their projection orientation.
        if (TargetClient->GetViewportType() == LVT_Perspective && bHasRotationDelta && (bLinkPitch || bLinkYaw || bLinkRoll))
        {
            FRotator TargetRotation = TargetClient->GetViewRotation();
            if (bLinkPitch) TargetRotation.Pitch += RotationDelta.Pitch;
            if (bLinkYaw) TargetRotation.Yaw += RotationDelta.Yaw;
            if (bLinkRoll) TargetRotation.Roll += RotationDelta.Roll;
            TargetClient->SetViewRotation(TargetRotation.GetNormalized());
            bChangedTarget = true;
        }

        if (bChangedTarget)
        {
            TargetClient->Invalidate();
        }
    }

    return true;
}

void FLinkModule::ToggleEnabled()
{
    bLinkEnabled = !bLinkEnabled;
    ResetSourceSample();
}

void FLinkModule::ToggleSource(FName ViewportKey)
{
    if (ViewportKey.IsNone())
    {
        return;
    }

    if (SourceViewportKey == ViewportKey)
    {
        SourceViewportKey = NAME_None;
    }
    else
    {
        SourceViewportKey = ViewportKey;
        TargetViewportKeys.Remove(ViewportKey);
    }

    ResetSourceSample();
}

void FLinkModule::ToggleTarget(FName ViewportKey)
{
    if (!CanUseAsTarget(ViewportKey))
    {
        return;
    }

    if (TargetViewportKeys.Contains(ViewportKey))
    {
        TargetViewportKeys.Remove(ViewportKey);
    }
    else
    {
        TargetViewportKeys.Add(ViewportKey);
    }
}

void FLinkModule::ClearLinkSetup()
{
    bLinkEnabled = false;
    SourceViewportKey = NAME_None;
    TargetViewportKeys.Reset();

    bLinkPositionX = true;
    bLinkPositionY = true;
    bLinkPositionZ = true;
    bLinkPitch = false;
    bLinkYaw = false;
    bLinkRoll = false;

    ResetSourceSample();
}

void FLinkModule::ResetSourceSample()
{
    bHasPreviousSourceSample = false;
    PreviousSourceLocation = FVector::ZeroVector;
    PreviousSourceRotation = FRotator::ZeroRotator;
}

bool FLinkModule::CanUseAsTarget(FName ViewportKey) const
{
    return !ViewportKey.IsNone() && ViewportKey != SourceViewportKey;
}

bool FLinkModule::IsSource(FName ViewportKey) const
{
    return !ViewportKey.IsNone() && ViewportKey == SourceViewportKey;
}

bool FLinkModule::IsTarget(FName ViewportKey) const
{
    return !ViewportKey.IsNone() && TargetViewportKeys.Contains(ViewportKey);
}

FText FLinkModule::GetToolbarLabel(FName ViewportKey) const
{
    if (IsSource(ViewportKey))
    {
        return bLinkEnabled ? LOCTEXT("LinkSourceOn", "Link S") : LOCTEXT("LinkSourceOff", "Link S");
    }

    if (IsTarget(ViewportKey))
    {
        return bLinkEnabled ? LOCTEXT("LinkTargetOn", "Link T") : LOCTEXT("LinkTargetOff", "Link T");
    }

    return LOCTEXT("LinkDefault", "Link");
}

FText FLinkModule::GetToolbarTooltip(FName ViewportKey) const
{
    if (IsSource(ViewportKey))
    {
        return bLinkEnabled
            ? LOCTEXT("SourceEnabledTooltip", "Link is enabled. This viewport is the source. Its relative camera movement drives the selected target viewports.")
            : LOCTEXT("SourceDisabledTooltip", "This viewport is Link's source, but Link is currently suspended. Open the menu to enable linking or change axes.");
    }

    if (IsTarget(ViewportKey))
    {
        return bLinkEnabled
            ? LOCTEXT("TargetEnabledTooltip", "Link is enabled. This viewport is a target and follows the enabled movement axes from the source while preserving its offset.")
            : LOCTEXT("TargetDisabledTooltip", "This viewport is a Link target, but Link is currently suspended. Open the menu to enable linking or change axes.");
    }

    return LOCTEXT("DefaultTooltip", "Link Level Editor viewports using relative camera movement. Set one viewport as the source, one or more as targets, and choose which position and rotation axes follow.");
}

IMPLEMENT_MODULE(FLinkModule, Link)

#undef LOCTEXT_NAMESPACE

// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "LinkModule.h"

#include "Editor.h"
#include "Editor/UnrealEdTypes.h"
#include "LevelEditorViewport.h"
#include "Interfaces/IPluginManager.h"
#include "SLevelViewport.h"
#include "Styling/AppStyle.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SegmentedControlStyle.h"
#include "Styling/StyleDefaults.h"
#include "ToolMenus.h"
#include "ViewportToolbar/UnrealEdViewportToolbarContext.h"
#include "Brushes/SlateImageBrush.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SSegmentedControl.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "Link"

namespace LinkUI
{
    constexpr float RoleVisualSize = 9.0f;
    constexpr float RoleColumnWidth = 14.0f;
    constexpr float SettingsRowWidth = 244.0f;
    constexpr float SettingsContentHorizontalPadding = 8.0f;
    constexpr float RoleButtonGap = 2.0f;

    const FName StyleSetName(TEXT("LinkStyle"));

    enum class ELinkMode : uint8
    {
        Relative,
        Absolute
    };

    const FCheckBoxStyle* RadioStyle()
    {
        static const FCheckBoxStyle RoleRadioStyle = []()
        {
            FCheckBoxStyle Style = FAppStyle::Get().GetWidgetStyle<FCheckBoxStyle>(TEXT("RadioButton"));

            // Make an unselected Link role easier to see without introducing a custom graphic:
            // the old hover appearance becomes the normal appearance, and hover becomes brighter.
            Style.UncheckedImage = Style.UncheckedHoveredImage;
            Style.UncheckedHoveredImage.TintColor = FSlateColor(FLinearColor::White);

            return Style;
        }();

        return &RoleRadioStyle;
    }

    const FSegmentedControlStyle& SegmentedStyle()
    {
        return FAppStyle::Get().GetWidgetStyle<FSegmentedControlStyle>(TEXT("SegmentedControl"));
    }

    const FTextBlockStyle* SegmentTextStyle()
    {
        return &FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>(TEXT("SmallButtonText"));
    }

    FSlateIcon SyncIcon()
    {
        return FSlateIcon(StyleSetName, TEXT("Link.Sync"));
    }

    FSlateIcon LinkOnIcon()
    {
        return FSlateIcon(StyleSetName, TEXT("Link.Enabled"));
    }

    FSlateIcon LinkOffIcon()
    {
        return FSlateIcon(StyleSetName, TEXT("Link.Disabled"));
    }

    FSlateIcon SettingsIcon()
    {
        return FSlateIcon(StyleSetName, TEXT("Link.Settings"));
    }
}

void FLinkModule::StartupModule()
{
    RegisterStyle();

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
    UnregisterStyle();
    UToolMenus::UnregisterOwner(this);

    RegisteredViewports.Reset();
    TargetViewportKeys.Reset();
    SourceViewportKey = NAME_None;
}

void FLinkModule::RegisterStyle()
{
    if (StyleSet.IsValid())
    {
        return;
    }

    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("Link"));
    if (!Plugin.IsValid())
    {
        return;
    }

    StyleSet = MakeShared<FSlateStyleSet>(LinkUI::StyleSetName);
    StyleSet->SetContentRoot(Plugin->GetBaseDir() / TEXT("Resources"));
    StyleSet->Set(TEXT("Link.Sync"), new FSlateVectorImageBrush(StyleSet->RootToContentDir(TEXT("Link_Sync"), TEXT(".svg")), FVector2D(18.0f, 18.0f)));
    StyleSet->Set(TEXT("Link.Enabled"), new FSlateVectorImageBrush(StyleSet->RootToContentDir(TEXT("Link_Off"), TEXT(".svg")), FVector2D(18.0f, 18.0f)));
    StyleSet->Set(TEXT("Link.Disabled"), new FSlateVectorImageBrush(StyleSet->RootToContentDir(TEXT("Link_On"), TEXT(".svg")), FVector2D(18.0f, 18.0f)));
    StyleSet->Set(TEXT("Link.Settings"), new FSlateVectorImageBrush(StyleSet->RootToContentDir(TEXT("Link_Settings"), TEXT(".svg")), FVector2D(18.0f, 18.0f)));

    FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
}

void FLinkModule::UnregisterStyle()
{
    if (!StyleSet.IsValid())
    {
        return;
    }

    FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
    ensure(StyleSet.IsUnique());
    StyleSet.Reset();
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

    FToolMenuEntry LinkControls = FToolMenuEntry::InitDynamicEntry(
        TEXT("LinkViewportControls"),
        FNewToolMenuSectionDelegate::CreateRaw(this, &FLinkModule::BuildToolbarEntries));

    // Keep Link beside the camera controls, but let the viewport toolbar itself own sizing and styling.
    LinkControls.InsertPosition = FToolMenuInsert(TEXT("Camera"), EToolMenuInsertType::Before);
    RightSection.AddEntry(LinkControls);
}

void FLinkModule::BuildToolbarEntries(FToolMenuSection& Section)
{
    UUnrealEdViewportToolbarContext* ToolbarContext = Section.FindContext<UUnrealEdViewportToolbarContext>();
    if (!ToolbarContext)
    {
        return;
    }

    TSharedPtr<SEditorViewport> EditorViewport = ToolbarContext->Viewport.Pin();
    if (!EditorViewport.IsValid())
    {
        return;
    }

    // This extension only runs inside LevelEditor.ViewportToolbar, whose viewport is an SLevelViewport.
    TSharedPtr<SLevelViewport> Viewport = StaticCastSharedPtr<SLevelViewport>(EditorViewport);
    if (!Viewport.IsValid())
    {
        return;
    }

    RegisterViewport(Viewport);
    const FName ViewportKey = GetViewportKey(Viewport);
    if (ViewportKey.IsNone())
    {
        return;
    }

    FToolMenuEntry& SyncEntry = Section.AddMenuEntry(
        TEXT("Link.Sync"),
        LOCTEXT("SyncButton", "Sync"),
        LOCTEXT(
            "SyncTooltip",
            "Sync\nImmediately align all Link targets to the source viewport on the enabled position and rotation axes. This is a one-shot absolute sync and works independently of Relative or Absolute mode."),
        LinkUI::SyncIcon(),
        FUIAction(
            FExecuteAction::CreateRaw(this, &FLinkModule::SyncTargets),
            FCanExecuteAction::CreateRaw(this, &FLinkModule::CanSyncTargets)),
        EUserInterfaceActionType::Button);
    SyncEntry.ToolBarData.LabelOverride = FText::GetEmpty();
    SyncEntry.ToolBarData.ResizeParams.ClippingPriority = 920;

    FToolMenuEntry& LinkEntry = Section.AddMenuEntry(
        TEXT("Link.Enabled"),
        LOCTEXT("LinkButton", "Link"),
        LOCTEXT(
            "LinkToggleTooltip",
            "Link\nEnable or suspend linked viewport camera movement. Disabling Link preserves the current source, targets, axis selections, and mode."),
        LinkUI::LinkOffIcon(),
        FUIAction(
            FExecuteAction::CreateRaw(this, &FLinkModule::ToggleEnabled),
            FCanExecuteAction(),
            FIsActionChecked::CreateLambda([this]() { return bLinkEnabled; })),
        EUserInterfaceActionType::ToggleButton);
    LinkEntry.Icon = TAttribute<FSlateIcon>::CreateLambda([this]()
    {
        return bLinkEnabled ? LinkUI::LinkOnIcon() : LinkUI::LinkOffIcon();
    });
    LinkEntry.ToolBarData.LabelOverride = FText::GetEmpty();
    LinkEntry.ToolBarData.ResizeParams.ClippingPriority = 930;

    FToolMenuEntry RoleEntry = FToolMenuEntry::InitWidget(
        TEXT("Link.Roles"),
        MakeRoleWidget(ViewportKey),
        FText::GetEmpty(),
        true,
        false);
    RoleEntry.ToolBarData.ResizeParams.ClippingPriority = 940;
    Section.AddEntry(RoleEntry);

    FToolMenuEntry& SettingsEntry = Section.AddSubMenu(
        TEXT("Link.Settings"),
        LOCTEXT("SettingsButton", "Settings"),
        LOCTEXT(
            "SettingsTooltip",
            "Settings\nConfigure linked position axes, rotation axes, synchronization mode, or clear the current Link setup."),
        FNewToolMenuDelegate::CreateRaw(this, &FLinkModule::PopulateSettingsMenu),
        false,
        LinkUI::SettingsIcon());
    SettingsEntry.ToolBarData.LabelOverride = FText::GetEmpty();
    SettingsEntry.ToolBarData.ResizeParams.ClippingPriority = 950;
    SettingsEntry.ToolBarData.PlacementOverride = MenuPlacement_BelowRightAnchor;
}

TSharedRef<SWidget> FLinkModule::MakeRoleWidget(FName ViewportKey)
{
    auto MakeRoleButton = [this, ViewportKey](bool bSource) -> TSharedRef<SWidget>
    {
        return SNew(SBox)
            .WidthOverride(LinkUI::RoleColumnWidth)
            .HeightOverride(LinkUI::RoleVisualSize)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            [
                SNew(SScaleBox)
                .Stretch(EStretch::ScaleToFit)
                .StretchDirection(EStretchDirection::DownOnly)
                [
                    SNew(SCheckBox)
                    .Style(LinkUI::RadioStyle())
                    .Padding(FMargin(0.0f))
                    .IsChecked_Lambda([this, ViewportKey, bSource]()
                    {
                        const bool bChecked = bSource ? IsSource(ViewportKey) : IsTarget(ViewportKey);
                        return bChecked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
                    })
                    .OnCheckStateChanged_Lambda([this, ViewportKey, bSource](ECheckBoxState)
                    {
                        if (bSource)
                        {
                            ToggleSource(ViewportKey);
                        }
                        else
                        {
                            ToggleTarget(ViewportKey);
                        }
                    })
                    .ToolTipText(bSource
                        ? LOCTEXT(
                            "SourceRoleTooltip",
                            "Make this viewport the Link source. Only one Level Editor viewport can be the source at a time. Choosing a new source automatically clears the previous source.")
                        : LOCTEXT(
                            "TargetRoleTooltip",
                            "Make this viewport a Link target. Any number of viewports can be targets, but the source viewport cannot also be a target."))
                ]
            ];
    };

    return SNew(SBox)
        .Padding(FMargin(2.0f, 0.0f))
        .VAlign(VAlign_Center)
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                MakeRoleButton(true)
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0.0f, LinkUI::RoleButtonGap, 0.0f, 0.0f)
            [
                MakeRoleButton(false)
            ]
        ];
}

void FLinkModule::PopulateSettingsMenu(UToolMenu* Menu)
{
    if (!Menu)
    {
        return;
    }

    auto WrapMenuContent = [](const TSharedRef<SWidget>& Content) -> TSharedRef<SWidget>
    {
        return SNew(SBorder)
            .BorderImage(FStyleDefaults::GetNoBrush())
            .Padding(FMargin(LinkUI::SettingsContentHorizontalPadding, 0.0f))
            [
                Content
            ];
    };

    auto MakeAxisSegments = [](bool* AValue, bool* BValue, bool* CValue,
                               const FText& ALabel, const FText& BLabel, const FText& CLabel,
                               const FText& ATooltip, const FText& BTooltip, const FText& CTooltip) -> TSharedRef<SWidget>
    {
        const FSegmentedControlStyle& Style = LinkUI::SegmentedStyle();
        FMargin SlotPadding = Style.UniformPadding;
        SlotPadding.Right = 0.0f;

        TSharedRef<SUniformGridPanel> Grid = SNew(SUniformGridPanel).SlotPadding(SlotPadding);

        auto AddSegment = [&Grid, &Style](int32 Column, bool* ValuePtr, const FText& Label, const FText& Tooltip)
        {
            const FCheckBoxStyle* CheckStyle = &Style.ControlStyle;
            if (Column == 0)
            {
                CheckStyle = &Style.FirstControlStyle;
            }
            else if (Column == 2)
            {
                CheckStyle = &Style.LastControlStyle;
            }

            Grid->AddSlot(Column, 0)
            [
                SNew(SCheckBox)
                .Clipping(EWidgetClipping::ClipToBounds)
                .HAlign(HAlign_Center)
                .Style(CheckStyle)
                .Padding(Style.UniformPadding)
                .IsChecked_Lambda([ValuePtr]()
                {
                    return *ValuePtr ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
                })
                .OnCheckStateChanged_Lambda([ValuePtr](ECheckBoxState NewState)
                {
                    *ValuePtr = NewState == ECheckBoxState::Checked;
                })
                .ToolTipText(Tooltip)
                [
                    SNew(STextBlock)
                    .TextStyle(LinkUI::SegmentTextStyle())
                    .Text(Label)
                ]
            ];
        };

        AddSegment(0, AValue, ALabel, ATooltip);
        AddSegment(1, BValue, BLabel, BTooltip);
        AddSegment(2, CValue, CLabel, CTooltip);

        return SNew(SBox)
            .WidthOverride(LinkUI::SettingsRowWidth)
            [
                SNew(SBorder)
                .BorderImage(&Style.BackgroundBrush)
                .Padding(FMargin(0.0f, 0.0f, Style.UniformPadding.Right, 0.0f))
                [
                    Grid
                ]
            ];
    };

    FToolMenuSection& LocationSection = Menu->AddSection(
        TEXT("Link.Location"),
        LOCTEXT("LocationSection", "Location"));
    LocationSection.AddEntry(FToolMenuEntry::InitWidget(
        TEXT("Link.LocationAxes"),
        WrapMenuContent(MakeAxisSegments(
            &bLinkPositionX,
            &bLinkPositionY,
            &bLinkPositionZ,
            LOCTEXT("PositionX", "X"),
            LOCTEXT("PositionY", "Y"),
            LOCTEXT("PositionZ", "Z"),
            LOCTEXT("PositionXTooltip", "Include the X position axis when synchronizing Link targets."),
            LOCTEXT("PositionYTooltip", "Include the Y position axis when synchronizing Link targets."),
            LOCTEXT("PositionZTooltip", "Include the Z position axis when synchronizing Link targets."))),
        FText::GetEmpty(),
        true,
        false));

    FToolMenuSection& RotationSection = Menu->AddSection(
        TEXT("Link.Rotation"),
        LOCTEXT("RotationSection", "Rotation"));
    RotationSection.AddEntry(FToolMenuEntry::InitWidget(
        TEXT("Link.RotationAxes"),
        WrapMenuContent(MakeAxisSegments(
            &bLinkPitch,
            &bLinkYaw,
            &bLinkRoll,
            LOCTEXT("Pitch", "Pitch"),
            LOCTEXT("Yaw", "Yaw"),
            LOCTEXT("Roll", "Roll"),
            LOCTEXT("PitchTooltip", "Include Pitch when synchronizing rotation to perspective Link targets."),
            LOCTEXT("YawTooltip", "Include Yaw when synchronizing rotation to perspective Link targets."),
            LOCTEXT("RollTooltip", "Include Roll when synchronizing rotation to perspective Link targets."))),
        FText::GetEmpty(),
        true,
        false));

    TSharedRef<SSegmentedControl<LinkUI::ELinkMode>> ModeControl =
        SNew(SSegmentedControl<LinkUI::ELinkMode>)
        .Value_Lambda([this]()
        {
            return bAbsoluteMode ? LinkUI::ELinkMode::Absolute : LinkUI::ELinkMode::Relative;
        })
        .OnValueChanged_Lambda([this](LinkUI::ELinkMode NewMode)
        {
            SetAbsoluteMode(NewMode == LinkUI::ELinkMode::Absolute);
        });

    ModeControl->AddSlot(LinkUI::ELinkMode::Relative, false)
        .Text(LOCTEXT("RelativeMode", "Relative"))
        .ToolTip(LOCTEXT(
            "RelativeModeTooltip",
            "Targets follow source camera movement deltas while preserving each target's existing position and rotation offsets."));

    ModeControl->AddSlot(LinkUI::ELinkMode::Absolute, false)
        .Text(LOCTEXT("AbsoluteMode", "Absolute"))
        .ToolTip(LOCTEXT(
            "AbsoluteModeTooltip",
            "Targets continuously match the source on the enabled axes whenever the source camera changes. Disabled axes remain independent."));

    ModeControl->RebuildChildren();

    TSharedRef<SWidget> ModeRow =
        SNew(SBox)
        .WidthOverride(LinkUI::SettingsRowWidth)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(1.0f)
            .VAlign(VAlign_Center)
            [
                ModeControl
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(8.0f, 0.0f, 0.0f, 0.0f)
            .VAlign(VAlign_Bottom)
            [
                SNew(SButton)
                .ContentPadding(FMargin(10.0f, 2.5f))
                .ToolTipText(LOCTEXT(
                    "ClearTooltip",
                    "Clear the current Link source and targets, disable Link, and restore the default axis and mode settings. Viewport cameras are not moved."))
                .OnClicked_Lambda([this]()
                {
                    ClearLinkSetup();
                    return FReply::Handled();
                })
                [
                    SNew(STextBlock)
                    .TextStyle(LinkUI::SegmentTextStyle())
                    .Text(LOCTEXT("ClearButton", "Clear"))
                ]
            ]
        ];

    FToolMenuSection& ModeSection = Menu->AddSection(
        TEXT("Link.Mode"),
        LOCTEXT("ModeSection", "Mode"));
    ModeSection.AddEntry(FToolMenuEntry::InitWidget(
        TEXT("Link.ModeRow"),
        WrapMenuContent(ModeRow),
        FText::GetEmpty(),
        true,
        false));
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

    // Public Level Editor viewport identity in UE 5.8.
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

    // Link is an editor navigation aid and should never drive runtime viewports.
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

    const bool bSourcePositionChanged = !LocationDelta.IsNearlyZero();
    const bool bSourceRotationChanged = !RotationDelta.IsNearlyZero(KINDA_SMALL_NUMBER);
    if (!bSourcePositionChanged && !bSourceRotationChanged)
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

        if (bSourcePositionChanged && (bLinkPositionX || bLinkPositionY || bLinkPositionZ))
        {
            FVector TargetLocation = TargetClient->GetViewLocation();

            if (bAbsoluteMode)
            {
                if (bLinkPositionX) TargetLocation.X = CurrentSourceLocation.X;
                if (bLinkPositionY) TargetLocation.Y = CurrentSourceLocation.Y;
                if (bLinkPositionZ) TargetLocation.Z = CurrentSourceLocation.Z;
            }
            else
            {
                if (bLinkPositionX) TargetLocation.X += LocationDelta.X;
                if (bLinkPositionY) TargetLocation.Y += LocationDelta.Y;
                if (bLinkPositionZ) TargetLocation.Z += LocationDelta.Z;
            }

            TargetClient->SetViewLocation(TargetLocation);
            bChangedTarget = true;
        }

        // Orthographic views keep their projection orientation. Rotation controls apply only to perspective targets.
        if (TargetClient->GetViewportType() == LVT_Perspective && bSourceRotationChanged && (bLinkPitch || bLinkYaw || bLinkRoll))
        {
            FRotator TargetRotation = TargetClient->GetViewRotation();

            if (bAbsoluteMode)
            {
                if (bLinkPitch) TargetRotation.Pitch = CurrentSourceRotation.Pitch;
                if (bLinkYaw) TargetRotation.Yaw = CurrentSourceRotation.Yaw;
                if (bLinkRoll) TargetRotation.Roll = CurrentSourceRotation.Roll;
            }
            else
            {
                if (bLinkPitch) TargetRotation.Pitch += RotationDelta.Pitch;
                if (bLinkYaw) TargetRotation.Yaw += RotationDelta.Yaw;
                if (bLinkRoll) TargetRotation.Roll += RotationDelta.Roll;
            }

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
    if (ViewportKey.IsNone())
    {
        return;
    }

    if (IsSource(ViewportKey))
    {
        // A viewport cannot be both roles. Choosing Target clears Source.
        SourceViewportKey = NAME_None;
        TargetViewportKeys.Add(ViewportKey);
        ResetSourceSample();
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

void FLinkModule::SetAbsoluteMode(bool bInAbsoluteMode)
{
    if (bAbsoluteMode == bInAbsoluteMode)
    {
        return;
    }

    bAbsoluteMode = bInAbsoluteMode;
    ResetSourceSample();
}

bool FLinkModule::CanSyncTargets() const
{
    if (!GEditor || GEditor->PlayWorld != nullptr || SourceViewportKey.IsNone() || TargetViewportKeys.IsEmpty())
    {
        return false;
    }

    FLevelEditorViewportClient* SourceClient = GetViewportClient(SourceViewportKey);
    return SourceClient
        && SourceClient->GetViewportType() == LVT_Perspective
        && !SourceClient->IsAnyActorLocked()
        && !SourceClient->IsLockedToCinematic();
}

void FLinkModule::SyncTargets()
{
    if (!CanSyncTargets())
    {
        return;
    }

    FLevelEditorViewportClient* SourceClient = GetViewportClient(SourceViewportKey);
    if (!SourceClient)
    {
        return;
    }

    const FVector SourceLocation = SourceClient->GetViewLocation();
    const FRotator SourceRotation = SourceClient->GetViewRotation();

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

        if (bLinkPositionX || bLinkPositionY || bLinkPositionZ)
        {
            FVector TargetLocation = TargetClient->GetViewLocation();
            if (bLinkPositionX) TargetLocation.X = SourceLocation.X;
            if (bLinkPositionY) TargetLocation.Y = SourceLocation.Y;
            if (bLinkPositionZ) TargetLocation.Z = SourceLocation.Z;
            TargetClient->SetViewLocation(TargetLocation);
            bChangedTarget = true;
        }

        if (TargetClient->GetViewportType() == LVT_Perspective && (bLinkPitch || bLinkYaw || bLinkRoll))
        {
            FRotator TargetRotation = TargetClient->GetViewRotation();
            if (bLinkPitch) TargetRotation.Pitch = SourceRotation.Pitch;
            if (bLinkYaw) TargetRotation.Yaw = SourceRotation.Yaw;
            if (bLinkRoll) TargetRotation.Roll = SourceRotation.Roll;
            TargetClient->SetViewRotation(TargetRotation.GetNormalized());
            bChangedTarget = true;
        }

        if (bChangedTarget)
        {
            TargetClient->Invalidate();
        }
    }

    ResetSourceSample();
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
    bAbsoluteMode = false;

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

IMPLEMENT_MODULE(FLinkModule, Link)

#undef LOCTEXT_NAMESPACE

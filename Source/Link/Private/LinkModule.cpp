// Copyright Epic Games, Inc. All Rights Reserved.

#include "LinkModule.h"

#include "Editor.h"
#include "Editor/UnrealEdTypes.h"
#include "LevelEditorMenuContext.h"
#include "LevelEditorViewport.h"
#include "SLevelViewport.h"
#include "Styling/AppStyle.h"
#include "ToolMenus.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/StyleColors.h"
#include "Widgets/Layout/SSeparator.h"

#define LOCTEXT_NAMESPACE "Link"

namespace LinkUI
{
    constexpr float ToolbarButtonHeight = 24.0f;
    constexpr float ToolbarGap = 2.0f;
    constexpr float ToolbarButtonPaddingX = 7.0f;
    constexpr float RoleColumnWidth = 18.0f;
    constexpr float SegmentHeight = 24.0f;
    constexpr float PopupWidth = 316.0f;
    constexpr float SegmentWidthShort = 86.0f;
    constexpr float SegmentWidthLong = 112.0f;

    const FCheckBoxStyle* ToggleStyle()
    {
        return &FAppStyle::Get().GetWidgetStyle<FCheckBoxStyle>(TEXT("ToggleButtonCheckbox"));
    }

    const FCheckBoxStyle* RadioStyle()
    {
        return &FAppStyle::Get().GetWidgetStyle<FCheckBoxStyle>(TEXT("RadioButton"));
    }

    const FCheckBoxStyle* PropertySegmentStyle(const FName StyleName)
    {
        return &FAppStyle::Get().GetWidgetStyle<FCheckBoxStyle>(StyleName);
    }

    const FButtonStyle* SimpleButtonStyle()
    {
        return &FAppStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("SimpleButton"));
    }

    const FComboButtonStyle* SimpleComboButtonStyle()
    {
        return &FAppStyle::Get().GetWidgetStyle<FComboButtonStyle>(TEXT("SimpleComboButton"));
    }

    const FTextBlockStyle* SmallTextStyle()
    {
        return &FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>(TEXT("SmallText"));
    }
}

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

    return SNew(SHorizontalBox)

        // 1. One-shot absolute sync.
        + SHorizontalBox::Slot()
        .AutoWidth()
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .HeightOverride(LinkUI::ToolbarButtonHeight)
            [
                SNew(SButton)
                .ButtonStyle(LinkUI::SimpleButtonStyle())
                .ContentPadding(FMargin(LinkUI::ToolbarButtonPaddingX, 1.0f))
                .IsEnabled_Lambda([this]() { return CanSyncTargets(); })
                .ToolTipText(LOCTEXT(
                    "SyncTooltip",
                    "Immediately align all Link targets to the source viewport on the enabled position and rotation axes. This is a one-shot absolute sync and works independently of Relative or Absolute mode."))
                .OnClicked_Lambda([this]()
                {
                    SyncTargets();
                    return FReply::Handled();
                })
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .VAlign(VAlign_Center)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("↺")))
                    ]
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .VAlign(VAlign_Center)
                    .Padding(4.0f, 0.0f, 0.0f, 0.0f)
                    [
                        SNew(STextBlock)
                        .Text(LOCTEXT("SyncButton", "Sync"))
                    ]
                ]
            ]
        ]

        + SHorizontalBox::Slot()
        .AutoWidth()
        .Padding(LinkUI::ToolbarGap, 0.0f)
        .VAlign(VAlign_Center)
        [
            // 2. Global Link enable / suspend toggle.
            SNew(SBox)
            .HeightOverride(LinkUI::ToolbarButtonHeight)
            [
                SNew(SCheckBox)
                .Style(LinkUI::ToggleStyle())
                .Padding(FMargin(LinkUI::ToolbarButtonPaddingX, 1.0f))
                .IsChecked_Lambda([this]()
                {
                    return bLinkEnabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
                })
                .OnCheckStateChanged_Lambda([this](ECheckBoxState)
                {
                    ToggleEnabled();
                })
                .ToolTipText(LOCTEXT(
                    "LinkToggleTooltip",
                    "Enable or suspend linked viewport camera movement. Disabling Link preserves the current source, targets, axis selections, and mode."))
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("LinkButton", "Link"))
                ]
            ]
        ]

        + SHorizontalBox::Slot()
        .AutoWidth()
        .Padding(0.0f, 0.0f, LinkUI::ToolbarGap, 0.0f)
        .VAlign(VAlign_Center)
        [
            // 3 / 4. Compact stacked Source and Target radial controls.
            SNew(SBorder)
            .Padding(FMargin(2.0f, 1.0f))
            .BorderImage(FAppStyle::GetBrush(TEXT("SimpleButton")))
            [
                SNew(SBox)
                .WidthOverride(LinkUI::RoleColumnWidth)
                .HeightOverride(LinkUI::ToolbarButtonHeight - 2.0f)
                [
                    SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    SNew(SCheckBox)
                    .Style(LinkUI::RadioStyle())
                    .Padding(FMargin(0.0f))
                    .IsChecked_Lambda([this, ViewportKey]()
                    {
                        return IsSource(ViewportKey) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
                    })
                    .OnCheckStateChanged_Lambda([this, ViewportKey](ECheckBoxState)
                    {
                        ToggleSource(ViewportKey);
                    })
                    .ToolTipText(LOCTEXT(
                        "SourceRoleTooltip",
                        "Make this viewport the Link source. Only one Level Editor viewport can be the source at a time. Choosing a new source automatically clears the previous source."))
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    SNew(SCheckBox)
                    .Style(LinkUI::RadioStyle())
                    .Padding(FMargin(0.0f))
                    .IsChecked_Lambda([this, ViewportKey]()
                    {
                        return IsTarget(ViewportKey) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
                    })
                    .OnCheckStateChanged_Lambda([this, ViewportKey](ECheckBoxState)
                    {
                        ToggleTarget(ViewportKey);
                    })
                    .ToolTipText(LOCTEXT(
                        "TargetRoleTooltip",
                        "Make this viewport a Link target. Any number of viewports can be targets, but the source viewport cannot also be a target."))
                ]
                ]
            ]
        ]

        // 5. Settings popup.
        + SHorizontalBox::Slot()
        .AutoWidth()
        .VAlign(VAlign_Center)
        [
            SNew(SBox)
            .HeightOverride(LinkUI::ToolbarButtonHeight)
            [
                SNew(SComboButton)
                .ComboButtonStyle(LinkUI::SimpleComboButtonStyle())
                .HasDownArrow(false)
                .ContentPadding(FMargin(LinkUI::ToolbarButtonPaddingX, 1.0f))
                .ToolTipText(LOCTEXT(
                    "SettingsTooltip",
                    "Configure linked position axes, rotation axes, synchronization mode, or clear the current Link setup."))
                .OnGetMenuContent_Lambda([this, WeakViewport]()
                {
                    return BuildSettingsPopup(WeakViewport);
                })
                .ButtonContent()
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .VAlign(VAlign_Center)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("⚙")))
                    ]
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .VAlign(VAlign_Center)
                    .Padding(4.0f, 0.0f, 0.0f, 0.0f)
                    [
                        SNew(STextBlock)
                        .Text(LOCTEXT("SettingsButton", "Settings"))
                    ]
                ]
            ]
        ];
}

TSharedRef<SWidget> FLinkModule::BuildSettingsPopup(TWeakPtr<SLevelViewport> WeakViewport)
{
    const TSharedPtr<SLevelViewport> Viewport = WeakViewport.Pin();
    if (Viewport.IsValid())
    {
        RegisterViewport(Viewport);
    }

    auto MakeToggleSegment = [](const FText& Label, const FText& Tooltip, bool* ValuePtr, float Width, const FName StyleName) -> TSharedRef<SWidget>
    {
        return SNew(SBox)
            .WidthOverride(Width)
            .HeightOverride(LinkUI::SegmentHeight)
            [
                SNew(SCheckBox)
                .Style(LinkUI::PropertySegmentStyle(StyleName))
                .Padding(FMargin(8.0f, 2.0f))
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
                    SNew(SBox)
                    .HAlign(HAlign_Center)
                    .VAlign(VAlign_Center)
                    [
                        SNew(STextBlock)
                        .Justification(ETextJustify::Center)
                        .Text(Label)
                    ]
                ]
            ];
    };

    auto MakeSegmentGroup = [](const TSharedRef<SWidget>& A, const TSharedRef<SWidget>& B, const TSharedRef<SWidget>& C) -> TSharedRef<SWidget>
    {
        return SNew(SBorder)
            .Padding(FMargin(1.0f))
            .BorderImage(FAppStyle::GetBrush(TEXT("ToolPanel.GroupBorder")))
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().AutoWidth()[A]
                + SHorizontalBox::Slot().AutoWidth()[B]
                + SHorizontalBox::Slot().AutoWidth()[C]
            ];
    };

    const TSharedRef<SWidget> PositionSegments = MakeSegmentGroup(
        MakeToggleSegment(
            LOCTEXT("PositionX", "X"),
            LOCTEXT("PositionXTooltip", "Include the X position axis when synchronizing Link targets."),
            &bLinkPositionX,
            LinkUI::SegmentWidthShort,
            TEXT("Property.ToggleButton.Start")),
        MakeToggleSegment(
            LOCTEXT("PositionY", "Y"),
            LOCTEXT("PositionYTooltip", "Include the Y position axis when synchronizing Link targets."),
            &bLinkPositionY,
            LinkUI::SegmentWidthShort,
            TEXT("Property.ToggleButton.Middle")),
        MakeToggleSegment(
            LOCTEXT("PositionZ", "Z"),
            LOCTEXT("PositionZTooltip", "Include the Z position axis when synchronizing Link targets."),
            &bLinkPositionZ,
            LinkUI::SegmentWidthShort,
            TEXT("Property.ToggleButton.End")));

    const TSharedRef<SWidget> RotationSegments = MakeSegmentGroup(
        MakeToggleSegment(
            LOCTEXT("Pitch", "Pitch"),
            LOCTEXT("PitchTooltip", "Include Pitch when synchronizing rotation to perspective Link targets."),
            &bLinkPitch,
            LinkUI::SegmentWidthShort,
            TEXT("Property.ToggleButton.Start")),
        MakeToggleSegment(
            LOCTEXT("Yaw", "Yaw"),
            LOCTEXT("YawTooltip", "Include Yaw when synchronizing rotation to perspective Link targets."),
            &bLinkYaw,
            LinkUI::SegmentWidthShort,
            TEXT("Property.ToggleButton.Middle")),
        MakeToggleSegment(
            LOCTEXT("Roll", "Roll"),
            LOCTEXT("RollTooltip", "Include Roll when synchronizing rotation to perspective Link targets."),
            &bLinkRoll,
            LinkUI::SegmentWidthShort,
            TEXT("Property.ToggleButton.End")));

    const TSharedRef<SWidget> RelativeSegment =
        SNew(SBox)
        .WidthOverride(LinkUI::SegmentWidthLong)
        .HeightOverride(LinkUI::SegmentHeight)
        [
            SNew(SCheckBox)
            .Style(LinkUI::PropertySegmentStyle(TEXT("Property.ToggleButton.Start")))
            .Padding(FMargin(8.0f, 2.0f))
            .IsChecked_Lambda([this]()
            {
                return !bAbsoluteMode ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
            })
            .OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
            {
                if (NewState == ECheckBoxState::Checked)
                {
                    SetAbsoluteMode(false);
                }
            })
            .ToolTipText(LOCTEXT(
                "RelativeModeTooltip",
                "Relative mode applies source camera movement deltas to targets while preserving each target's existing position and rotation offsets."))
            [
                SNew(SBox)
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    SNew(STextBlock)
                    .Justification(ETextJustify::Center)
                    .Text(LOCTEXT("RelativeMode", "Relative"))
                ]
            ]
        ];

    const TSharedRef<SWidget> AbsoluteSegment =
        SNew(SBox)
        .WidthOverride(LinkUI::SegmentWidthLong)
        .HeightOverride(LinkUI::SegmentHeight)
        [
            SNew(SCheckBox)
            .Style(LinkUI::PropertySegmentStyle(TEXT("Property.ToggleButton.End")))
            .Padding(FMargin(8.0f, 2.0f))
            .IsChecked_Lambda([this]()
            {
                return bAbsoluteMode ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
            })
            .OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
            {
                if (NewState == ECheckBoxState::Checked)
                {
                    SetAbsoluteMode(true);
                }
            })
            .ToolTipText(LOCTEXT(
                "AbsoluteModeTooltip",
                "Absolute mode makes targets match the source on the enabled axes whenever the source camera changes. Disabled axes remain independent."))
            [
                SNew(SBox)
                .HAlign(HAlign_Center)
                .VAlign(VAlign_Center)
                [
                    SNew(STextBlock)
                    .Justification(ETextJustify::Center)
                    .Text(LOCTEXT("AbsoluteMode", "Absolute"))
                ]
            ]
        ];

    const TSharedRef<SWidget> ModeSegments =
        SNew(SBorder)
        .Padding(FMargin(1.0f))
        .BorderImage(FAppStyle::GetBrush(TEXT("ToolPanel.GroupBorder")))
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot().AutoWidth()[RelativeSegment]
            + SHorizontalBox::Slot().AutoWidth()[AbsoluteSegment]
        ];

    return SNew(SBox)
        .WidthOverride(LinkUI::PopupWidth)
        [
            SNew(SBorder)
            .Padding(FMargin(10.0f, 9.0f))
            .BorderImage(FAppStyle::GetBrush(TEXT("Menu.Background")))
            [
                SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0.0f, 0.0f, 0.0f, 4.0f)
                [
                    SNew(STextBlock)
                    .TextStyle(LinkUI::SmallTextStyle())
                    .ColorAndOpacity(FSlateColor::UseSubduedForeground())
                    .Text(LOCTEXT("LocationSection", "Location"))
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0.0f, 0.0f, 0.0f, 8.0f)
                [
                    PositionSegments
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0.0f, 0.0f, 0.0f, 8.0f)
                [
                    SNew(SSeparator)
                    .Orientation(Orient_Horizontal)
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0.0f, 0.0f, 0.0f, 4.0f)
                [
                    SNew(STextBlock)
                    .TextStyle(LinkUI::SmallTextStyle())
                    .ColorAndOpacity(FSlateColor::UseSubduedForeground())
                    .Text(LOCTEXT("RotationSection", "Rotation"))
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0.0f, 0.0f, 0.0f, 8.0f)
                [
                    RotationSegments
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0.0f, 0.0f, 0.0f, 8.0f)
                [
                    SNew(SSeparator)
                    .Orientation(Orient_Horizontal)
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(0.0f, 0.0f, 0.0f, 4.0f)
                [
                    SNew(STextBlock)
                    .TextStyle(LinkUI::SmallTextStyle())
                    .ColorAndOpacity(FSlateColor::UseSubduedForeground())
                    .Text(LOCTEXT("ModeSection", "Mode"))
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    [
                        ModeSegments
                    ]
                    + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    [
                        SNullWidget::NullWidget
                    ]
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(8.0f, 0.0f, 0.0f, 0.0f)
                    .VAlign(VAlign_Center)
                    [
                        SNew(SBox)
                        .HeightOverride(LinkUI::SegmentHeight)
                        [
                            SNew(SButton)
                            .ButtonStyle(LinkUI::SimpleButtonStyle())
                            .ContentPadding(FMargin(10.0f, 2.0f))
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
                                .Text(LOCTEXT("ClearButton", "Clear"))
                            ]
                        ]
                    ]
                ]
            ]
        ];
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

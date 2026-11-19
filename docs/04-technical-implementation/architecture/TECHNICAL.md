# Module and Toolbar Architecture — Technical

**Link v1.0.0 · Source reference**

## Startup and teardown

`FLinkModule::StartupModule` registers a `FSlateStyleSet`, an `FTSTicker` callback and a ToolMenus startup registration callback. `ShutdownModule` removes ticker and menu callbacks, unregisters the style, and clears viewport keys/state.

## Native toolbar extension

<figure class="doc-media" data-kind="image" data-media-id="native-toolmenus">
  <img src="../../assets/media-placeholder.png" alt="Architecture of the Link ToolMenus and Slate viewport integration" loading="lazy">
  <figcaption>Link registers native ToolMenus entries and tracks viewport widgets by configuration identity.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Create a labeled diagram showing RegisterMenus → BuildToolbarEntries → SLevelViewport/GetConfigKey → RegisteredViewports; include Source and Target key collections and native Slate icon/style registration.</span></figcaption>
</figure>


`RegisterMenus` extends `LevelEditor.ViewportToolbar`, Right section, before Camera using a dynamic ToolMenus entry. `BuildToolbarEntries` finds `UUnrealEdViewportToolbarContext` and `SLevelViewport`, then creates native Sync and Link entries, a role widget and a Settings submenu.

## Slate controls

Role selectors use native radio-style `SCheckBox`; Settings uses a segmented-control style with `SUniformGridPanel` axis choices, `SSegmentedControl` for Relative/Absolute and an `SButton` for Clear. Icon brushes load SVGs from the plugin's `Resources/` directory.

## Viewport keys

`GetViewportKey` uses `SLevelViewport::GetConfigKey()`; weak `TSharedPtr` references are stored in `TMap<FName,TWeakPtr<SLevelViewport>> RegisteredViewports`, and Target keys are a `TSet<FName>`. Rebuilt viewports can reuse their config keys.

## Compilation scope

`Link.Build.cs` depends on UnrealEd, LevelEditor, Slate, SlateCore, ToolMenus and Projects. `Link.uplugin` declares an Editor-only Win64 module loaded post-engine initialization. No engine source patch is required.

---

For user steps, [read Design](DESIGN.md).

## Code references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

Source-review only; editor regression results belong in the Verification Matrix.

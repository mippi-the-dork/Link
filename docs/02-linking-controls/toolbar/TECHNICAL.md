# Toolbar Controls — Technical

**Link v1.0.0 · Source-reviewed implementation**

## Toolbar registration

`FLinkModule::RegisterMenus` extends `LevelEditor.ViewportToolbar` and adds a dynamic entry to its `Right` section, inserted before `Camera`. This is an editor toolbar extension, not a custom window or Actor component.

## Entry construction

<figure class="doc-media" data-kind="image" data-media-id="toolbar-registration">
  <img src="../../assets/media-placeholder.png" alt="Diagram of the native Link toolbar entry construction" loading="lazy">
  <figcaption>The Link toolbar is created through Unreal ToolMenus and uses the current Level Editor viewport context.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Create an annotated call-flow diagram from RegisterMenus to BuildToolbarEntries, showing LevelEditor.ViewportToolbar context, Sync/Link actions, role widget and Settings submenu.</span></figcaption>
</figure>


`BuildToolbarEntries` requires a valid `UUnrealEdViewportToolbarContext` and `SLevelViewport`. It registers the viewport by its configuration key and creates:

| Entry | Native implementation |
| --- | --- |
| Sync | `FToolMenuSection::AddMenuEntry` bound to `SyncTargets`, gated by `CanSyncTargets` |
| Link | Toggle menu entry bound to `ToggleEnabled`, with `bLinkEnabled` checked state |
| Roles | `FToolMenuEntry::InitWidget` with `MakeRoleWidget(ViewportKey)` |
| Settings | `AddSubMenu` bound to `PopulateSettingsMenu` |

## Icons and presentation

`RegisterStyle` creates a `FSlateStyleSet` using SVG brushes in `Resources/`; toolbar entries use native Slate styles and tooltips. Sync, Link and Settings are icon-only in the toolbar, and the Source/Target selectors are small vertically stacked radio-style controls.

## Cross-viewport state

The Link enabled state, selected Source, Targets, axes and mode belong to the `FLinkModule` instance, so actions in any viewport toolbar affect the shared configuration. Only the role selectors operate on the specific viewport's key.

## One-shot command eligibility

`CanSyncTargets` checks that the Editor is not in PIE, Source and at least one Target are configured, the Source client exists and is Perspective, and the Source is not Actor/cinematic-locked. The command can remain enabled even if every configured Target is currently invalid or locked; those Targets are individually skipped by `SyncTargets`.

---

For step-by-step use, [read Design](DESIGN.md).

## Source references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

**Verification:** Implementation inspected in v1.0.0 source; comprehensive live-editor behavior testing remains separately tracked in the verification matrix.

# Updating Link and Support — Technical

**Link v1.0.0 · Source reference**

## Distribution declaration

`Link.uplugin` version 1.0.0 (`Version:9`) declares `EngineVersion:5.8.0`, `Type:Editor`, `LoadingPhase:PostEngineInit`, and `PlatformAllowList:[Win64]`. `CanContainContent` is false.

## Build module

`Source/Link/Link.Build.cs` includes core module dependencies and editor-specific UnrealEd, LevelEditor, Slate, SlateCore, ToolMenus and Projects. The plugin must be built into an Editor target.

## Update boundaries

The plugin state isn't serialized as a Link data asset or to level package metadata; restarting resets its configuration. Replacing the plugin should not require migrating saved Link roles/axes. Native Unreal viewport layouts and user editor settings are outside Link's own persistence system.

## Debug-report checklist

For crashes or toolbar failures, include full engine version, plugin commit/release, stack trace, when the module was loaded, context (Level Editor vs Asset Editor), and related ToolMenus/Slate log details.

For synchronization defects, include exact Source/Target configuration keys if available, projection types, masks, lock state, PIE status and a recording.

## Maintenance expectations

A new release should update the plugin descriptor, README, docs manifest, both Design and Technical pages affected, supported build targets and confirmed QA results together. Source links currently track `main` and should be pinned to release tags/commits if maintaining historical versions.

---

For practical workflows, [read Design](DESIGN.md).

## Code references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

**QA scope:** Source inspected, not a live editor test report.

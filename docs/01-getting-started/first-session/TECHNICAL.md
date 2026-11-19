# First Viewport Setup — Technical

**Link v1.0.0 · Source-reviewed implementation**

## Per-viewport toolbar context

`FLinkModule::BuildToolbarEntries` retrieves `UUnrealEdViewportToolbarContext`, pins the `SLevelViewport`, registers it with `RegisteredViewports`, and looks up the stable identity through `SLevelViewport::GetConfigKey()`.

## Role selection

The upper small radio-style control calls `ToggleSource(ViewportKey)`; the lower calls `ToggleTarget(ViewportKey)`. A viewport cannot hold both roles, and a new Source replaces the previous Source. Several Targets can remain selected in a `TSet<FName>`.

## Initial defaults

`bLinkEnabled=false`, position X/Y/Z flags true, rotation Pitch/Yaw/Roll flags false, and `bAbsoluteMode=false` (Relative). Source and Target sets start empty. The state is in-memory and is not loaded from an asset.

## First update and baseline

The first tick after activation or reset records Source location and rotation and intentionally performs **no propagated movement**. Later ticks calculate differences from that sample. The one-shot `SyncTargets` command does not need a stored movement delta.

## Source camera eligibility

`TickLink` requires a valid Perspective Source not locked to an Actor or cinematic camera, and a nonempty Target set; it skips updates during PIE. A Source selection on an orthographic viewport does not satisfy this runtime eligibility check.

## Target application

Only valid registered unlocked Target viewport clients are updated. Their camera values change through viewport-client setters followed by an invalidation for redraw; the World is not edited.

---

For step-by-step use, [read Design](DESIGN.md).

## Source references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

**Verification:** Implementation inspected in v1.0.0 source; comprehensive live-editor behavior testing remains separately tracked in the verification matrix.

# Source and Targets — Technical

**Link v1.0.0 · Source-reviewed implementation**

## Internal identity

`FLinkModule::GetViewportKey` calls `SLevelViewport::GetConfigKey()`; `RegisteredViewports` maps each key to a `TWeakPtr<SLevelViewport>`. The Source is `FName SourceViewportKey` and Targets are `TSet<FName> TargetViewportKeys`.

## Exclusivity invariants

`ToggleSource` clears its own Source selection when clicked again; otherwise assigns the key as Source and removes that key from the Target set. `ToggleTarget` clears Source if given the Source key; otherwise toggles membership in the Target set. This ensures a key is never simultaneously Source and Target.

## Source replacement and invalidation

Changing Source calls `ResetSourceSample` so prior camera movement does not carry across roles. Target membership does not require an Actor or runtime object. Multiple target keys remain independently selected.

## Viewport rebuilds

`PruneInvalidViewports` removes expired weak viewports from the active map. A Source key absent from that map is intentionally retained so a rebuilt viewport with the same config identity can reconnect; its previous Source sample is discarded. Stored Target keys likewise may reconnect when the corresponding viewport is reconstructed.

## Eligibility is separate from role

A role selector can store a Source identity on an orthographic viewport, but `CanSyncTargets` and `TickLink` require a Perspective Source to execute. That is not the same as the source selector being a validation gate.

---

For step-by-step use, [read Design](DESIGN.md).

## Source references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

**Verification:** Implementation inspected in v1.0.0 source; comprehensive live-editor behavior testing remains separately tracked in the verification matrix.

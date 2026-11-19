# Frequently Asked Questions — Technical

**Link v1.0.0 · Source reference**

## FAQ implementation map

| User question | Source implementation |
| --- | --- |
| Sync while Link disabled | `CanSyncTargets` / `SyncTargets` do not depend on `bLinkEnabled` |
| Mode change without movement | `SetAbsoluteMode` resets Source sample |
| Missed movement not caught up | `ToggleEnabled` / `ResetSourceSample` |
| Orthographic rotation | `TargetClient->GetViewportType() == LVT_Perspective` |
| Orthographic Source rejected | `CanSyncTargets`, `TickLink` |
| Camera lock | `IsAnyActorLocked`, `IsLockedToCinematic` |
| PIE suspension | `GEditor->PlayWorld != nullptr` |
| Multiple Targets | `TSet<FName> TargetViewportKeys` |
| Session-only | No serialized Link state or settings asset |
| No Actor changes | Direct `FLevelEditorViewportClient` getters/setters |

## Distinct failure categories

A selected Source role does not guarantee runtime eligibility; a valid Target may still be locked; an enabled axis may have no corresponding Source delta. Diagnose those independently rather than treating every nonmovement as failure to register ToolMenus.

## Verification caution

This FAQ summarizes code and README behavior for v1.0.0. Reproducible tests of unusual editor layouts, viewport rebuilds and PIE belong in the Verification Matrix.

---

For practical workflows, [read Design](DESIGN.md).

## Code references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

**QA scope:** Source inspected, not a live editor test report.

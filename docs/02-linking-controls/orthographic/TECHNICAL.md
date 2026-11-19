# Orthographic Viewports — Technical

**Link v1.0.0 · Source reference**

## Projection checks

`CanSyncTargets` and `TickLink` require the Source client type to be `LVT_Perspective`. The Source role selector can store another viewport key; the type requirement is enforced during operation, not assignment.

## Target behavior

Any valid, unlocked target type receives enabled X/Y/Z changes through `SetViewLocation`. Rotation writes in both Sync and continuous linking are guarded by `TargetClient->GetViewportType() == LVT_Perspective`.

## Zoom

No orthographic zoom getter/setter is used in Link's source; `SetOrthoZoom` does not appear in the algorithm.

## Lock handling

Actor-locked or cinematic-locked orthographic Targets are skipped just like locked Perspective Targets. The rest of the Target loop continues.

## Scope

Only standard Level Editor viewports are registered through the Level Editor toolbar context; specialized Asset Editor viewports are outside this registration path.

---

For use cases, [read Design](DESIGN.md).

## Source references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

Source-reviewed v1.0.0 behavior; actual editor QA remains tracked separately.

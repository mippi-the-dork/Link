# Camera Synchronization Algorithm — Technical

**Link v1.0.0 · Source reference**

## Tick and early-outs

`TickLink` runs through `FTSTicker`, skips recursive application, prunes invalid viewport references and returns early if Link isn't enabled, Source absent, Targets empty, PIE active or Source invalid, orthographic or locked.

## Baseline sampling

After eligibility checks, if no prior sample exists, current Source location and rotation are stored and the tick returns. Otherwise location delta is current minus previous; rotation delta is normalized current-minus-previous `FRotator`.

## Change thresholds

Location uses `LocationDelta.IsNearlyZero()`; rotation uses `RotationDelta.IsNearlyZero(KINDA_SMALL_NUMBER)`. If neither changes, there is no Target loop update.

## Target computation

Each valid unlocked Target is evaluated independently. Position is updated only when Source position changed and a position flag is on; Absolute assigns Source values while Relative adds component deltas. Rotation is processed only for Perspective Targets when Source rotation changed and a rotation flag is on.

## Reentrancy and redraw

`TGuardValue<bool> ApplyingGuard(bApplyingLinkedUpdate,true)` prevents reentrant propagation. Changed Targets receive `FLevelEditorViewportClient::Invalidate()`.

## Sync bypasses delta requirement

`SyncTargets` directly assigns enabled Source values, independent of mode and previous sample. It finishes by resetting the sample so subsequent relative ticks don't count the Sync itself as a Source delta. There is no Actor transaction because no level Actor is modified.

---

For user steps, [read Design](DESIGN.md).

## Code references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

Source-review only; editor regression results belong in the Verification Matrix.

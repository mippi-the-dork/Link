# One-Shot Sync — Technical

**Link v1.0.0 · Source reference**

## Toolbar gating

`CanSyncTargets` requires GEditor, no PIE, a configured Source and at least one Target, plus a valid Perspective Source client without Actor/cinematic lock. The toolbar command is bound to `SyncTargets` with this eligibility check.

## Source and Targets

`SyncTargets` reads Source location/rotation and uses `TGuardValue<bool>` to mark an ongoing application. Each Target is checked separately for validity, role collision and camera locks.

## Direct values

Selected X/Y/Z are copied individually to Target location. Selected Pitch/Yaw/Roll are copied only to Perspective Targets, and the result is normalized. Changed clients are invalidated for redraw.

## Reset after action

`ResetSourceSample` runs after Sync, so ongoing Relative linking does not treat the sync adjustment as a movement delta. Sync does not alter Link enabled state, mode or axis masks.

---

For use cases, [read Design](DESIGN.md).

## Source references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

Source-reviewed v1.0.0 behavior; actual editor QA remains tracked separately.

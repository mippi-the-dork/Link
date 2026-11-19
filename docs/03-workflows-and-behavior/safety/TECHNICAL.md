# Camera Locks and Play In Editor — Technical

**Link v1.0.0 · Source reference**

## PIE checks

`TickLink` returns early and resets its movement baseline if `GEditor->PlayWorld != nullptr`. `CanSyncTargets` also returns false in PIE.

## Locked Source

A Source client with `IsAnyActorLocked()` or `IsLockedToCinematic()` fails the continuous synchronization gate. The baseline is discarded to prevent delayed changes after unlocking.

## Locked Target

Each Target is tested independently in `TickLink` and `SyncTargets`. Invalid, Actor-locked and cinematic-locked Targets are skipped, while later valid Target keys remain eligible.

## Runtime boundary

Link is an Editor module and has no runtime module, CameraActor ownership, PlayerController modifications or replication.

## Verification scenarios

Test Source/Target actor lock independently, cinematic lock, entering PIE and returning from PIE after significant Source movement. Confirm the first post-PIE eligible tick does not cause an accumulated jump.

---

For user steps, [read Design](DESIGN.md).

## Code references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

Source-review only; editor regression results belong in the Verification Matrix.

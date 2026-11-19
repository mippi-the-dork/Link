# Absolute Mode — Technical

**Link v1.0.0 · Source reference**

## Change-triggered absolute writes

`FLinkModule::TickLink` computes Source translation/rotation deltas before updating any Target. When `bAbsoluteMode=true`, a nonzero Source position delta causes enabled Target position coordinates to be assigned to the corresponding current Source values. A nonzero rotation delta similarly assigns enabled Pitch/Yaw/Roll fields, but only on Perspective Targets.

## Why choosing Absolute alone is inert

`SetAbsoluteMode` updates the mode and calls `ResetSourceSample()`. The next eligible tick only captures the Source baseline. Axis checkbox handlers also do not immediately call any camera setters.

## Independent axes

Position and rotation updates are separate blocks. Changing Source rotation without changing position does not force an absolute position update, even if X/Y/Z are selected.

## Difference from Sync

`SyncTargets` directly copies enabled Source values without waiting for a movement delta. Both paths skip invalid or locked Target viewports, and both leave orthographic rotation untouched.

---

For use cases, [read Design](DESIGN.md).

## Source references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

Source-reviewed v1.0.0 behavior; actual editor QA remains tracked separately.

# One-Shot Sync — Design

**Link v1.0.0 · User guide**

## What Sync does

**Sync** immediately copies the enabled Source position and rotation axes to all valid Targets. It always performs an **absolute copy**, even when continuous mode is Relative or Link is disabled.

## Requirements

A valid Perspective Source, at least one configured Target, no active PIE session, and no Source Actor/cinematic camera lock. Individual locked Targets are skipped.

## Align, then establish a new offset

1. Assign Source and Target viewports.
2. Enable X/Y/Z, and optionally rotation for Perspective Targets.
3. Press **Sync** to align.
4. Move the Target into the independent offset or angle you prefer.
5. Select **Relative** and enable Link.
6. Navigate the Source; the Target now follows future deltas from its new position.

## Selective example

If X/Y/Yaw are enabled and Z/Pitch/Roll are disabled, Sync copies only X, Y and Yaw to Perspective Targets. Orthographic Targets receive X/Y only.

## What Sync does not do

Sync does not change Link enabled state, mode, Source/Target roles, camera projection type, field of view, orthographic zoom or level Actors.

---

For engine implementation, [read Technical](TECHNICAL.md).

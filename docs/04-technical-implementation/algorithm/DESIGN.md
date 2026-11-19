# Camera Synchronization Algorithm — Design

**Link v1.0.0 · User guide**

## How Link follows a camera

When Link is enabled, the plugin samples the valid Perspective Source's camera location and rotation. It compares these values with the previous sample. If the Source hasn't changed, nothing happens. If it has moved, Link applies the relevant changes to each valid Target viewport.

## Relative vs Absolute

| Mode | Position | Rotation |
| --- | --- | --- |
| Relative | Add Source movement delta on enabled X/Y/Z | Add Source rotation delta on enabled Pitch/Yaw/Roll (Perspective Targets only) |
| Absolute | Copy current Source values on enabled X/Y/Z when Source position changes | Copy current Source values on enabled Pitch/Yaw/Roll when Source rotation changes (Perspective Targets only) |

**Sync** is different: it copies enabled values immediately even if continuous Link is disabled or the Source hasn't moved.

## Special behavior

The first eligible tick establishes a movement baseline. Changing Source, mode or enabled state resets that baseline so targets don't unexpectedly catch up. Locked Targets are skipped; PIE and an unusable Source suspend updates.

## Important limitation

Source rotation does not orbit Target **positions**; relative camera position is world-axis delta movement. This isn't a camera rig or parent-child transform hierarchy.

## When investigating a discrepancy

Check whether you changed position, rotation, or both, then inspect which axes are enabled, Target projection, role identity and camera locks.

---

For native implementation, [read Technical](TECHNICAL.md).

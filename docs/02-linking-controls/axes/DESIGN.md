# Position and Rotation Axes — Design

**Link v1.0.0 · User guide**

## Open Link Settings

In any Level Editor viewport toolbar, choose **Settings**. The chosen flags are shared across all Targets.

## Position

| Axis | Default | Result |
| --- | --- | --- |
| X | On | Include world-X translation or value |
| Y | On | Include world-Y translation or value |
| Z | On | Include world-Z translation or value |

## Rotation

| Axis | Default | Result |
| --- | --- | --- |
| Pitch | Off | Follow/copy Perspective camera Pitch |
| Yaw | Off | Follow/copy Perspective camera Yaw |
| Roll | Off | Follow/copy Perspective camera Roll |

Rotation flags have no effect on orthographic Targets.

## Settings also includes

**Relative / Absolute** selects continuous mode. **Clear** removes Source and Targets, turns Link off, enables X/Y/Z, disables Pitch/Yaw/Roll and restores Relative. It never moves camera positions.

## Popular combinations

For coordinated navigation with independent facing, choose X/Y/Z in Relative with rotation off. For horizontal matching without matching height, choose X/Y in Absolute with Z off. For following turns while keeping an independent yaw offset, enable Yaw in Relative on Perspective Targets.

## New axis selections

Checking an axis does not itself move the Target. Continue moving the Source, or press **Sync** to copy the new axis immediately.

---

For engine implementation, [read Technical](TECHNICAL.md).

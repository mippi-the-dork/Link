# Session State and Reset — Design

**Link v1.0.0 · User guide**

## What Link remembers

While Unreal Editor is open, Link remembers Source, Targets, enabled state, six axis flags and Relative/Absolute mode. Turning **Link off** suspends following but does not erase this configuration.

## Re-enabling does not catch up

If Link is off, moving the Source does not move Targets. Turning Link on captures a new baseline, so those missed movements are not applied. Press **Sync** when you intentionally want immediate alignment.

## Clear returns to defaults

| Setting | After Settings → Clear |
| --- | --- |
| Continuous Link | Off |
| Source | None |
| Targets | None |
| X/Y/Z position | On |
| Pitch/Yaw/Roll rotation | Off |
| Mode | Relative |

Clear does not move the cameras or reset Unreal's layout.

## Editor restart

Link's setup is **session-only** and not saved to your map, Actor data or a Link asset. Restarting Unreal returns to defaults. The viewport layout and native editor camera persistence are separate Unreal behaviors.

## Changing Source or mode

Assigning another Source, toggling Link, or changing Relative/Absolute mode resets the Source sample to avoid applying stale movement. Axis-checkbox changes only affect later updates or the next Sync.

---

For native implementation, [read Technical](TECHNICAL.md).

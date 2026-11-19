# Session State and Reset — Design

**Link v1.0.0 · User guide**

## What Link remembers

While Unreal Editor is open, Link remembers Source, Targets, enabled state, six axis flags and Relative/Absolute mode. Turning **Link off** suspends following but does not erase this configuration.

## Re-enabling does not catch up

If Link is off, moving the Source does not move Targets. Turning Link on captures a new baseline, so those missed movements are not applied. Press **Sync** when you intentionally want immediate alignment.

## Clear returns to defaults

<figure class="doc-media" data-kind="gif" data-media-id="session-clear">
  <img src="../../assets/media-placeholder.png" alt="Link Settings Clear restoring the default Source, Target and axis configuration" loading="lazy">
  <figcaption>Clear removes Link's session roles and restores default settings without moving existing cameras.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record a linked setup with altered axes and Absolute mode, open Settings → Clear, then show Link off, no Source/Targets, X/Y/Z on, rotation off and Relative selected, with cameras stationary.</span></figcaption>
</figure>


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

# Position and Rotation Axes — Design

**Link v1.0.0 · User guide**

## Open Link Settings

<figure class="doc-media" data-kind="image" data-media-id="axis-settings">
  <img src="../../assets/media-placeholder.png" alt="Complete Link Settings menu displaying axes, modes and Clear" loading="lazy">
  <figcaption>The Settings menu provides independent X/Y/Z and Pitch/Yaw/Roll controls alongside Relative/Absolute mode and Clear.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture the full expanded Settings menu with Location X/Y/Z, Rotation Pitch/Yaw/Roll, Relative/Absolute selector and Clear, plus the toolbar anchor.</span></figcaption>
</figure>


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

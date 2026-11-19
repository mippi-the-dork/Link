# Absolute Mode — Design

**Link v1.0.0 · User guide**

## Purpose

**Absolute** matches selected Source coordinates on each Target when the Source changes. Disabled axes remain independent.

## Example: preserve height

<figure class="doc-media" data-kind="gif" data-media-id="absolute-horizontal">
  <img src="../../assets/media-placeholder.png" alt="Absolute X/Y linking while retaining a different Target height" loading="lazy">
  <figcaption>Absolute mode matches enabled Source coordinates while leaving disabled Target axes independent.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record two perspectives with visibly different heights, show Settings X/Y on Z off in Absolute, then move Source horizontally and show Target matching X/Y while keeping its own Z.</span></figcaption>
</figure>


With X and Y enabled and Z disabled, moving the Source makes each Target match Source X/Y while retaining its own Z. This is useful when comparing stacked spaces without losing horizontal alignment.

## How to use it

1. Assign a Perspective Source and one or more Targets.
2. Choose **Absolute** under Settings.
3. Enable the desired X/Y/Z and, for Perspective Targets, Pitch/Yaw/Roll axes.
4. Turn on Link and **move the Source**. Only selected axes of valid Targets match Source values.

## Mode changes do not immediately align

Switching from Relative to Absolute resets the movement baseline; it does **not** jump a Target immediately. A newly enabled axis also waits for subsequent Source movement. To align now, press **Sync**.

## Orthographic Targets

Top, Front and Side Targets can match selected position coordinates, but never receive Pitch/Yaw/Roll or orthographic zoom changes.

[Compare Relative](../relative/DESIGN.md) · [One-Shot Sync](../sync/DESIGN.md).

---

For engine implementation, [read Technical](TECHNICAL.md).

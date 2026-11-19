# Relative Mode — Design

**Link v1.0.0 · User guide**

## What Relative means

**Relative is the default Link mode.** A Target follows the Source's **change in camera position and rotation**, applied only to enabled axes. The Target preserves its existing offset rather than snapping to the Source's exact transform.

## Example: preserve distance

| Camera | Before Source moves | After Source X moves +100 |
| --- | ---: | ---: |
| Source X | 0 | 100 |
| Target X | 500 | 600 |
| Offset | 500 | 500 |

The Target moves the same **100 world units**, retaining its existing 500-unit difference along X.

## Example: preserve viewing direction

Suppose Source Yaw is 0° and Target Yaw is 90°. If Yaw synchronization is enabled and the Source rotates by +30°, the Target's Yaw becomes 120°. If Yaw is **disabled**, Target rotation remains unchanged.

## Typical workflow

1. Choose one Perspective Source and at least one Target.
2. Navigate each Target to the independent view you want.
3. Open Settings and select **Relative**.
4. Enable the position and, if desired for Perspective Targets, rotation axes you wish to follow.
5. Turn on Link and navigate the Source.

## A crucial distinction

Relative position follows the Source's **world-axis translation deltas**. Turning the Source does **not** orbit Targets around it. Position and rotation use separate enabled-axis calculations.

## Re-enabling without a jump

If you disable Link, move the Source elsewhere, and re-enable Link, the Target does **not** catch up on missed travel. A fresh movement baseline is established. Use **Sync** when immediate alignment is desired.

[Compare Absolute mode](../absolute/DESIGN.md).

---

For native integration, state and eligibility details, [read Technical](TECHNICAL.md).

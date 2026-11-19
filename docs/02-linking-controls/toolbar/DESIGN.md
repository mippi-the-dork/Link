# Toolbar Controls — Design

**Link v1.0.0 · User guide**

## Where the controls live

Link is integrated into the **Level Editor viewport toolbar**, beside Unreal Engine's standard camera controls. Each Level Editor viewport has access to the same setup and role controls.

| Control | What it does |
| --- | --- |
| **Sync** | Immediately copy enabled Source transform axes to valid Targets |
| **Link** | Turn continuous viewport following on or off |
| **Upper role selector** | Assign this viewport as the one Source |
| **Lower role selector** | Add/remove this viewport as a Target |
| **Settings** | Choose position/rotation axes, Relative/Absolute mode, or Clear |

The role selectors refer to **the viewport whose toolbar you click**, not whichever viewport was last clicked or selected elsewhere.

## Reading the controls

Click **Link** once to start following, again to suspend it. Turning it off preserves your selected Source and Targets. The Source/Target role selectors are independent from the global Link state.

**Sync** is always a one-shot absolute copy, even when Link is disabled and the continuous mode is Relative, provided the Source is valid and at least one Target exists.

## When Sync appears unavailable

A valid Perspective Source and Target must be assigned; the Source cannot be camera-locked, and PIE must not be active. Sync is gated by those conditions. For actual movement, at least one relevant axis must also be selected.

## Example

Put Perspective 1 beside a Top viewport. Mark Perspective 1 Source and Top Target. Click Sync to align position, then enable Link so the Top view follows later Source movement.

[Source and Target rules](../roles/DESIGN.md) · [Settings and axes](../axes/DESIGN.md).

---

For native integration, state and eligibility details, [read Technical](TECHNICAL.md).

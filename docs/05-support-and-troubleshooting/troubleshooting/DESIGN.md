# Troubleshooting — Design

**Link v1.0.0 · User guide**

## Link controls are missing

Open **Edit → Plugins** and confirm Link is enabled. Restart Unreal if the plugin was just enabled. Use a normal **Level Editor viewport**; Asset Editor preview viewports are outside Link's registration scope.

## Sync button is disabled

Check that a Perspective Source and at least one Target are assigned, PIE isn't active, and Source is not Actor- or cinematic-camera-locked. Target camera locks are checked separately during Sync.

## Link is enabled but Targets don't move

Check that the Source is **Perspective**, each intended viewport is assigned Target, the Source actually moves on at least one enabled axis, and neither Source nor Target is locked. Verify PIE is off. If you have only rotation axes enabled, a Source movement that is translation-only won't trigger rotation changes.

## Rotation doesn't follow

Enable Pitch/Yaw/Roll as appropriate in Settings. Rotation applies only to **Perspective Targets**. Orthographic Targets always keep their fixed orientation.

## Top/Front zoom is unchanged

This is by design. Link moves orthographic camera position but does not synchronize orthographic zoom.

## Source moved while Link was disabled; Target stayed behind

Expected behavior. Link resets its movement sample on re-enable and intentionally discards the travel made while off. Press **Sync** if you want enabled axes copied now.

## Switching to Absolute didn't align

Expected behavior. A mode change doesn't call Sync. Move the Source after the new baseline is captured, or press Sync.

## An axis checkbox has no immediate effect

Also expected. Settings define subsequent linked updates; press Sync for a one-shot copy.

## A camera-locked viewport stays put

Link respects native Actor and cinematic locks. Unlock that viewport using Unreal's standard controls if you want it moved.

## The setup disappears after restarting Unreal

Source, Targets, mode, axes and enabled state are session-only; no persistent Link configuration is currently supported.

## Reporting a defect

Include exact Link/UE versions, which viewports were Source/Targets, their projection types, Relative/Absolute mode, enabled axis masks, PIE and camera lock state, a short recording of the full layout, reproduction steps and relevant Output Log lines.

---

For implementation, [read Technical](TECHNICAL.md).

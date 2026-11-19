# Camera Locks and Play In Editor — Design

**Link v1.0.0 · User guide**

## Native camera locks come first

Link respects Viewports locked to Actors or cinematic cameras. A **locked Target** is skipped without preventing other Targets from following. A **locked Source** suspends continuous following; Sync is unavailable until the Source is usable again.

## Play In Editor

While PIE is active, Link suspends editor camera synchronization. It does not move gameplay cameras or interfere with PIE viewport navigation. When returning to the editor, Link starts with a fresh Source movement baseline rather than playing back all travel made during PIE.

## Scenario: cinematic plus orthographic reference

Keep one target locked to a cinematic camera and another Top target unlocked. With Link enabled, moving the Perspective Source skips the cinematic Target but can still move Top. Unlock the cinematic viewport if you want it to respond.

## If every Target stops moving

Verify Link is enabled, the Source is a valid Perspective viewport, the Source isn't locked, PIE is off, at least one axis is enabled and Source actually moved on an enabled axis.

## Does Sync ignore camera locks?

No. Sync is disabled for a locked Source and individually skips locked Targets. An available Sync button does not guarantee every selected Target is currently movable.

---

For native implementation, [read Technical](TECHNICAL.md).

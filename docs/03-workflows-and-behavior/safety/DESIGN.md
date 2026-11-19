# Camera Locks and Play In Editor — Design

**Link v1.0.0 · User guide**

## Native camera locks come first

<figure class="doc-media" data-kind="image" data-media-id="camera-locks">
  <img src="../../assets/media-placeholder.png" alt="Locked viewport excluded from Link Target synchronization" loading="lazy">
  <figcaption>A camera-locked Target is skipped while other unlocked Targets can continue receiving Source movement.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture one Perspective Source, one Target visibly locked to an Actor/cinematic camera, and a second unlocked Target, with Lock indicators and role selectors clearly identifiable.</span></figcaption>
</figure>


Link respects Viewports locked to Actors or cinematic cameras. A **locked Target** is skipped without preventing other Targets from following. A **locked Source** suspends continuous following; Sync is unavailable until the Source is usable again.

## Play In Editor

<figure class="doc-media" data-kind="gif" data-media-id="pie-suspend">
  <img src="../../assets/media-placeholder.png" alt="Link pausing synchronization during Play In Editor" loading="lazy">
  <figcaption>Link suspends when PIE begins and resumes future editor navigation from a fresh Source sample.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record a viewport layout with Link enabled, enter PIE, move the game camera without the editor Target being driven, then exit PIE and demonstrate normal Source following resuming.</span></figcaption>
</figure>


While PIE is active, Link suspends editor camera synchronization. It does not move gameplay cameras or interfere with PIE viewport navigation. When returning to the editor, Link starts with a fresh Source movement baseline rather than playing back all travel made during PIE.

## Scenario: cinematic plus orthographic reference

Keep one target locked to a cinematic camera and another Top target unlocked. With Link enabled, moving the Perspective Source skips the cinematic Target but can still move Top. Unlock the cinematic viewport if you want it to respond.

## If every Target stops moving

Verify Link is enabled, the Source is a valid Perspective viewport, the Source isn't locked, PIE is off, at least one axis is enabled and Source actually moved on an enabled axis.

## Does Sync ignore camera locks?

No. Sync is disabled for a locked Source and individually skips locked Targets. An available Sync button does not guarantee every selected Target is currently movable.

---

For native implementation, [read Technical](TECHNICAL.md).

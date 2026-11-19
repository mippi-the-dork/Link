# Frequently Asked Questions — Design

**Link v1.0.0 · User guide**

## General

**Does Link move Camera Actors or level content?** No. Link moves editor viewport cameras, not Actors, Components, gameplay cameras or level geometry.

**Can one Source drive several Targets?** Yes. A single Source can have multiple Perspective and Orthographic Targets.

**Do I need to enable Link to press Sync?** No. Sync performs a one-shot copy whenever a valid Perspective Source and at least one Target exist, PIE is inactive and the Source isn't camera-locked.

## Modes and controls

<figure class="doc-media" data-kind="image" data-media-id="faq-mode-compare">
  <img src="../../assets/media-placeholder.png" alt="Comparison of Relative and Absolute linking with the same initial camera offsets" loading="lazy">
  <figcaption>Relative preserves a Target's offset while Absolute aligns the enabled coordinates after Source movement.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Use a side-by-side comparison with matching two-viewport framing and clear Relative/Absolute labels, showing Target offset preserved versus enabled-axis position matched.</span></figcaption>
</figure>


**What's the difference between Relative and Absolute?** Relative adds Source movement deltas and preserves each Target's existing offset. Absolute matches the enabled Source coordinates when the Source changes.

**Why didn't choosing Absolute move anything?** A mode change doesn't reposition Targets. Move the Source again, or press Sync for immediate alignment.

**Why didn't checking an axis move my Target?** An axis checkbox only changes what participates in subsequent linking or Sync.

**Will Source rotation move Targets around it?** Relative rotation changes the Target's selected rotation values; it does not orbit Target positions.

## Viewport types

**Can a Top or Front viewport be Source?** The role selector may show Source, but actual synchronization requires a Perspective Source.

**Why doesn't Top rotate or zoom?** Orthographic Targets follow enabled position axes only. Their fixed orientation and zoom remain independent.

**Why won't my Target move while locked to an Actor?** Link respects Unreal's Actor and cinematic camera locks.

## Persistence

**Does Link save roles and axes?** No. They're editor-session state. Use Clear to reset manually, or restart Unreal to return to defaults.

**Why does re-enabling Link not catch up?** Missed Source movement is intentionally ignored. Sync if you want to align immediately.

**Does Link work in PIE?** No. Linking is suspended during play and never drives gameplay cameras.

---

For implementation, [read Technical](TECHNICAL.md).

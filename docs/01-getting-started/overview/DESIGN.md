# What Is Link? — Design

**Link v1.0.0 · User guide**

## The problem Link solves

<figure class="doc-media" data-kind="image" data-media-id="overview-viewports">
  <img src="../../assets/media-placeholder.png" alt="Link Source and Target viewports within Unreal Engine's Level Editor" loading="lazy">
  <figcaption>Link coordinates several Level Editor cameras while retaining an independent viewing position in each Target.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture Unreal's four-viewport layout with one Perspective Source and two visibly different Targets, with the Link toolbar controls and Source/Target selections legible.</span></figcaption>
</figure>


Unreal Engine can display several Level Editor viewports at once, but normally each camera must be navigated independently. Link allows you to designate a **Source** and make other **Target** viewports follow its movement. This is useful when you want a main working camera alongside a side perspective, a Top view, or a Front reference.

## What you can do

- **Relative linking** moves Targets by the Source's movement delta, preserving each Target's existing offset.
- **Absolute linking** copies the Source's enabled camera axes when the Source moves.
- **Sync** copies enabled axes immediately, independently of the continuous mode.
- **Axis filtering** lets you synchronize X, Y, Z, Pitch, Yaw, and Roll independently.
- **Multiple Targets** follow one Perspective Source; orthographic Targets retain their projection orientation.

## Example: inspect the same room from three views

Switch the Level Editor to a three- or four-viewport layout. Choose a Perspective viewport as Source, select a Top and a Side viewport as Targets, then enable Link in Relative mode. Navigating the Perspective camera carries the reference views through the level without converting them to Perspective or forcing them into the same viewing angle.

## What Link is not

Link does **not** create Camera Actors, move level Actors, author sequences, replicate gameplay cameras, save anything into the level, or modify engine source. It is an **Editor-only** navigation utility with session-only configuration.

## Before you begin

You need at least two valid Level Editor viewports for a meaningful Source→Target setup. A Perspective Source is required for actual synchronization, although the toolbar can appear on an orthographic viewport as well.

[Start with installation](../installation/DESIGN.md) or [configure your first layout](../first-session/DESIGN.md).

---

For native integration, state and eligibility details, [read Technical](TECHNICAL.md).

# Orthographic Viewports — Design

**Link v1.0.0 · User guide**

## Position following with a fixed view

Top, Front and Side **orthographic viewports can be Targets** for a Perspective Source. Their enabled world-position axes follow Source motion while projection orientation and zoom remain independent.

## Set it up

<figure class="doc-media" data-kind="gif" data-media-id="orthographic-follow">
  <img src="../../assets/media-placeholder.png" alt="Perspective Source guiding Top and Front orthographic views" loading="lazy">
  <figcaption>Orthographic Targets follow selected Source position changes without rotating their fixed projections or changing zoom.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record Perspective, Top and Front viewports together: show Source/Target roles, enable Relative X/Y/Z, navigate Perspective through the level and keep both orthographic camera views visible.</span></figcaption>
</figure>


1. Select a Perspective + Top + Front Level Editor layout.
2. Mark Perspective **Source** and both orthographic views **Targets**.
3. Use Relative mode with X/Y/Z enabled and turn on Link.
4. Navigate the Source; the Top and Front views follow your position without becoming Perspective.

## Capability table

| Operation | Orthographic Target |
| --- | --- |
| X/Y/Z linking or Sync | Yes |
| Pitch/Yaw/Roll | No |
| Changing projection orientation | No |
| Orthographic zoom sync | No |

## Can an orthographic view be Source?

You can see the Source selector on an orthographic viewport, but actual continuous linking and Sync **require a Perspective Source**. Reassign Source to a Perspective viewport if the system isn't responding.

## Practical benefit

Keep Top zoomed tightly for placement and Front framed more widely while moving through a large level from Perspective. Their separate orthographic zoom settings remain untouched.

---

For engine implementation, [read Technical](TECHNICAL.md).

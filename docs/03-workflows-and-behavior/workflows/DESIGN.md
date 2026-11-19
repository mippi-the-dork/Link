# Multi-Viewport Workflows — Design

**Link v1.0.0 · User guide**

## Maintain a side view while navigating

<figure class="doc-media" data-kind="gif" data-media-id="side-view-workflow">
  <img src="../../assets/media-placeholder.png" alt="Multi-viewport navigation retaining a useful independent side perspective" loading="lazy">
  <figcaption>A second Perspective camera can follow the Source's translation while maintaining its original viewing direction.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record a main Perspective viewport moving down a corridor while a side-angle Perspective Target follows at its initial offset; keep the complete layout visible.</span></figcaption>
</figure>


Open two Perspective Level Editor viewports. Choose the first as Source and second as Target. Leave mode Relative, enable X/Y/Z and disable rotation. As you move the Source, the Target moves by the same world-axis translation while retaining its original camera-facing direction.

## Keep Top and Front reference views with you

<figure class="doc-media" data-kind="image" data-media-id="reference-workflow">
  <img src="../../assets/media-placeholder.png" alt="Multi-viewport layout with a Perspective Source and two orthographic references" loading="lazy">
  <figcaption>Perspective, Top and Front viewports can track the same working area while showing different projections.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture a three- or four-viewport layout with Source and both Targets labeled, framing the same area in Perspective, Top and Front with visibly different zoom/orientation.</span></figcaption>
</figure>


Use a Perspective Source and Top/Front Targets. With X/Y/Z in Relative mode, the orthographic cameras move alongside the Source without adopting its orientation or changing zoom. This is particularly useful for blockout and spatial alignment.

## Match horizontal position but preserve vertical height

In Absolute mode, enable X/Y and disable Z. After the Source moves, Target X/Y match while each Target Z remains independent.

## Follow a turning camera with a different yaw

Enable X/Y/Z and Yaw in Relative mode for two Perspective viewports. As Source moves and rotates, Target follows its position and yaw deltas, retaining independent yaw offset. Source yaw changes don't orbit the Target's camera location.

## Sync first, then choose working offsets

Assign Source/Targets, press Sync, reposition each Target into the angle or offset you want, then use Relative Link. Each Target follows independently from its new camera position.

## Multiple Target limitations

There is one shared set of axis settings and one shared mode; you cannot set a different position/rotation mask for each Target in v1.0.0. Disable Link to pause movement, or use Settings → Clear to remove all roles.

---

For native implementation, [read Technical](TECHNICAL.md).

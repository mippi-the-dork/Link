# First Viewport Setup — Design

**Link v1.0.0 · User guide**

## Prepare your layout

Open a Level Editor layout with **two or more viewports**, ideally one Perspective plus another Perspective or Top/Front view. Link controls appear independently in each viewport's toolbar.

## Your first linked navigation

<figure class="doc-media" data-kind="gif" data-media-id="first-session">
  <img src="../../assets/media-placeholder.png" alt="Two Level Editor viewports linking for the first time" loading="lazy">
  <figcaption>Assigning a Perspective Source and a Target allows the Target camera to follow later movement without losing its chosen offset.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record a 10–15-second Unreal Editor sequence with two viewports: select upper Source radio in Perspective 1, lower Target radio in Perspective 2, enable Link, then move Source and show Target following.</span></figcaption>
</figure>


1. In the first **Perspective** viewport, click the **upper** role selector to make it the **Source**.
2. In a second viewport, click the **lower** role selector to make it a **Target**.
3. Leave the default **Relative** mode. X, Y and Z are enabled by default, while Pitch, Yaw and Roll start disabled.
4. Click **Link** to enable continuous following.
5. Move the Source camera. The Target follows the Source's position changes while retaining its own offset and viewing direction.
6. Turn Link off. Move the Source again and observe that the Target remains where it was.
7. Turn Link back on and move the Source. Only subsequent Source movement is propagated.

## Try a one-shot alignment

With the Source and Target configured, click **Sync**. Enabled Source axes are copied to the Target immediately. Sync does not enable Link or change Relative/Absolute mode.

## A helpful setup variation

After syncing, position the Target at the viewing angle or camera offset you want, then continue in Relative mode. The Target now follows the Source while retaining that distinct viewpoint.

## If it does not work

Check all five conditions: a valid **Perspective Source**, one or more **Targets**, **Link enabled** for continuous following, at least one relevant axis enabled, and no active PIE or Source/Target camera lock. If the Target is orthographic, only position can be synchronized.

[Learn every toolbar control](../../02-linking-controls/toolbar/DESIGN.md).

---

For native integration, state and eligibility details, [read Technical](TECHNICAL.md).

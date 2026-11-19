# Installation and Compatibility — Design

**Link v1.0.0 · User guide**

## Supported release

Link **v1.0.0** is an **Unreal Engine 5.8 / Win64 Editor plugin**. The README lists a packaged release targeted at **UE 5.8.3**; the plugin descriptor declares engine version **5.8.0**. For precompiled builds, choose the ZIP explicitly labeled for your exact engine and platform.

## Install from a packaged release

<figure class="doc-media" data-kind="image" data-media-id="installation-files">
  <img src="../../assets/media-placeholder.png" alt="Correctly installed Link plugin folder inside a project" loading="lazy">
  <figcaption>The Link plugin belongs directly under a project's Plugins directory, with its descriptor and resources available to Unreal Editor.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Show File Explorer at YourProject/Plugins/Link with Link.uplugin, Source, Resources and Config visible and the address bar showing the correct folder nesting.</span></figcaption>
</figure>


1. Close Unreal Editor.
2. Open [Link Releases](https://github.com/mippi-the-dork/Link/releases) and obtain a compatible precompiled package if one is published.
3. Extract it so `Link.uplugin` is directly inside `YourProject/Plugins/Link/`.
4. Reopen your project; use **Edit → Plugins → Link** to enable the plugin if necessary.
5. Restart if prompted, then open a standard Level Editor viewport.

Do not treat GitHub's automatic **Source code** download as a precompiled binary plugin.

## Fab installation

If Link is listed in Fab for your compatible Unreal version, install it from the Epic Games Launcher, enable it under Edit → Plugins, and restart if requested. **Do not assume Fab availability merely because the README explains this installation route.**

## Install from source

For development, place a Git checkout at `YourProject/Plugins/Link` and build your project's **Editor** target using the UE 5.8 C++ toolchain. Generate IDE project files if needed.

```text
YourProject/
  Plugins/
    Link/
      Link.uplugin
      Source/
      Resources/
      Config/
  YourProject.uproject
```

## Confirm installation

You should see Link's Sync, Link, role selector and Settings controls near the Level Editor viewport camera toolbar. They do not appear in ordinary asset editor previews.

## Updating

Close Unreal Editor, replace the entire precompiled plugin folder with the matching version, and relaunch. A Git checkout can be updated with Git and rebuilt. Link's session configuration is not persisted, so it does not require saved Link settings migration.

---

For native integration, state and eligibility details, [read Technical](TECHNICAL.md).

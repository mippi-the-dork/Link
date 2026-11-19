# What Is Link? — Technical

**Link v1.0.0 · Source-reviewed implementation**

## Plugin scope

`Link.uplugin` declares version **1.0.0**, one `Editor` module, loading phase `PostEngineInit`, Win64 platform allowlist, `CanContainContent: false`, and `EngineVersion: 5.8.0`. The README describes the v1.0.0 release build for UE 5.8.3. This is not a runtime game module.

## Editor integration

`FLinkModule` registers a native ToolMenus dynamic entry in `LevelEditor.ViewportToolbar`. It adds Sync, Link, two per-viewport role selectors, and Settings in the toolbar's Right section before the camera controls. It registers a Slate style containing vector icons from `Resources/`.

## State ownership

Current Source identity, Target viewport keys, Link enabled flag, six axis booleans, Absolute/Relative mode and the previous Source camera sample are held in **module instance memory**. The Source and Target identities are `FName` viewport configuration keys, not Camera Actor object pointers. No Link asset, Actor or Component stores them.

## How synchronization differs from Actor editing

`FLevelEditorViewportClient::GetViewLocation/GetViewRotation` provide editor-camera values. `SetViewLocation/SetViewRotation` update valid Target viewport clients, followed by `Invalidate()`. The plugin does not use `AActor::SetActorTransform` or edit level geometry.

## Supported boundaries

The active Source must be Perspective; target camera locks and PIE are respected. Rotation writes only occur on Perspective Targets. Orthographic zoom is not read or propagated. The module is not an API for arbitrary Asset Editor viewports.

---

For step-by-step use, [read Design](DESIGN.md).

## Source references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

**Verification:** Implementation inspected in v1.0.0 source; comprehensive live-editor behavior testing remains separately tracked in the verification matrix.

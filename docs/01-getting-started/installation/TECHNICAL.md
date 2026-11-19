# Installation and Compatibility — Technical

**Link v1.0.0 · Source-reviewed implementation**

## Descriptor and build dependencies

`Link.uplugin` declares `Editor` module `Link` loaded at `PostEngineInit` for `Win64`. `Source/Link/Link.Build.cs` uses public Core/CoreUObject/Engine and private UnrealEd, LevelEditor, Slate, SlateCore, ToolMenus and Projects. These dependencies mean the module must be compiled for an Editor target, not a packaged game.

## Runtime registration

`FLinkModule::StartupModule` calls `RegisterStyle`, registers a `FTSTicker` delegate, and requests a ToolMenus startup callback. `RegisterMenus` adds the dynamic toolbar entry when ToolMenus is ready, so the control is created with the correct viewport context.

## Installed-file checks

The installed plugin should include `Link.uplugin`, `Source/`, `Resources/`, and its associated configuration as supplied by the distribution. The SVG files under `Resources/` are required for the native toolbar icons. An incorrect folder nesting level can prevent the Editor plugin manager from finding the descriptor.

## Engine target caveat

The descriptor's `EngineVersion: 5.8.0` is a compatibility declaration, whereas the README's v1.0.0 release example explicitly targets a UE 5.8.3 build. Neither proves forward compatibility with unrelated versions or non-Windows platforms.

## Safe unloading

`ShutdownModule` removes the ticker delegate, unregisters ToolMenus startup callbacks/owner and Slate style, then releases in-memory viewport state. It does not delete saved level content or undo Actor changes, because Link does not author those changes.

---

For step-by-step use, [read Design](DESIGN.md).

## Source references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

**Verification:** Implementation inspected in v1.0.0 source; comprehensive live-editor behavior testing remains separately tracked in the verification matrix.

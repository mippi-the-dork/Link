# Troubleshooting — Technical

**Link v1.0.0 · Source reference**

## Debug order

1. Verify Editor plugin load and ToolMenus registration.
2. Confirm Source/Target `GetConfigKey()` identities and live `RegisteredViewports` entries.
3. Check `bLinkEnabled` for continuous following, or `CanSyncTargets` for one-shot alignment.
4. Test the Source Perspective/lock/PIE conditions.
5. Inspect enabled position/rotation masks and whether Source changed in the matching dimension.
6. Inspect each Target's validity, lock and projection type.

## Important state transitions

`ToggleEnabled`, `ToggleSource`, `SetAbsoluteMode` and PIE/invalid Source early-outs reset Source sample. The first eligible tick only records baseline. A lack of movement on that tick is intentional.

## Native commands

`SyncTargets` is gated by `CanSyncTargets`, while the Link toggle can be enabled even without a Source or Target. This means a visually enabled Link button can still produce no movement until the role prerequisites are met.

## Toolbar visibility

`RegisterMenus` extends `LevelEditor.ViewportToolbar` and `BuildToolbarEntries` requires a valid viewport context. Separate this from Source/Target eligibility: missing controls are different from visible but inactive controls.

## Target skip behavior

A locked or expired Target doesn't abort updates for other targets. Capture the whole layout to diagnose whether the failure is one Target or the entire Source pipeline.

## Evidence

Record exact steps and source type, expected versus observed Target camera state, active PIE status, camera locks and Output Log; do not infer a passing case from successful compilation alone.

---

For practical workflows, [read Design](DESIGN.md).

## Code references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

**QA scope:** Source inspected, not a live editor test report.

# Multi-Viewport Workflows — Technical

**Link v1.0.0 · Source reference**

## Independent Target update loop

`TargetViewportKeys` stores zero or more Target configuration keys. Each valid unlocked Target's own location/rotation is read before the module applies Source deltas or absolute values.

## Shared global behavior

`bAbsoluteMode` and the six axis flags are global module fields. Different Targets can preserve different offsets in Relative mode, but do not have individual mode or axis masks.

## Delta model

Relative translation applies world-axis deltas rather than a rig hierarchy transform. Rotating the Source affects enabled Perspective rotation fields; it does not rotate Target location around Source.

## No layout management

The plugin doesn't instantiate or reposition viewport panels. Native Level Editor viewport layouts provide the cameras and Link registers them through the toolbar context.

## Performance characteristics

`TickLink` exits if no Source movement is detected. Changed Target clients are invalidated; world Actors aren't scanned. Viewport redraw cost may rise with more active Targets and complex scenes, but the plugin does not spawn render cameras or game Actors.

---

For user steps, [read Design](DESIGN.md).

## Code references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

Source-review only; editor regression results belong in the Verification Matrix.

# Position and Rotation Axes — Technical

**Link v1.0.0 · Source reference**

## Native settings menu

`PopulateSettingsMenu` creates Location, Rotation and Mode ToolMenus sections. Position and rotation use segmented native `SCheckBox` controls in a `SUniformGridPanel`, styled with `FAppStyle`; the UI row has a 244-pixel width.

## Fields

Each checkbox directly changes `bLinkPositionX/Y/Z` or `bLinkPitch/Yaw/Roll`, with no immediate camera write. A native `SSegmentedControl` selects Relative/Absolute and invokes `SetAbsoluteMode`.

## Clear path

`ClearLinkSetup` sets Link disabled, removes Source/Target keys, restores default masks and Relative mode, and resets Source sample. It doesn't modify viewport camera transforms.

## Tick checks

Position and rotation blocks independently require Source changes of that type and at least one enabled axis. Perspective Target is required for rotation.

## Persistence

All axis and mode fields are ordinary `FLinkModule` fields and are not serialized into the project or level.

---

For use cases, [read Design](DESIGN.md).

## Source references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

Source-reviewed v1.0.0 behavior; actual editor QA remains tracked separately.

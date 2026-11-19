# Link Roadmap

**Current documented version: v1.0.0 — Released.**  
**Future milestones: No version numbers, dates or features have been committed.**

## Shipped — v1.0.0

Link's initial release establishes coordinated Level Editor viewport navigation with a single Perspective Source and multiple Targets. It includes:

- Continuous **Relative** following that preserves Target position/rotation offsets on the enabled axes.
- Continuous **Absolute** matching of enabled Source position/rotation values when the Source changes.
- **One-Shot Sync**, which copies enabled axes immediately regardless of Relative/Absolute mode.
- Independent X/Y/Z and Pitch/Yaw/Roll settings; rotation affects Perspective Targets only.
- Orthographic position following without changing fixed projection orientation or zoom.
- Temporary Link enable/disable that preserves role/axis configuration and resets the movement baseline.
- Native toolbar integration, Actor/cinematic camera-lock awareness, and PIE suspension.

## Future — Under consideration, not scheduled

No future Link features have been approved or assigned a release milestone. Suggestions should be evaluated against a clear goal: **make multi-viewport editor navigation easier without adding complexity or altering runtime cameras**.

Potential requests can be discussed through [GitHub Issues](https://github.com/mippi-the-dork/Link/issues). A proposal is not a planned or shipped feature until it has been explicitly approved.

## Release documentation policy

When functionality ships, add a confirmed milestone and describe the changes actually implemented in that version. Preserve historical entries, but do not present unimplemented ideas as capabilities of the current plugin.

[Feature documentation](02-linking-controls/README.md) · [Support](05-support-and-troubleshooting/README.md)

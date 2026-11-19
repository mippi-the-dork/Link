# Link Documentation Media Inventory

This page tracks every planned documentation image and GIF. Link's existing hero image is already authentic; **28 additional figure slots** use the same neutral placeholder supplied for the Focus documentation until final captures or diagrams are available.

Each figure already has two distinct captions:

- **Reader caption:** Explains what the final visual demonstrates, appropriate for the published manual.
- **Capture instructions · Image/GIF:** Explains exactly what to capture or illustrate, including the controls, viewports, states and steps that must be visible.

The capture instructions are also stored in [media-slots.json](assets/media-slots.json) as `capture_instructions`.

## Captures to produce

| Page | Kind | Planned asset | Capture requirement |
| --- | --- | --- | --- |
| [01-getting-started/overview/DESIGN.md ](01-getting-started/overview/DESIGN.md) | IMAGE | `overview-viewports.png` | Capture Unreal's four-viewport layout with one Perspective Source and two visibly different Targets, with the Link toolbar controls and Source/Target selections legible. |
| [01-getting-started/overview/TECHNICAL.md ](01-getting-started/overview/TECHNICAL.md) | IMAGE | `overview-architecture.png` | Create a diagram showing FLinkModule, LevelEditor.ViewportToolbar, the Source/Target key registry, the core ticker and Target viewport clients; distinguish UI and camera data. |
| [01-getting-started/installation/DESIGN.md ](01-getting-started/installation/DESIGN.md) | IMAGE | `installation-files.png` | Show File Explorer at YourProject/Plugins/Link with Link.uplugin, Source, Resources and Config visible and the address bar showing the correct folder nesting. |
| [01-getting-started/first-session/DESIGN.md ](01-getting-started/first-session/DESIGN.md) | GIF | `first-session.gif` | Record a 10–15-second Unreal Editor sequence with two viewports: select upper Source radio in Perspective 1, lower Target radio in Perspective 2, enable Link, then move Source and show Target following. |
| [02-linking-controls/toolbar/DESIGN.md ](02-linking-controls/toolbar/DESIGN.md) | IMAGE | `toolbar-controls.png` | Capture a close-up of one viewport toolbar and label Sync, Link, upper Source selector, lower Target selector and Settings. Include enough of the viewport to establish location. |
| [02-linking-controls/toolbar/TECHNICAL.md ](02-linking-controls/toolbar/TECHNICAL.md) | IMAGE | `toolbar-registration.png` | Create an annotated call-flow diagram from RegisterMenus to BuildToolbarEntries, showing LevelEditor.ViewportToolbar context, Sync/Link actions, role widget and Settings submenu. |
| [02-linking-controls/roles/DESIGN.md ](02-linking-controls/roles/DESIGN.md) | IMAGE | `role-source.png` | Capture at least two viewports with the upper Source selector visibly active on Perspective 1 and inactive on Perspective 2. Keep both viewport labels visible. |
| [02-linking-controls/roles/DESIGN.md ](02-linking-controls/roles/DESIGN.md) | GIF | `role-targets.gif` | Record a sequence assigning two Targets to one Source, selecting a new Source, and showing how the old Source selection is replaced; keep each viewport toolbar visible. |
| [02-linking-controls/relative/DESIGN.md ](02-linking-controls/relative/DESIGN.md) | GIF | `relative-follow.gif` | Record two perspective viewports initially at distinct positions, enable Relative with X/Y/Z on, move the Source forward and sideways, and show Target following without snapping to Source. |
| [02-linking-controls/relative/TECHNICAL.md ](02-linking-controls/relative/TECHNICAL.md) | IMAGE | `relative-delta-diagram.png` | Create a concise diagram using example Source X 0→100 and Target X 500→600, plus separate yaw 0→30 and 90→120; mark these as independent axis calculations. |
| [02-linking-controls/absolute/DESIGN.md ](02-linking-controls/absolute/DESIGN.md) | GIF | `absolute-horizontal.gif` | Record two perspectives with visibly different heights, show Settings X/Y on Z off in Absolute, then move Source horizontally and show Target matching X/Y while keeping its own Z. |
| [02-linking-controls/absolute/TECHNICAL.md ](02-linking-controls/absolute/TECHNICAL.md) | IMAGE | `absolute-trigger.png` | Diagram Source sample → position delta/rotation delta decisions → axis-mask checks → assign current Source values to Target; note mode/axis changes alone don't trigger immediate writes. |
| [02-linking-controls/sync/DESIGN.md ](02-linking-controls/sync/DESIGN.md) | GIF | `sync-offset.gif` | Record two perspective viewports: click Sync to align enabled axes, manually move Target to a distinct view, then enable Relative Link and move Source to demonstrate the retained offset. |
| [02-linking-controls/axes/DESIGN.md ](02-linking-controls/axes/DESIGN.md) | IMAGE | `axis-settings.png` | Capture the full expanded Settings menu with Location X/Y/Z, Rotation Pitch/Yaw/Roll, Relative/Absolute selector and Clear, plus the toolbar anchor. |
| [02-linking-controls/axes/TECHNICAL.md ](02-linking-controls/axes/TECHNICAL.md) | IMAGE | `axis-ui-diagram.png` | Create a compact diagram of Location, Rotation and Mode sections with their native Slate types and field names; emphasize that checking a box updates state rather than moving the camera immediately. |
| [02-linking-controls/orthographic/DESIGN.md ](02-linking-controls/orthographic/DESIGN.md) | GIF | `orthographic-follow.gif` | Record Perspective, Top and Front viewports together: show Source/Target roles, enable Relative X/Y/Z, navigate Perspective through the level and keep both orthographic camera views visible. |
| [02-linking-controls/orthographic/TECHNICAL.md ](02-linking-controls/orthographic/TECHNICAL.md) | IMAGE | `orthographic-boundary.png` | Draw a capability diagram with Perspective Source → Perspective Target (position and rotation) and → Top/Front Target (position only), marking zoom and orthographic rotation as unsupported. |
| [03-workflows-and-behavior/workflows/DESIGN.md ](03-workflows-and-behavior/workflows/DESIGN.md) | GIF | `side-view-workflow.gif` | Record a main Perspective viewport moving down a corridor while a side-angle Perspective Target follows at its initial offset; keep the complete layout visible. |
| [03-workflows-and-behavior/workflows/DESIGN.md ](03-workflows-and-behavior/workflows/DESIGN.md) | IMAGE | `reference-workflow.png` | Capture a three- or four-viewport layout with Source and both Targets labeled, framing the same area in Perspective, Top and Front with visibly different zoom/orientation. |
| [03-workflows-and-behavior/safety/DESIGN.md ](03-workflows-and-behavior/safety/DESIGN.md) | IMAGE | `camera-locks.png` | Capture one Perspective Source, one Target visibly locked to an Actor/cinematic camera, and a second unlocked Target, with Lock indicators and role selectors clearly identifiable. |
| [03-workflows-and-behavior/safety/DESIGN.md ](03-workflows-and-behavior/safety/DESIGN.md) | GIF | `pie-suspend.gif` | Record a viewport layout with Link enabled, enter PIE, move the game camera without the editor Target being driven, then exit PIE and demonstrate normal Source following resuming. |
| [03-workflows-and-behavior/session/DESIGN.md ](03-workflows-and-behavior/session/DESIGN.md) | GIF | `session-clear.gif` | Record a linked setup with altered axes and Absolute mode, open Settings → Clear, then show Link off, no Source/Targets, X/Y/Z on, rotation off and Relative selected, with cameras stationary. |
| [04-technical-implementation/architecture/TECHNICAL.md ](04-technical-implementation/architecture/TECHNICAL.md) | IMAGE | `native-toolmenus.png` | Create a labeled diagram showing RegisterMenus → BuildToolbarEntries → SLevelViewport/GetConfigKey → RegisteredViewports; include Source and Target key collections and native Slate icon/style registration. |
| [04-technical-implementation/algorithm/TECHNICAL.md ](04-technical-implementation/algorithm/TECHNICAL.md) | IMAGE | `sync-loop.png` | Draw a flowchart: Tick → Link enabled, PIE, Perspective Source, locks → baseline present → delta checks → per-Target eligibility → Relative/Absolute axis writes → Invalidate. |
| [05-support-and-troubleshooting/faq/DESIGN.md ](05-support-and-troubleshooting/faq/DESIGN.md) | IMAGE | `faq-mode-compare.png` | Use a side-by-side comparison with matching two-viewport framing and clear Relative/Absolute labels, showing Target offset preserved versus enabled-axis position matched. |
| [05-support-and-troubleshooting/troubleshooting/DESIGN.md ](05-support-and-troubleshooting/troubleshooting/DESIGN.md) | IMAGE | `troubleshoot-sync.png` | Capture a viewport toolbar where Sync is disabled, alongside a clearly identifiable missing Source or Target role; provide an adjacent corrected state showing Sync available. |
| [05-support-and-troubleshooting/updating/DESIGN.md ](05-support-and-troubleshooting/updating/DESIGN.md) | IMAGE | `update-folder.png` | Show the YourProject/Plugins/Link directory with Link.uplugin and Resources/Source contents visible; include a release version label if supplied by the package. |
| [ROADMAP.md ](ROADMAP.md) | IMAGE | `roadmap-v1.png` | Create a clean static feature overview with a central Perspective Source linked to several Targets and labels for Relative, Absolute, Sync, axis filters and orthographic support. Do not depict unapproved future features. |

## Replacement procedure

1. Capture authentic **UE 5.8 Link** interface media with text legible and the relevant Source/Target states visible.
2. Save the file under `docs/assets/media/` with the planned ID and extension shown above. Static screenshots/diagrams should be PNG, animated demonstrations GIF.
3. Find the corresponding HTML figure in the Markdown page by its stable `data-media-id` attribute and change the `<img src>` from `assets/media-placeholder.png` to the new asset's relative path.
4. Retain the reader caption if it remains accurate; update alt text and the instruction registry if framing/behavior changes.
5. Verify the result in desktop and narrow mobile layouts before marking the media complete.

Do not replace the existing repository hero with a placeholder. Do not claim a GIF is recorded merely because its placeholder is visible.

[Documentation home](README.md) · [Verification Matrix](VERIFICATION_MATRIX.md)

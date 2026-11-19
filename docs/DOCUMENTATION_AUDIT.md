# Link Documentation Audit

**Baseline:** Link v1.0.0, Unreal Engine 5.8.x (README package example UE 5.8.3).  
**Basis:** Link README, descriptor, Build.cs, `LinkModule.h` and `LinkModule.cpp`.  
**Status:** Documentation/source review only; no Unreal Editor or browser integration test passes are claimed.

## Coverage

The documentation site has **five sections, 18 paired feature topics (36 Design/Technical pages)**, a source-reviewed v1.0 roadmap and a media production inventory. It preserves the layout and reader interaction patterns established in the Focus documentation while using Link's native toolbar terminology.

### Confirmed source behaviors

- One Perspective Source can drive multiple Targets; role assignments are exclusive.
- Continuous Relative adds selected Source translation/rotation deltas; it does not orbit Target positions.
- Continuous Absolute assigns selected Source values only when the relevant Source transform type changes.
- One-Shot Sync copies selected values immediately regardless of Link mode/enabled state.
- Position axes apply to valid orthographic Targets; rotation/zoom do not.
- PIE and Source camera locks suspend operation; locked Targets are skipped individually.
- Link configuration is ephemeral module session state, not level or Actor serialization.
- Link is an Editor-only Win64 plugin and does not change packaged gameplay behavior.

## Improvements already delivered

| Area | Change | Verification limit |
| --- | --- | --- |
| Information architecture | Five substantive category overviews and 18 distinct Design/Technical topic pairs | Link integrity checked statically |
| Navigation | Reused Focus's searchable responsive docs design with Link-specific branding | Live browser QA still needed |
| Media | Existing Link hero and authentic SVG icon plus 28 typed placeholder figures | Replacement screenshots and GIFs not captured |
| Captions | Reader-facing captions and separate, precise Image/GIF capture instructions | Screenshot fidelity still to be verified |
| Roadmap | Shipped v1.0.0 described; no invented future milestone | Author must approve future scope |
| Verification | Editor and website test matrix with explicit Not run status | Tests not executed |

## Remaining gaps

| Priority | Gap | Action |
| --- | --- | --- |
| P1 | Authentic controls and workflow media | Capture the 19 planned still images and 9 GIFs from a running UE 5.8 editor (or create grounded technical diagrams) |
| P1 | Actual behavior verification | Execute the [Verification Matrix](VERIFICATION_MATRIX.md), especially camera lock/PIE/baseline reset and orthographic behavior |
| P1 | Live Pages validation | Confirm website publishing, full-text search, mobile width, anchors and responsive caption rendering |
| P2 | Version-pinned source links | Change current main-branch source links to exact release commit/tag when maintaining multiple versions |
| P2 | Accessibility and performance | Test keyboard-only search, contrast, reduced-motion preference and large multi-viewport layouts |
| P2 | Future release maintenance | Keep README, descriptor, docs and roadmap synchronized when the plugin changes |

## Editorial policy

Design should answer what a user clicks, why they would use it, expected outcome and what to check when it doesn't work. Technical should explain precise Unreal module integration, state and transformation semantics. Source review is not a substitute for observed editor behavior. Unapproved future feature ideas do not belong among shipped capabilities.

[Verification Matrix](VERIFICATION_MATRIX.md) · [Media Inventory](MEDIA_INVENTORY.md) · [Roadmap](ROADMAP.md)

# Relative Mode — Technical

**Link v1.0.0 · Source-reviewed implementation**

## Per-tick delta model

<figure class="doc-media" data-kind="image" data-media-id="relative-delta-diagram">
  <img src="../../assets/media-placeholder.png" alt="Diagram of Source camera delta applied to a Target camera" loading="lazy">
  <figcaption>Relative movement adds Source translation and rotation deltas independently to each valid Target's enabled axes.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Create a concise diagram using example Source X 0→100 and Target X 500→600, plus separate yaw 0→30 and 90→120; mark these as independent axis calculations.</span></figcaption>
</figure>


In `FLinkModule::TickLink`, the Source camera is sampled after eligibility checks:

```cpp
const FVector LocationDelta = CurrentSourceLocation - PreviousSourceLocation;
const FRotator RotationDelta = (CurrentSourceRotation - PreviousSourceRotation).GetNormalized();
```

When `bAbsoluteMode=false`, each enabled position component is updated as `TargetLocation.Axis += LocationDelta.Axis`; each enabled Perspective rotation component similarly adds `RotationDelta`.

## Rotational normalization

`FRotator::GetNormalized()` is applied to the difference and to the resulting Target rotation. This handles ordinary wrapping across angle boundaries better than raw unbounded angle accumulation. The code does **not** use a full quaternion-relative pose transform.

## No implicit orbital translation

The code separately handles Source translation and Source rotation. Rotating Source while standing still can change Target rotation (for enabled axes), but does not rotate the Target's position offset around the Source. This distinction matters for users expecting a rigid camera rig.

## Baseline and disabled time

The first eligible Source tick establishes `PreviousSourceLocation` / `PreviousSourceRotation`, sets `bHasPreviousSourceSample=true`, and returns. `ToggleEnabled`, `ToggleSource` and `SetAbsoluteMode` reset the sample, preventing catch-up deltas after discontinuities.

## Target scope

The loop skips Source, missing or camera-locked Targets; position updates apply to Perspective or Orthographic Targets, while rotation only applies to Perspective Targets. `SetViewLocation`, `SetViewRotation`, and `Invalidate` update native editor cameras.

---

For step-by-step use, [read Design](DESIGN.md).

## Source references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

**Verification:** Implementation inspected in v1.0.0 source; comprehensive live-editor behavior testing remains separately tracked in the verification matrix.

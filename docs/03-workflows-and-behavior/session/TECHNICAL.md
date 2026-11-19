# Session State and Reset — Technical

**Link v1.0.0 · Source reference**

## Stored fields

`FLinkModule` owns Source/Target `FName` keys, `TSet<FName>` Targets, Link/axis/mode booleans and previous Source camera samples. There is no config, `UObject` asset or `UPROPERTY` serialization for Link session settings.

## ResetSourceSample

Sets `bHasPreviousSourceSample=false`, previous location to zero and previous rotation to zero. The next valid tick records Source transform without moving Targets.

## Toggle and Clear

`ToggleEnabled` flips the enabled flag and resets sample. `SetAbsoluteMode` resets sample only if mode changed. `ClearLinkSetup` resets all fields to their original defaults and clears Source/Targets, without camera setter calls.

## Viewport reconstruction

`PruneInvalidViewports` removes expired weak pointers. The Source key may remain so the same viewport configuration can reconnect after a layout rebuild; when missing, the source sample is reset.

## Lifecycle

The module's shutdown removes its ticker/toolbar/style registrations and clears role state. Relaunching recreates module fields at defaults.

---

For user steps, [read Design](DESIGN.md).

## Code references

- [LinkModule.cpp](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Private/LinkModule.cpp)
- [LinkModule.h](https://github.com/mippi-the-dork/Link/blob/main/Source/Link/Public/LinkModule.h)

Source-review only; editor regression results belong in the Verification Matrix.

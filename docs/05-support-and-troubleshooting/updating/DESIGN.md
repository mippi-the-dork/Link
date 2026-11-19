# Updating Link and Support — Design

**Link v1.0.0 · User guide**

## Installing a newer precompiled release

1. Close Unreal Editor.
2. Download a release package matching your engine and platform from [Link Releases](https://github.com/mippi-the-dork/Link/releases), if one is published.
3. Replace the **entire** `YourProject/Plugins/Link` folder, rather than overlaying old files.
4. Start the project, enable the plugin if needed and confirm the viewport controls appear.

A Git source checkout should be updated with Git and rebuilt against the chosen Unreal Editor version.

## Compatibility

| Item | v1.0.0 |
| --- | --- |
| Unreal Engine | 5.8.x target; precompiled README release example 5.8.3 |
| Platform | Win64 |
| Plugin module | Editor |
| Runtime components | None |
| Runtime camera/Actor changes | None |

The plugin descriptor identifies `EngineVersion: 5.8.0`; this does not automatically guarantee every later engine release. Follow the actual package label for binary compatibility.

## Requesting support

Use [Link GitHub Issues](https://github.com/mippi-the-dork/Link/issues). Provide Link/UE versions, installation type, full Source/Target viewport layout, mode and axes, camera-lock/PIE state, steps, expected result and actual observation. An 8–12-second recording that clearly shows all viewports is often particularly useful.

## Feature requests

Describe the editor navigation problem you need solved, not only a proposed button. Link's current product scope is coordinated Level Editor camera navigation. The [Roadmap](../../ROADMAP.md) does not commit unapproved future features.

## License

Link is distributed under MIT; see the repository [LICENSE](https://github.com/mippi-the-dork/Link/blob/main/LICENSE).

---

For implementation, [read Technical](TECHNICAL.md).

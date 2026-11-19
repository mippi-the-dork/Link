# Link

### Multiple viewports. One effortless workflow.

**Keep your Unreal Engine Level Editor cameras moving together—without forcing them to look at the same thing.**

When you're building a level, a second viewport is only useful if it stays focused on the area you're working in. But every time you navigate your main viewport, the others get left behind. You stop, reposition them, and repeat.

**Link removes that busywork.** Pick one Perspective viewport as your **Source**, choose as many **Targets** as you need, and navigate your level while the others follow. Each Target can retain its own position, viewing angle, and purpose.

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.8.x-black?logo=unrealengine)
![Platform](https://img.shields.io/badge/Platform-Win64-blue)
![Plugin](https://img.shields.io/badge/Plugin-Editor%20Only-green)
![Version](https://img.shields.io/badge/Version-1.0.0-blue)
[![License: MIT](https://img.shields.io/badge/License-MIT-green)](LICENSE)

![Link synchronizing multiple Level Editor viewports](Doc/Images/Link-Hero.png)

> ### 📖 Full Link Documentation
>
> **[Explore the Link Documentation Website](https://mippi-the-dork.github.io/Link/)**
>
> Learn how to install Link, synchronize your first viewports, choose between Relative and Absolute, configure individual axes, and troubleshoot common editor scenarios.
>
> **[Getting Started](https://mippi-the-dork.github.io/Link/?page=01-getting-started%2Ffirst-session%2FDESIGN.md)** · **[All Linking Controls](https://mippi-the-dork.github.io/Link/?page=02-linking-controls%2FREADME.md)** · **[Practical Workflows](https://mippi-the-dork.github.io/Link/?page=03-workflows-and-behavior%2Fworkflows%2FDESIGN.md)**

---

## Why use Link?

**Spend more time working in your level and less time managing your viewport layout.**

Whether you're building an environment, blocking out gameplay, comparing composition, or inspecting an area from multiple angles, Link lets a single viewport guide the rest.

| Your workflow | What Link makes easier |
| --- | --- |
| **Level design and blockout** | Move through a large level while your Top or Side reference views follow the working area. |
| **Environment art** | Maintain a close-up camera and a wider overview without repeatedly repositioning either one. |
| **Composition and spatial checks** | Watch the same area from multiple independent perspectives as you navigate. |
| **Precise alignment** | Match only the coordinates you need while preserving other Target camera values. |

### One Source. Multiple useful views.

Assign one Perspective viewport as Source, then add Perspective or Orthographic Targets. The Targets move when your Source moves, and each can serve a different purpose.

A Top viewport stays Top. A side view can keep its viewing angle. You remain in control of how much synchronization you want.

**[See Source and Target setup](https://mippi-the-dork.github.io/Link/?page=02-linking-controls%2Froles%2FDESIGN.md)**

## Three ways to keep cameras coordinated

| | **Relative** | **Absolute** | **Sync** |
| --- | --- | --- | --- |
| **What happens** | Targets follow Source movement and keep their offsets. | Targets match the Source's enabled coordinates when it moves. | Copy the enabled Source coordinates to Targets immediately. |
| **Best for** | Keeping several different views moving through the same level. | Maintaining alignment on chosen axes, such as X/Y without matching height. | Quickly realigning cameras whenever you need to. |
| **Continuous?** | Yes, while Link is enabled. | Yes, while Link is enabled. | No—one click, one alignment. |

Relative is the default, making it easy to preserve the independent viewing positions you already established. Absolute gives you tighter control over matching coordinates. Sync works independently of which continuous mode you've selected—even while Link is off.

**[Relative Mode](https://mippi-the-dork.github.io/Link/?page=02-linking-controls%2Frelative%2FDESIGN.md)** · **[Absolute Mode](https://mippi-the-dork.github.io/Link/?page=02-linking-controls%2Fabsolute%2FDESIGN.md)** · **[One-Shot Sync](https://mippi-the-dork.github.io/Link/?page=02-linking-controls%2Fsync%2FDESIGN.md)**

## Fine-tune the connection

Link is built around quick controls inside Unreal's **native Level Editor viewport toolbar**.

- **Independent position axes:** Follow or match X, Y, and Z separately.
- **Independent rotation axes:** Choose Pitch, Yaw, and Roll for Perspective Targets.
- **Orthographic-friendly:** Top, Front, and Side Targets follow position without losing their fixed projection orientation.
- **Pause without losing your setup:** Turn Link off, then on again without forcing Targets to catch up on missed movement.
- **Respect native editor behavior:** Actor/cinematic camera locks are honored, and synchronization pauses during Play In Editor.
- **Editor-only:** No Camera Actors, level-content changes, or packaged-game systems required.

**[Explore Axis Settings](https://mippi-the-dork.github.io/Link/?page=02-linking-controls%2Faxes%2FDESIGN.md)** · **[Orthographic Workflows](https://mippi-the-dork.github.io/Link/?page=02-linking-controls%2Forthographic%2FDESIGN.md)** · **[Camera Locks and PIE](https://mippi-the-dork.github.io/Link/?page=03-workflows-and-behavior%2Fsafety%2FDESIGN.md)**

## Get started

Link v1.0.0 is an **Unreal Engine 5.8 / Win64 Editor plugin**. The documented precompiled v1.0.0 release targets **UE 5.8.3**; use a package explicitly compatible with your engine version.

1. Obtain a compatible package from [GitHub Releases](https://github.com/mippi-the-dork/Link/releases), or clone the source into your project's `Plugins/Link` directory.
2. Enable Link in **Edit → Plugins** and restart the Editor if prompted.
3. Open two or more **Level Editor** viewports, choose a Perspective Source and at least one Target, and enable Link.

For complete instructions—including packaged installation, source builds, updates, and compatibility—use the **[Installation Guide](https://mippi-the-dork.github.io/Link/?page=01-getting-started%2Finstallation%2FDESIGN.md)**.

## What Link doesn't do

Link is deliberately focused on **Level Editor viewport navigation**, not gameplay camera control. It does not:

- Create or move Camera Actors, level Actors, or geometry.
- Affect gameplay cameras, replication, runtime systems, or packaged games.
- Synchronize orthographic rotation or zoom.
- Override viewports locked to Actors or cinematic cameras.
- Control specialized Asset Editor viewports or replace Unreal's viewport layout system.
- Save Source/Target assignments, axis settings, or Link mode between Editor sessions.
- Automatically snap Targets just because you switch linking modes or enable a new axis.

A **Perspective Source** is required for synchronization. Link is suspended during Play In Editor. These behaviors are intentional, and you can read the details in the **[FAQ](https://mippi-the-dork.github.io/Link/?page=05-support-and-troubleshooting%2Ffaq%2FDESIGN.md)**.

## Documentation and help

The README is an introduction. The full **[Link Documentation Website](https://mippi-the-dork.github.io/Link/)** is the source for detailed usage, expected behavior, examples, and technical implementation.

| Looking for… | Read |
| --- | --- |
| First-time setup | [Your First Viewport Setup](https://mippi-the-dork.github.io/Link/?page=01-getting-started%2Ffirst-session%2FDESIGN.md) |
| Every toolbar control | [Toolbar Controls](https://mippi-the-dork.github.io/Link/?page=02-linking-controls%2Ftoolbar%2FDESIGN.md) |
| Recommended workflows | [Multi-Viewport Workflows](https://mippi-the-dork.github.io/Link/?page=03-workflows-and-behavior%2Fworkflows%2FDESIGN.md) |
| Technical implementation | [Camera Synchronization Algorithm](https://mippi-the-dork.github.io/Link/?page=04-technical-implementation%2Falgorithm%2FTECHNICAL.md) |
| Something not working | [Troubleshooting](https://mippi-the-dork.github.io/Link/?page=05-support-and-troubleshooting%2Ftroubleshooting%2FDESIGN.md) |
| Releases and future direction | [Roadmap](https://mippi-the-dork.github.io/Link/?page=ROADMAP.md) |

## Issues, feature requests, and contributions

**Found a bug?** [Open a GitHub Issue](https://github.com/mippi-the-dork/Link/issues). Include your Link and Unreal Engine versions, Source/Target viewport setup, mode and enabled axes, reproduction steps, and a screenshot or short recording when possible.

**Have an idea?** [Suggest a feature](https://github.com/mippi-the-dork/Link/issues) and describe the multi-viewport workflow you want to improve. The goal is to keep Link straightforward and useful rather than add controls simply because they are possible.

**Want to contribute?** Pull requests are welcome. For significant changes, open an Issue first so the behavior and scope can be discussed before implementation.

## License

Link is distributed under the **MIT License**. See [LICENSE](LICENSE) for details.

---

**Link** is created by [Mippi the Dork](https://github.com/mippi-the-dork).

*Multiple viewports are more useful when they can work together without becoming the same view.*

**[Read the Documentation](https://mippi-the-dork.github.io/Link/)** · **[Browse Releases](https://github.com/mippi-the-dork/Link/releases)** · **[Report an Issue](https://github.com/mippi-the-dork/Link/issues)**

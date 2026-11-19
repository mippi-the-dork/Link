# Module and Toolbar Architecture — Design

**Link v1.0.0 · User guide**

## What the plugin adds

Link integrates with the native Level Editor viewport toolbar rather than creating a floating utility window, placing an Actor or modifying the level. Each viewport toolbar offers Sync, Link, Source/Target selectors and Settings.

## What those parts are responsible for

| Part | User-visible purpose |
| --- | --- |
| Toolbar | Fast access to Link commands near native camera tools |
| Source/Target roles | Decide which viewport drives and which follow |
| Settings | Choose position/rotation axes, continuous mode and Clear |
| Editor camera updates | Move only the participating viewport cameras |
| Session state | Remember roles and settings until the Editor closes |

## Why it's editor-only

Link controls how people **navigate the editor**, not how a player sees the game. It doesn't introduce new gameplay camera components, Camera Actors, tick Actors or project assets.

## What technical users should know

Viewport identities are the Level Editor's configuration keys. A reopened or rebuilt viewport can reconnect under the same key; if no valid Perspective Source exists, updates pause. The source is a camera viewport, not a gameplay Actor.

## Scope of documentation

The Design guide covers the editor-facing architecture. The Technical guide details ToolMenus, Slate style, native client pointers and lifetime management.

---

For native implementation, [read Technical](TECHNICAL.md).

# 02. Linking Controls

Every viewport toolbar action and how it changes camera synchronization.

## Controls in context

Each Level Editor viewport exposes Link controls near its camera toolbar. The upper role control assigns a Source, the lower assigns a Target; the Link button toggles continuous following, Sync aligns once, and Settings chooses axes and mode. The Source must be Perspective when driving synchronization.

## Topics

| Topic | What you'll learn | Design | Technical |
| --- | --- | --- | --- |
| **Toolbar Controls** | Identify Sync, Link, role selectors and Settings. | [Design](toolbar/DESIGN.md) | [Technical](toolbar/TECHNICAL.md) |
| **Source and Targets** | Assign and change the Source and any number of Targets. | [Design](roles/DESIGN.md) | [Technical](roles/TECHNICAL.md) |
| **Relative Mode** | Follow Source movement while preserving Target offsets. | [Design](relative/DESIGN.md) | [Technical](relative/TECHNICAL.md) |
| **Absolute Mode** | Match selected Source position and rotation values continuously. | [Design](absolute/DESIGN.md) | [Technical](absolute/TECHNICAL.md) |
| **One-Shot Sync** | Immediately align enabled axes without changing Link mode. | [Design](sync/DESIGN.md) | [Technical](sync/TECHNICAL.md) |
| **Position and Rotation Axes** | Choose which axes synchronize and learn the defaults. | [Design](axes/DESIGN.md) | [Technical](axes/TECHNICAL.md) |
| **Orthographic Viewports** | Use top/front/side Targets without rotating their projections. | [Design](orthographic/DESIGN.md) | [Technical](orthographic/TECHNICAL.md) |

## Key distinction

Changing a setting changes which axes participate next; use **Sync** when you want an immediate position/rotation copy.

[Return to documentation home](../README.md) · [Read the Roadmap](../ROADMAP.md)

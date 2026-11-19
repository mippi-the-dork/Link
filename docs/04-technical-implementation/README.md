# 04. Technical Implementation

Source-grounded editor integration and camera update semantics.

## How this plugin works

The Editor-only module registers native viewport toolbar actions through ToolMenus, stores viewport roles using configuration keys, then samples camera movement using a lightweight core ticker. It does not create runtime Actors or modify Unreal Engine source.

## Topics

| Topic | What you'll learn | Design | Technical |
| --- | --- | --- | --- |
| **Module and Toolbar Architecture** | Module lifecycle, native ToolMenus, viewport identities and Slate style. | [Design](architecture/DESIGN.md) | [Technical](architecture/TECHNICAL.md) |
| **Camera Synchronization Algorithm** | Per-tick sampling, relative deltas, absolute copies and reentrancy safeguards. | [Design](algorithm/DESIGN.md) | [Technical](algorithm/TECHNICAL.md) |

## Key distinction

Source-level method names are implementation details; ordinary users only need the Design steps.

[Return to documentation home](../README.md) · [Read the Roadmap](../ROADMAP.md)

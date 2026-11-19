# 03. Workflows and Editor Behavior

Practical layouts and special editor conditions that affect synchronization.

## A coordinated layout

A Perspective Source can drive multiple perspective or orthographic Targets. Relative follows movement without collapsing your distinct viewpoints. Link respects native camera locks and suspends automatically during PIE; changing mode or enabling Link does not itself jump the cameras.

## Topics

| Topic | What you'll learn | Design | Technical |
| --- | --- | --- | --- |
| **Multi-Viewport Workflows** | Side reference, matching horizontal position, and multi-Target navigation. | [Design](workflows/DESIGN.md) | [Technical](workflows/TECHNICAL.md) |
| **Camera Locks and Play In Editor** | Understand locked viewport protection and automatic PIE suspension. | [Design](safety/DESIGN.md) | [Technical](safety/TECHNICAL.md) |
| **Session State and Reset** | Temporarily disable Link, change modes or Clear the configuration. | [Design](session/DESIGN.md) | [Technical](session/TECHNICAL.md) |

## Key distinction

A Target can be valid and still remain stationary when the Source is not moving on an enabled axis.

[Return to documentation home](../README.md) · [Read the Roadmap](../ROADMAP.md)

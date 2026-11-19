# Source and Targets — Design

**Link v1.0.0 · User guide**

## Choose a Source

Every viewport toolbar contains a small **upper role selector**. Click it in the viewport you want to drive the others. Only one Source can exist. Choosing a new Source automatically replaces the previous one.

For actual synchronization, that Source must be a **Perspective Level Editor viewport**.

## Choose Targets

Click the **lower role selector** in each desired Target viewport. You can assign multiple Targets, including Perspective, Top, Front or Side. Click a Target selector again to remove that viewport from the target set.

| Selection | Behavior |
| --- | --- |
| Select a second Source | Replaces the old Source |
| Select Source on a Target | Removes it from Targets, makes it Source |
| Select Target on the Source | Clears the Source role, makes it a Target |
| Select the same Source again | Clears the Source assignment |
| Select a selected Target again | Removes that Target |
| Assign several Targets | All valid Targets can follow the single Source |

## Example layout

```text
Perspective 1     Source
Perspective 2     Target
Top               Target
Front             Target
```

With Link enabled, moving Perspective 1 can change the position of all three Targets. Perspective 2 can also follow rotation when those axes are enabled; Top and Front retain orthographic orientation.

## Common mistake

The Source and Target selectors are attached to **each viewport**, not to a global list. Selecting Target on the current Source intentionally removes the Source role, so continuous linking stops until you assign another Source.

[Orthographic limitations](../orthographic/DESIGN.md).

---

For native integration, state and eligibility details, [read Technical](TECHNICAL.md).

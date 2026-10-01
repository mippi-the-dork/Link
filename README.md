# Link

**Synchronize Unreal Engine Level Editor viewport cameras without forcing them into the same view.**

Link adds camera synchronization controls directly to the Level Editor viewport toolbar, letting one viewport drive one or more target viewports while preserving the layout and perspective you already established.

Use Relative mode to preserve viewport offsets, Absolute mode to continuously match selected transform axes, or Sync for a one-shot camera alignment.

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.8.3-black?logo=unrealengine)
![Platform](https://img.shields.io/badge/Platform-Windows%2064--bit-blue)
![Type](https://img.shields.io/badge/Plugin-Editor%20Only-green)
![Version](https://img.shields.io/badge/Version-0.3.2-blue)
![License](https://img.shields.io/badge/License-MIT-green)

---

## What is Link?

Working with multiple Level Editor viewports can be useful for level design, composition, alignment, spatial comparison, and inspecting a scene from several angles at once.

The problem is that Unreal normally treats each viewport camera independently.

Link lets you define:

- One **Source** viewport
- One or more **Target** viewports
- Which position axes should synchronize
- Which rotation axes should synchronize
- Whether targets should follow relatively or match absolutely

The result is a lightweight multi-viewport navigation tool that stays inside Unreal's normal Level Editor workflow.

Link does not create cameras, modify level Actors, or add runtime systems.

---

# Features

### Source and Target Viewports

Assign one Level Editor viewport as the **Source** and any number of other viewports as **Targets**.

Only one Source can be active at a time.

A viewport cannot be both Source and Target simultaneously.

### Relative Linking

Relative mode applies the Source camera's movement and rotation deltas to each Target.

This preserves the Target viewport's existing offset and viewing angle.

For example:

```text
Source X = 0
Target X = 500

Source moves +100

Target X = 600
```

The Target follows the Source without snapping onto it.

### Absolute Linking

Absolute mode continuously matches the Source camera on the enabled axes whenever the Source moves.

For example:

```text
Source X = 100
Target X = 500

Absolute X enabled

Target X = 100
```

Axes that are disabled remain independent.

### One-Shot Sync

The **Sync** button performs an immediate absolute alignment without changing the current Link mode.

Sync respects the enabled Position and Rotation axes.

This makes it useful for temporarily matching cameras before continuing in Relative mode.

### Independent Position Axes

Choose which location axes Link controls:

```text
X
Y
Z
```

Each axis can be enabled or disabled independently.

### Independent Rotation Axes

Choose which rotation axes Link controls:

```text
Pitch
Yaw
Roll
```

Each axis can be enabled or disabled independently.

Rotation synchronization applies to Perspective Target viewports.

### Multiple Targets

A single Source viewport can drive several Targets simultaneously.

This makes it possible to maintain several coordinated views of the same area while navigating through a level.

### Instant Enable / Disable

The **Link** toolbar control enables or suspends synchronization without clearing the current setup.

When Link is disabled:

- Source assignment is preserved
- Target assignments are preserved
- Axis settings are preserved
- Relative / Absolute mode is preserved

Re-enabling Link establishes a fresh movement baseline rather than applying a catch-up jump.

### Native Level Editor UI

Link integrates directly into Unreal Engine's Level Editor viewport toolbar.

The toolbar provides:

- Sync
- Link enable / disable
- Source / Target selection
- Settings

The Settings menu provides:

- Location axis controls
- Rotation axis controls
- Relative / Absolute mode
- Clear

### Editor Only

Link is an editor workflow utility.

It does not add runtime components, gameplay systems, camera Actors, or packaged-game dependencies.

---

# Using Link

## Set Up Multiple Viewports

Switch the Level Editor to a layout containing two or more viewports.

For the most complete Link behavior, use a **Perspective** viewport as the Source.

Targets may be Perspective or Orthographic.

---

## Choose the Source

In the viewport that should drive the others, enable the upper Source selector beside the Link controls.

Only one viewport can be the Source.

Choosing a new Source automatically replaces the previous Source.

---

## Choose Targets

In each viewport that should follow the Source, enable the lower Target selector.

Any number of Level Editor viewports may be Targets.

Selecting Target on the current Source clears its Source role first.

---

## Enable Link

Click **Link** in the viewport toolbar.

When Link is enabled, Source camera changes are propagated to the configured Targets according to the current settings.

Click Link again to suspend synchronization without clearing the setup.

---

# Sync

The **Sync** button immediately aligns all configured Targets to the Source on the currently enabled axes.

For example, with:

```text
Location
[X] X
[X] Y
[ ] Z

Rotation
[ ] Pitch
[X] Yaw
[ ] Roll
```

Sync copies:

```text
X
Y
Yaw
```

from the Source to each valid Target.

The Target's Z, Pitch, and Roll remain unchanged.

Sync is always a one-shot absolute operation.

It works independently of whether the current continuous Link mode is Relative or Absolute.

---

# Relative Mode

Relative mode preserves the Target viewport's existing relationship to the Source.

If two viewports begin several hundred units apart, they remain several hundred units apart as the Source moves.

The same principle applies to enabled Perspective rotation axes.

This is useful when you want several coordinated views of an area without collapsing them into the exact same camera.

### Example

```text
Initial

Source:
X = 0
Y = 0
Z = 500

Target:
X = 1000
Y = 0
Z = 500
```

Move the Source:

```text
Source:
X = 200
Y = 300
Z = 500
```

With X and Y enabled, the Target becomes:

```text
Target:
X = 1200
Y = 300
Z = 500
```

The original X offset is preserved.

---

# Absolute Mode

Absolute mode continuously copies the Source transform values on the enabled axes whenever the Source camera changes.

For example:

```text
Location
[X] X
[X] Y
[ ] Z
```

means:

```text
Target.X = Source.X
Target.Y = Source.Y
```

while Target Z remains independent.

The same rule applies to enabled Pitch, Yaw, and Roll on Perspective Targets.

Switching between Relative and Absolute mode does not immediately move the cameras.

A camera change from the Source, or pressing Sync, causes the next synchronization.

---

# Settings

Open **Settings** beside the Link viewport controls.

## Location

Choose which position axes Link is allowed to control:

| Setting | Description |
| --- | --- |
| **X** | Synchronize world X position. |
| **Y** | Synchronize world Y position. |
| **Z** | Synchronize world Z position. |

The default configuration enables all three Position axes.

## Rotation

Choose which rotation axes Link is allowed to control:

| Setting | Description |
| --- | --- |
| **Pitch** | Synchronize Perspective camera Pitch. |
| **Yaw** | Synchronize Perspective camera Yaw. |
| **Roll** | Synchronize Perspective camera Roll. |

Rotation synchronization is disabled by default.

## Mode

Choose how continuous Link movement is applied:

| Mode | Description |
| --- | --- |
| **Relative** | Apply Source camera deltas while preserving Target offsets. |
| **Absolute** | Continuously match enabled Source transform axes. |

Relative is the default mode.

## Clear

**Clear** resets the current Link setup.

It:

- Disables Link
- Clears the Source
- Clears all Targets
- Enables X, Y, and Z Position linking
- Disables Pitch, Yaw, and Roll linking
- Returns the mode to Relative

Clear does **not** move any viewport cameras.

---

# Orthographic Viewports

Orthographic viewports can be used as Targets for position synchronization.

Link intentionally preserves the projection orientation of Orthographic Targets.

Rotation controls therefore apply only to Perspective Targets.

Orthographic zoom is not synchronized.

The Source viewport must currently be Perspective for Link propagation and Sync.

---

# Locked and Cinematic Viewports

Link avoids modifying Level Editor viewports that are locked to an Actor or cinematic camera.

If the Source is locked, continuous Link propagation is suspended until it becomes usable again.

Locked Targets are skipped rather than forcibly moved.

This keeps Link from fighting Unreal's existing camera-lock workflows.

---

# Play In Editor

Link is an editor navigation tool.

Synchronization is suspended while Play In Editor is active.

Link does not drive runtime gameplay viewports or modify gameplay cameras.

---

# Example Workflows

## Maintain a Side View While Navigating

1. Open two Perspective Level Editor viewports.
2. Position the second viewport at a useful side angle.
3. Make the first viewport the Source.
4. Make the second viewport a Target.
5. Enable X, Y, and Z.
6. Enable Yaw only if you want both cameras to turn together.
7. Use Relative mode.
8. Enable Link.

The second viewport follows the Source while keeping its established offset.

## Align Two Viewports, Then Separate Them

1. Assign Source and Target viewports.
2. Enable the transform axes you want to match.
3. Press **Sync**.
4. Move the Target to a new offset.
5. Select Relative mode.
6. Enable Link.

You now have two coordinated views that started from the same alignment.

## Keep an Orthographic Reference View Moving With You

1. Use a Perspective viewport as the Source.
2. Set an Orthographic viewport as a Target.
3. Enable the desired Location axes.
4. Leave Rotation disabled.
5. Enable Link.

The Orthographic viewport follows the Source position while retaining its fixed projection direction.

---

# Installation

Link can be installed through **Fab**, from a **precompiled GitHub Release**, or directly from the **GitHub source**.

For most users, the Fab or GitHub Release installation is recommended once those packages are available.

---

## Fab / Epic Games Launcher

> **Availability:** Use this installation method once Link is available through Fab.

1. Add **Link** to your library on Fab.
2. Open the **Epic Games Launcher**.
3. Navigate to your Unreal Engine Library.
4. Locate Link in your Fab / Vault library.
5. Install Link to the supported Unreal Engine version.
6. Launch your Unreal Engine project.
7. Open **Edit > Plugins**.
8. Search for **Link**.
9. Enable the plugin if it is not already enabled.
10. Restart Unreal Editor if prompted.

Once enabled, Link controls appear in the Level Editor viewport toolbar.

---

## GitHub Release

This is the easiest manual installation method once precompiled release packages are available.

### 1. Download Link

Open the repository's **Releases** page:

https://github.com/mippi-the-dork/Link/releases

Download the latest release package matching your Unreal Engine version and platform.

### 2. Close Unreal Editor

Close the project before installing the plugin.

### 3. Locate Your Project Plugins Folder

Your project should contain a `Plugins` directory beside the `.uproject` file:

```text
YourProject/
├── Config/
├── Content/
├── Plugins/
└── YourProject.uproject
```

If the `Plugins` directory does not exist, create it.

### 4. Extract Link

Extract the `Link` folder into:

```text
YourProject/Plugins/
```

The final structure should look similar to:

```text
YourProject/
├── Plugins/
│   └── Link/
│       ├── Doc/
│       ├── Resources/
│       ├── Source/
│       └── Link.uplugin
└── YourProject.uproject
```

### 5. Launch the Project

Open your Unreal Engine project.

If necessary, navigate to:

**Edit > Plugins**

Search for:

```text
Link
```

Enable the plugin and restart Unreal Editor if prompted.

---

## GitHub Source

Developers who want the latest source or want to modify Link can clone the repository directly.

### Requirements

Building Link from source requires a working Unreal Engine C++ development environment.

For Windows this generally means:

- Unreal Engine 5.8.x
- Visual Studio with the appropriate C++ workloads
- A project capable of compiling C++ plugins

### Clone the Repository

Close Unreal Editor and navigate to your project's `Plugins` directory.

```bash
cd YourProject/Plugins
git clone https://github.com/mippi-the-dork/Link.git
```

Your project should now contain:

```text
YourProject/Plugins/Link/
```

### Generate Project Files

If necessary:

1. Right-click your `.uproject`.
2. Select **Generate Visual Studio project files**.

Then open the generated solution and build your project's Editor target.

For example:

```text
YourProjectEditor
Win64
Development Editor
```

Launch the project after compilation completes.

---

# Updating Link

## GitHub Release Installation

When updating a manually installed release:

1. Close Unreal Editor.
2. Remove the existing `Plugins/Link` folder.
3. Extract the new Link release into the `Plugins` directory.
4. Reopen the project.

Replacing the complete plugin folder is recommended rather than copying individual files over an older version.

## Git Source Installation

If you cloned the repository using Git:

```bash
cd YourProject/Plugins/Link
git pull
```

Rebuild the project if the source has changed.

---

# Compatibility

The current Link development release targets:

| | |
| --- | --- |
| **Link Version** | 0.3.2 |
| **Unreal Engine** | 5.8.3 |
| **Platform** | Windows 64-bit |
| **Plugin Type** | Editor |
| **Runtime Dependency** | None |
| **Packaged Game Impact** | None |

Link is currently configured as a **Win64 editor plugin**.

The current development and validation target is Unreal Engine **5.8.3**.

Compatibility with additional Unreal Engine versions or platforms should not be assumed unless explicitly listed in a release.

---

# How Link Works

Link integrates into Unreal Engine's modern Level Editor viewport toolbar through ToolMenus.

Each supported Level Editor viewport is tracked by its viewport configuration identity.

When Link is enabled:

1. Link identifies the configured Source viewport.
2. Link samples the Source Perspective camera transform.
3. Source movement is compared against the previous sample.
4. In Relative mode, the resulting transform delta is applied to enabled Target axes.
5. In Absolute mode, enabled Target axes are assigned the corresponding Source values.
6. Perspective Target rotation is updated only for enabled Pitch, Yaw, and Roll axes.
7. Updated Targets are invalidated so their viewports redraw.

A guard prevents linked updates from feeding back into the Source operation.

Disabling Link resets the Source sampling baseline so re-enabling synchronization does not create a delayed catch-up movement.

The **Sync** command uses the same axis configuration, but performs an immediate absolute copy rather than continuous linking.

---

# What Link Does Not Do

Link is an **editor viewport navigation utility**, not a runtime camera system.

It does not:

- Create Camera Actors
- Move or edit Actors in the level
- Modify saved level geometry
- Control gameplay cameras
- Affect packaged builds
- Synchronize Orthographic projection orientation
- Synchronize Orthographic zoom
- Force camera-locked viewports away from their locked Actor
- Replace Unreal's viewport layout system
- Modify engine source

---

# Current Limitations

### Perspective Source Required

The active Source must be a Perspective Level Editor viewport.

Orthographic Source cameras are not currently used to drive Link.

### Orthographic Rotation

Orthographic Targets can follow enabled position axes, but their projection orientation is intentionally preserved.

Pitch, Yaw, and Roll synchronization applies only to Perspective Targets.

### Camera Locks

Viewports locked to Actors or cinematic cameras are skipped.

Link does not override Unreal's existing camera-lock behavior.

### Session State

The current Link configuration is editor-session state.

Source assignment, Target assignments, enabled axes, Link state, and Relative / Absolute mode are not currently documented as persistent project settings.

### Level Editor Only

Link currently targets Unreal Engine's Level Editor viewports.

Other asset-editor viewport types are outside the current scope.

---

# Troubleshooting

## Link Does Not Appear in the Viewport Toolbar

Check:

**Edit > Plugins**

Search for:

```text
Link
```

Confirm that the plugin is enabled.

Restart Unreal Editor if the plugin was just enabled.

---

## Sync Is Disabled

Sync requires:

- A valid Perspective Source
- At least one Target
- No active Play In Editor session
- A Source viewport that is not locked to an Actor or cinematic camera

Confirm those conditions before trying Sync again.

---

## A Target Is Not Moving

Check that:

- Link is enabled
- A Source is assigned
- The viewport is assigned as a Target
- At least one relevant Location or Rotation axis is enabled
- The Source is a Perspective viewport
- The Target is not camera-locked
- Play In Editor is not currently active

---

## Rotation Is Not Affecting an Orthographic Target

This is expected.

Link preserves Orthographic projection orientation.

Rotation linking applies only to Perspective Targets.

---

## Re-Enabling Link Does Not Catch Up to the Source

This is intentional.

Disabling Link suspends propagation.

When Link is enabled again, Link establishes a fresh Source baseline instead of applying camera movement that occurred while synchronization was disabled.

Use **Sync** if you want Targets to immediately align to the Source.

---

## Absolute Mode Did Not Move the Target Immediately

Changing Relative / Absolute mode does not move cameras by itself.

Absolute synchronization occurs when the Source camera changes.

Use **Sync** for an immediate one-shot alignment.

---

# Reporting Bugs

If you encounter a problem, please open an issue:

https://github.com/mippi-the-dork/Link/issues

When reporting a bug, include:

- Link version
- Unreal Engine version
- Windows version
- Viewport layout
- Which viewport was Source
- Which viewports were Targets
- Relative or Absolute mode
- Enabled Position and Rotation axes
- Steps needed to reproduce the issue
- Screenshots or video if the issue is visual

---

# Feature Requests

Feature requests are welcome through GitHub Issues:

https://github.com/mippi-the-dork/Link/issues

When suggesting a feature, explain the viewport workflow or problem the feature would improve.

This helps keep Link focused on practical Level Editor navigation rather than adding unrelated camera systems.

---

# Contributions

Pull requests are welcome.

For larger changes, consider opening an issue first so the proposed behavior can be discussed before implementation.

Please keep contributions focused on:

- Level Editor viewport workflow
- Native Unreal Editor UX
- Safe editor-only behavior
- Compatibility with Link's Source / Target model

---

# License

Link is released under the **MIT License**.

See the repository license for details.

---

# About

Link is a standalone Unreal Engine editor plugin created by **Mippi the Dork**.

It is designed to make multi-viewport Level Editor navigation faster and more deliberate while preserving Unreal Engine's existing viewport workflow.

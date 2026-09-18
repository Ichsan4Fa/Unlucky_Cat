# Unlucky Cat

A small 2D game-development project written in C++ for Windows. **Unlucky Cat** is a side-scrolling/endless-survival prototype in which the player controls an animated cat, moves and jumps across a grass platform, avoids falling objects, and tries to survive long enough to increase the score.

The repository is currently a Visual Studio/OpenGL game project rather than a cross-platform build. The active executable is implemented in `vs/Project/unluckyCat.cpp` and uses SDL2 for the window, input, timing, and audio integration, OpenGL for rendering, and bundled third-party headers/libraries under `deps/`.

## Features

- Main menu with Play and Exit buttons.
- Cat sprite-sheet animation for idle, walking, and hit states.
- Left/right movement, sprinting, jumping, and basic controller mappings.
- Falling hazards that are pooled, spawned at intervals, and recycled after their lifetime.
- Bounding-box collision detection between the player, platforms, and hazards.
- Health, elapsed-time score, high score, invulnerability after damage, and a game-over screen.
- 2D sprite, text, background, sound-effect, and looping music rendering.

## Technology stack

- **Language:** C++
- **Build system/IDE:** Visual Studio MSBuild project (`vs/Project/Project.sln`, `vs/Project/Project.vcxproj`)
- **Windowing/input/audio:** SDL2 and SDL2_mixer
- **Graphics:** OpenGL with GLEW
- **Math:** GLM
- **Text rendering:** FreeType
- **Texture loading:** SOIL
- **Target platform:** Windows; the project targets the Windows 10 SDK and uses the MSVC `v143` toolset.

## Repository layout

```text
.
├── CppProperties.json       Visual Studio C++ IntelliSense configuration
├── deps/
│   ├── include/              Bundled SDL, OpenGL/GLEW, GLM, SOIL, and FreeType headers
│   └── lib/                  Bundled platform libraries used by the Visual Studio project
├── vs/
│   ├── Project.sln           Visual Studio solution
│   └── Project/
│       ├── Project.vcxproj   Build settings, sources, include paths, and linker inputs
│       ├── unluckyCat.*       Active game class and application entry point
│       ├── Game.*              Engine lifecycle and main loop
│       ├── Input.*             Named keyboard, mouse, and controller mappings
│       ├── Sprite.*, Texture.* Rendering primitives and texture management
│       ├── Shader.*, Quad.*    OpenGL shader and geometry helpers
│       ├── Text.*              FreeType glyph loading and OpenGL text rendering
│       ├── GameObject.*         Falling-object state and movement
│       ├── Cat_Player.*         Player state, animation, health, and invulnerability
│       ├── BoundingBox.*,       Collision geometry and SAT collision helper
│       │   SATCollision.*
│       ├── Button.*, Music.*,   Menu UI and audio wrappers
│       │   Sound.*
│       ├── *.vert, *.frag       Sprite and text shaders
│       └── Assets/              Backgrounds, sprite sheets, fonts, and music
└── .gitignore                 Local Visual Studio/build-output exclusions
```

The repository also contains Visual Studio cache/build directories such as `.vs/`, `x64/`, `Debug/`, and `Release/`. They are generated development artifacts and are excluded by `.gitignore`; already tracked artifacts are not removed automatically.

## Architecture

### Engine lifecycle

`Engine::Game` in `vs/Project/Game.cpp` provides the reusable application shell:

1. `Run()` calls `InitInternal()` to initialize SDL, create an OpenGL window/context, initialize GLEW and SDL_mixer, create the orthographic projection, shaders, quad, and input manager.
2. The subclass `Init()` loads game resources and creates its screen state.
3. `GameLoop()` repeatedly polls SDL events, calls `Update()`, clears the OpenGL buffers, calls `Render()`, swaps the window buffers, limits the frame rate, and calculates delta time.
4. `Engine::unluckyCat` overrides `Init`, `Update`, and `Render` to supply the actual game.

The projection is 2D orthographic, with coordinates based on the configured window size. The active entry point creates a 720×576 window, targets 60 FPS, and starts the game with `game->Run()`.

### Screen/state flow

`unluckyCat` uses `ScreenState` values defined in `unluckyCat.h`:

- **MAIN_MENU:** `UpdateMainMenu()` navigates the Play/Exit buttons and `RenderMainMenu()` draws the background, controls, title, and buttons.
- **IN_GAME:** `UpdateInGame()` updates hazards, score, player movement, jumping, platform collisions, and damage; `RenderInGame()` draws the world and HUD.
- **GAME_OVER:** `UpdateGameOver()` updates the score/high score and handles retry or return-to-menu; `RenderGameOver()` draws the result screen.

### Gameplay/data flow

- `Cat_Player` wraps a `Sprite`, configures its 12×8 sprite sheet and animations, tracks health/jump state, and provides a 1.5-second invulnerability window after a hit.
- `GameObject` wraps each falling hazard. Objects begin in the `DIE` state, are reused by `SpawnObjects()`, fall until reaching the platform, remain grounded for three seconds, and then return to `DIE` for reuse.
- `Sprite` owns animation-frame selection, scale/flip/rotation transforms, shader uniforms, and a configurable bounding box.
- `BoundingBox`/`SATCollision` provide polygon-based collision support; gameplay checks the player and hazards/platforms through their bounding boxes.
- `Text` loads the first 128 glyphs of a font with FreeType into OpenGL textures and renders the title, controls, score, health, and game-over messages.
- `Texture`, `Shader`, `Quad`, `Music`, and `Sound` are the resource/rendering wrappers used by the game layer.

## Controls

| Action | Keyboard | Other mapping |
|---|---|---|
| Move left/right | Left/Right Arrow | SDL mouse/controller mappings are registered in code |
| Sprint | Left Shift | — |
| Jump | `Z` | Controller D-pad Up |
| Navigate menu | Up/Down Arrow | — |
| Select menu item | Enter | — |
| Return to menu | Escape | — |
| Retry after game over | `R` | — |

## Requirements

- Windows 10 or later.
- Visual Studio with **Desktop development with C++** installed.
- MSVC `v143` toolset and a Windows 10 SDK.
- An OpenGL-capable graphics driver.

The repository includes dependency headers, import/static libraries, runtime DLLs, and game assets. The project is configured to find headers and libraries relative to `vs/Project`, so keep the repository directory structure intact.

## Build and run

### Visual Studio

1. Clone the repository and open `vs/Project.sln` in Visual Studio.
2. Select a configuration and platform. `Debug|x64` is the recommended starting point; the solution also defines Win32/x86 and Release variants.
3. Build the `Project` project (`Build → Build Solution`).
4. Run with `Debug → Start Without Debugging` or press `F5`.

The program uses relative paths such as `sprite.vert`, `text.frag`, and `Assets/...`. Run it with `vs/Project` as the working directory. If Visual Studio starts the executable from another directory, set **Project → Properties → Debugging → Working Directory** to `$(ProjectDir)`.

### Command line MSBuild

From a Visual Studio Developer Command Prompt, run:

```bat
msbuild vs\Project.sln /p:Configuration=Debug /p:Platform=x64
vs\Project\Project\Project_debug.exe
```

If the executable cannot locate shaders or assets, change into the project directory before launching it:

```bat
cd vs\Project
Project\Project_debug.exe
```

The exact output name can vary by configuration: the Debug Win32 target is explicitly named `Project_debug`, while Release and some x64 settings use the normal project name.

## Development notes

- The active game entry point is `unluckyCat.cpp`; files named `Lesson01`–`Lesson07`, `Demo`, and `MainMenu` are older exercises/examples retained in the Visual Studio project and are not the application selected by the current `main()`.
- Asset paths are case-sensitive in the source strings and must match the names under `vs/Project/Assets`.
- Dependencies and binary outputs are currently committed in parts of the repository. Do not delete required runtime DLLs from the executable directory unless you replace them with compatible versions.
- The project has no automated test suite or package-manager configuration at this time.

## Current limitations

This is an early game prototype. Resource ownership is largely manual, some reset paths clear containers without deleting allocated objects, and the Visual Studio project contains legacy/demo code and generated binaries. These are useful areas to address as the project evolves.

## License and attribution

No license file is currently present. Before redistributing the project or its bundled fonts, music, images, DLLs, or libraries, confirm and document the licenses and attribution requirements for each asset and dependency.

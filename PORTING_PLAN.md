# Porting plan: Eaglercraft 26.2 to Xbox 360

## Goal

Investigate whether an Eaglercraft 26.2-compatible game experience can be implemented for Xbox 360 homebrew. This is a research plan, not a claim that the game has been ported.

## Important architecture constraint

Eaglercraft is a browser-based JavaScript/WebGL project. The current LibXenon workflow builds native C/C++ code for Xbox 360. Those are different execution environments: a web page is not a native XEX, and compiling a small C program does not compile or translate the Eaglercraft game.

## Milestone 1 — establish a reliable native baseline

- Verify the current ELF artifact launches on the intended console/loader.
- Record the exact loader, video mode, and build commit for each test.
- Add a minimal test that initializes graphics and reads controller input.
- Keep the test independent of the existing VEYAORA shell until its behavior is understood.

**Exit condition:** a repeatable boot with visible output and working controller input.

## Milestone 2 — graphics prototype

- Draw a simple textured scene using APIs supported by the chosen Xbox 360 homebrew environment.
- Add camera movement and a small block grid.
- Measure frame rate and memory use on hardware.
- Do not assume browser WebGL APIs are available in LibXenon.

**Exit condition:** stable block rendering and navigation on the console.

## Milestone 3 — feasibility review for the actual game

- Audit the exact Eaglercraft 26.2 distribution and its runtime dependencies.
- Identify browser APIs, JavaScript features, WebGL calls, networking, storage, and input that would need an alternative implementation.
- Estimate whether a JavaScript runtime can realistically run on the console or whether a substantial native reimplementation is necessary.
- Check multiplayer protocol compatibility separately from rendering.

**Exit condition:** a documented technical decision before attempting a full port.

## Browser project page

The root `index.html` contains an interactive browser-only block sandbox. It is a UI/interaction prototype, not the Eaglercraft game and not evidence that the Xbox 360 build works.

## Risks

- Toolchain and loader differences may make some ELF/XEX artifacts incompatible.
- Modern JavaScript/WebGL requirements may be too costly to recreate on the Xbox 360.
- Networking and multiplayer compatibility may require additional porting work.
- A visual prototype is not a playable port.

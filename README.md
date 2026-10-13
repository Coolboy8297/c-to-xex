# Eaglercraft 26.2 — Xbox 360 Lab

An experimental research project exploring what it would take to bring an Eaglercraft-inspired, controller-first block game to Xbox 360 homebrew.

**Project page:** https://coolboy8297.github.io/c-to-xex/  
**Builds:** https://github.com/Coolboy8297/c-to-xex/actions

## Current status

- The root `index.html` is a GitHub Pages landing page with an interactive browser-only block sandbox.
- The existing GitHub Actions workflow builds the repository's C/C++ LibXenon project and publishes build artifacts.
- The browser sandbox is **not Eaglercraft 26.2** and is not an Xbox executable.
- Eaglercraft's browser runtime cannot be converted to XEX by renaming the HTML or passing it through a C compiler. A native port or compatible runtime would be required.

## First milestones

1. Confirm the current LibXenon ELF starts reliably on the target console.
2. Build a minimal native test for graphics initialization and controller input.
3. Prototype block rendering and measure performance/memory.
4. Research the runtime and rendering work needed for an actual Eaglercraft 26.2 port.

See [PORTING_PLAN.md](PORTING_PLAN.md) for the technical breakdown.

## Publish the website

In the repository, open **Settings → Pages**. Set **Source** to **Deploy from a branch**, select **main** and **/(root)**, then save. The site entry point is the root `index.html`. GitHub may take a few minutes to publish the first build.

## Xbox 360 build workflow

Push C/C++ source into `source/`, then open the **Actions** tab and download the `xbox360-homebrew` artifact from a completed build. The existing build workflow is experimental; test new builds carefully and keep known-working ELF files backed up.

## Links

- [GitHub Actions](https://github.com/Coolboy8297/c-to-xex/actions)
- [Eaglercraft 26.2 in a browser](https://eaglercraft.com/play?version=26.2)

# HTML/CSS/JS to C Converter — Xbox 360 ELF32

**Website:** https://coolboy8297.github.io/c-to-xex/  
**Build results:** https://github.com/Coolboy8297/c-to-xex/actions

This repository has a browser-based HTML-to-C source converter and a GitHub Actions LibXenon build pipeline.

## Use the pipeline

1. Open the website and drop in an `.html` file.
2. Click **Convert HTML to C**, then download the generated `.c` file.
3. In this repository, replace `source/main.c` with that generated C file and commit the change. Keep the file named `main.c` in the `source/` directory.
4. The existing GitHub Actions workflow runs on the push. Open **Actions** and wait for **Build Xbox 360 Homebrew** to finish.
5. Download the `xbox360-homebrew` artifact ZIP from the successful workflow run. It contains the generated ELF/ELF32 and XEX build outputs when the toolchain succeeds.

## Important limits

- The current converter turns the HTML document's visible text into C console-print statements. It does **not** translate arbitrary CSS styling, images, canvas/WebGL, DOM behavior, or JavaScript logic into native code.
- This is a first-stage text conversion, not a full browser engine. A complex web game such as Eaglercraft cannot become a working native Xbox game through this simple conversion; its runtime and rendering would need to be ported or implemented separately.
- GitHub Pages cannot run the Xbox toolchain itself. The website generates C in your browser; GitHub Actions compiles committed C using the LibXenon toolchain.
- Builds are experimental. Check the Actions logs and test ELF32 compatibility on the target console.

## Files

- `index.html` — drag-and-drop HTML-to-C converter.
- `source/main.c` — native C source compiled by the workflow.
- `.github/workflows/build.yml` — builds the LibXenon project and uploads the output artifacts.

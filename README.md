# HTML/CSS/JS to C Converter — Xbox 360 ELF32

**Website:** https://coolboy8297.github.io/c-to-xex/  
**Direct build downloads:** https://github.com/Coolboy8297/c-to-xex/releases/tag/latest-build

The website includes a browser-based HTML-to-C source converter and direct links to the latest published Xbox 360 build. Website visitors do not need to open the Actions page to download the published files.

## Download the latest published build

Open the [Latest Xbox 360 build release](https://github.com/Coolboy8297/c-to-xex/releases/tag/latest-build) or use the direct files linked on the website:

- `xbox360-homebrew.xex`
- `xbox360-homebrew.elf`
- `xbox360-homebrew.elf32`
- `SHA256SUMS.txt` (checksums)

The release is updated after a successful build on the default branch. These links will not return files until the first successful build publishes the release. The Actions artifact remains available for maintainers, but ordinary users do not need it.

## Use the converter

1. Open the website and drop in an `.html` file.
2. Click **Convert HTML to C**, then download the generated `.c` source.
3. The website currently converts visible HTML text into C console-print statements. It does not compile each visitor's uploaded file into a custom ELF/XEX.

## Important limits

- The current converter turns the HTML document's visible text into C console-print statements. It does **not** translate arbitrary CSS styling, images, canvas/WebGL, DOM behavior, or JavaScript logic into native code.
- This is a first-stage text conversion, not a full browser engine. A complex web game such as Eaglercraft cannot become a working native Xbox game through this simple conversion; its runtime and rendering would need to be ported or implemented separately.
- GitHub Pages cannot run the Xbox toolchain itself. The site generates C in the browser; GitHub Actions compiles the repository's committed C using LibXenon, then publishes direct release downloads.
- Builds are experimental. Check build results and test ELF32 compatibility on the target console.

## Files

- `index.html` — drag-and-drop HTML-to-C converter and direct build download links.
- `source/main.c` — native C source compiled by the workflow.
- `.github/workflows/build.yml` — builds the LibXenon project, uploads maintainer artifacts, and publishes release files for direct download.

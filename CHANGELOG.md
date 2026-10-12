# Changelog

All notable changes to **nestri** are documented here.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project uses the logue SDK version scheme (`<major>.<minor>-<patch>`).

## [1.1.0] - 2026-10-12

### Added
- **LFO Mod** edit-menu parameter: selects which control sits on the
  LFO-modulatable SHAPE knob. `1` (default) = SHAPE drives Morph and SHIFT+SHAPE
  drives Step; `2` = swapped, so the SHAPE LFO can sweep the step resolution.

### Changed
- Renamed the source file `triangle.cpp` → `nestri.cpp` and the build project
  (`PROJECT`) to `nestri`, so the packaged unit is now `nestri.mnlgxdunit`.
- Manifest version bumped to `1.1-0`.
- README documents the two controls (Morph / Step) and the new LFO Mod switch.

## [1.0.0] - 2026-10-11

### Added
- Initial release of the NES-style triangle oscillator for the Korg minilogue xd.
- SHAPE morphs a smooth triangle into a stepped, bit-crushed NES-style triangle
  (LFO-modulatable); SHIFT+SHAPE sets the step resolution (2 … 128 levels,
  16 = the NES 4-bit triangle).

[1.1.0]: https://github.com/danielhuebert/nestri/releases/tag/v1.1.0
[1.0.0]: https://github.com/danielhuebert/nestri/releases/tag/v1.0.0

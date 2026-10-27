# Bouncers
Little squares bouncing around your GBA's screen, made using Butano.
A yellow dot follows the average position of the squares.

## Controls
- **A** - Add a square, up to 20
- **B** - Remove a square

# Play
## Online (recommended)
Visit https://tk421bsod.github.io/sdev301-bouncers

## Build from source
First, either open the project in the included VSCode Dev Container or install Butano and its dependencies.
If you use the latter option, set the `LIBBUTANO` `Makefile` variable to the path Butano is installed at.
Once finished, run `make`.
Then open `bouncers.gba` in your emulator. (or copy to a flash cart!)
# SPFLIVL — Small Projects For Learning In Various Languages

A single repo collecting the small projects I build while learning to program.
Each subfolder is its own self-contained project (with its own README and build
instructions). I keep them here so I have one place to look back at.

## Projects

- **Calculator** — a basic and an advanced calculator in C++.
- **Sudoku-Solver** — a backtracking Sudoku solver in C. Reads a puzzle from
  a text file, solves it, prints both. See `Sudoku-Solver/README.md`.
- **Dynamic-Array** — a growing array (vector) written from scratch in C:
  `malloc`/`realloc`/`free`, doubling capacity, header/source split, tests,
  Makefile, ASan-clean. See `Dynamic-Array/README.md`.
- **Password-Manager** — a CLI password manager in C, C++, and Rust. Stores
  app/user/password in a plain-text vault file. Plain text = no encryption;
  learning project only, not for real passwords. See `Password-Manager/README.md`.

## Notes

- These are learning projects, not production code.
- Build artifacts (`Build/`, `target/`, `Output/`) are gitignored; create them
  locally when building.

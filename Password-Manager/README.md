# Password Manager

A small CLI password manager I wrote to learn C, C++, and Rust. Stores app
logins (app name, username, password) in a plain-text vault file. Plain text
means no encryption — this is a learning project, not a system to store real
passwords in.

## Project layout

```
Password-Manager/
├── README.md
├── .gitignore
├── C/
│   ├── Manager.c
│   ├── Build/            # compiled binary (gitignored)
│   └── Output/           # runtime vault dir (gitignored)
├── C++/
│   ├── Manager.cpp
│   ├── Build/
│   └── Output/
└── Rust/
    ├── Cargo.toml
    ├── Cargo.lock
    ├── src/
    │   └── main.rs
    ├── Build/            # cargo build output (gitignored; cargo uses target/)
    ├── Output/
    └── target/           # cargo artifact dir (gitignored)
```

`Build/`, `Output/`, and `target/` are all gitignored — they are created at
build/run time, so a fresh clone has none of them.

## How it works

- **Load** — on start, the program reads `Output/Vault.txt` into memory.
  First line is the account count, then one line per account
  (`App User Pass`, space-separated).
- **Menu** — `Manage` lists accounts and shows one; `Create` adds a new
  account and rewrites the whole file; `Exit` quits.
- **Save** — every create rewrites the entire vault, so the file always
  matches memory.

## Building & running

`Output/` is gitignored, so create it before running the C/C++ versions
(Rust creates it itself via `create_dir_all`).

### C

```sh
mkdir -p C/Build C/Output
gcc -o C/Build/passmgr C/Manager.c && C/Build/passmgr
```

Run from the project root (so `Output/Vault.txt` resolves).

### C++

```sh
mkdir -p C++/Build C++/Output
clang++ -std=c++20 -o C++/Build/passmgr C++/Manager.cpp && C++/Build/passmgr
```

Run from the project root (so `Output/Vault.txt` resolves).

### Rust

```sh
cd Rust
cargo build          # debug build -> target/debug/password-manager
cargo run            # build + run
cargo run --release  # optimized build -> target/release/password-manager
cargo fmt            # format Src/main.rs
cargo check          # type-check without producing a binary
```

`cargo run` creates `Rust/Output/` on first save. Run from inside `Rust/`.

## Notes on the Rust layout

Rust's convention is a lowercase `src/` directory, and that wins over my
PascalCase directory convention here (language conventions come first). No
`Cargo.toml` override is needed — `src/main.rs` is the default.
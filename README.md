# Password Manager (C CLI)

A small CLI password manager I wrote to learn C. Stores app logins
(app name, username, password) in a plain-text vault file. Plain text
means no encryption — this is a learning project, not a system to
store real passwords in.

## Project layout

```
Password-Manager/
├── README.md
├── .gitignore
└── C/
    ├── Manager.c
    ├── build/            # compiled binary (gitignored)
    └── output/
        ├── .gitkeep      # keeps the folder in git (see below)
        └── vault.txt     # the saved vault (gitignored)
```

## How it works

- **Load** — on start, `manager.c` reads `output/vault.txt` into memory.
  First line is the account count, then one line per account
  (`App User Pass`, space-separated).
- **Menu** — `Manage` lists accounts and shows one; `Create` adds a new
  account and rewrites the whole file; `Exit` quits.
- **Save** — every create rewrites the entire vault with `fopen(..., "w")`,
  so the file always matches memory.

## Why is `C/output/` almost empty?

The program writes the vault there at runtime, and `fopen` creates the
**file** but never the **directory**. If `output/` didn't exist, saving
would silently fail. Git also doesn't track empty folders — so the
`.gitkeep` placeholder guarantees the folder exists on any fresh clone.

The actual `vault.txt` is gitignored because it's a runtime data store,
not source code.

## Building & running

```sh
gcc -o build/passmgr C/Manager.c && build/passmgr
```

Run from the project root (so `output/vault.txt` resolves).
# Dynamic Array (C)

My C learning project — a growing array (vector) written from scratch. It's
the project that finally got me into pointers and heap memory. Fixed-size
arrays (like the `vault[1024]` in my password manager) can't grow; this one
can. It starts with room for 4 and doubles whenever it fills up.

## What I learned here

- pointers and `->` vs `.`
- `malloc` / `realloc` / `free` — my first time owning heap memory
- automatic resizing by doubling the capacity (so pushing n things is
  amortized O(1))
- header + source split: `vec.h` is the menu (struct + prototypes), `vec.c`
  is the kitchen (the implementation), `test.c` is the test harness
- `#include "..."` vs `#include <...>`, header guards
- a Makefile that builds the `.o` files and links them
- bounds-checking the public ops: `vecGet` / `vecSet` validate the index
  against `used`, `vecPop` refuses an empty crate — a bad call exits with a
  message instead of quietly reading or writing junk
- `static` for `checkIndex`: file-private, shared by get and set, and
  deliberately *not* in `vec.h`
- why ASan exists: once it caught a heap-buffer-overflow I didn't even know
  I had, then caught a leak. Run the sanitizer build, learn the bug is real.

## Layout

```
Dynamic-Array/
├── Include/Vec.h   # the Vec struct + prototypes (the API)
├── Src/Vec.c       # the implementation
├── Tests/Test.c    # one test per function
├── build/          # objects + binary (gitignored)
├── Makefile
└── README.md
```

## The Vec struct

```c
typedef struct {
    int* items; // the actual numbers, living on the heap
    int  used;  // how many slots are filled right now
    int  room;  // total seats the heap chunk can hold
} Vec;
```

`room` vs `used`: capacity vs contents. Pushing only fills `used`; `room`
only jumps when the bus is full.

## API

```c
void vecInit(Vec* vec);                       // start empty: items=NULL, used=0, room=0
void vecPush(Vec* vec, int value);            // append; doubles capacity when full
int  vecGet(Vec* vec, int index);             // read items[index] (bounds-checked)
void vecSet(Vec* vec, int index, int value);  // overwrite items[index] (bounds-checked)
int  vecPop(Vec* vec);                        // remove + return last (errors if empty)
void vecFree(Vec* vec);                       // hand the heap chunk back
```

## Build

```sh
make && ./build/vec
make clean
```

With sanitizers (should report zero errors / leaks):

```sh
make clean && make CFLAGS="-fsanitize=address,undefined -g -O0 -Wall -Wextra -IInclude"
```

## Tests

`Tests/Test.c` has one test per function, run from `main`:

- `testInit` — a fresh crate is empty: `items == NULL`, `used == 0`, `room == 0`
- `testPush` — values land in order, `used` counts them, and pushing past
  the starting room of 4 forces the doubling (4 → 8 → 16)
- `testGet` — reads back exactly what was pushed, by index
- `testSet` — overwrites one slot in place and leaves the size alone
- `testPop` — last in, first out; `used` drops but `room` stays put
- `testFree` — clears the crate and a second free is harmless

Each check asserts (via the `CHECK` macro) instead of just printing, so a
failure is loud and the whole thing exits non-zero if anything is wrong —
`make && ./build/vec` prints `all tests passed` on success. The run still
prints nothing but that one line; the per-push `used`/`room` trace from the
old demo is gone, since the tests check those values directly now.

The three bounds-check paths are tested too — `vecGet` and `vecSet` past
`used`, and `vecPop` on an empty crate. Those call `exit()` rather than
return, so I can't check them in-process: `expectExitFailure` forks a child
to run the bad call and the parent confirms it exited with `EXIT_FAILURE`.
The child uses an empty vec on purpose, so no heap is allocated and the
sanitizer build's leak checker stays quiet.

## Status

Done — I wrote the whole API and it's ASan/UBSan-clean: `vecInit`,
`vecPush`, `vecGet`, `vecSet`, `vecPop`, `vecFree`. `Tests/Test.c` drives
every one of them. I bounds-check `vecGet`/`vecSet` against `used` and
make `vecPop` refuse an empty crate, so a bad call exits with a message
instead of touching junk.
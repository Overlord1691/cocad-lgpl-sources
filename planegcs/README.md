# PlaneGCS — vendored

FreeCAD's 2D geometric constraint solver. CoCAD's sketcher is built on it (ADR 0005); this
directory is the extraction the Phase 1 week-1 spike called for, and ADR 0035 records how it was
done and why the shape is what it is.

## Provenance — this is the pin

| | |
|---|---|
| Upstream | https://github.com/FreeCAD/FreeCAD |
| Tag | `1.1.3` |
| Commit | `145529fe741292ff0b3977a01195bf0247425794` |
| Path | `src/Mod/Sketcher/App/planegcs/` |
| Vendored | 2026-07-31 |
| Licence | LGPL-2.1-or-later (`LICENSE.LGPL-2.1.txt`, verbatim from upstream's `LICENSE`) |

This is the commit hash ADR 0013's dependency table asks for. Do not update it casually: the
solver's numerical behaviour is what the golden sketch tests pin, so a bump is a change with test
consequences and belongs in its own commit.

## Layout, and why it is shaped like this

```
App/planegcs/     the 11 upstream files, byte-for-byte unmodified
Base/Console.h    shim
FCConfig.h        shim
SketcherGlobal.h  shim
boost_graph_adjacency_list.hpp   shim
```

**Nothing under `App/planegcs/` is edited, ever.** That is the whole design constraint, and the
directory nesting exists to serve it: the upstream sources include `"../../SketcherGlobal.h"` by
relative path, so placing them two levels below this directory makes that include resolve to the
shim here without touching a line. A patched vendor tree has to be re-patched at every upstream
bump, and the patches are exactly the thing nobody remembers.

`tools/check-planegcs-pristine.mjs` enforces it — the SHA-256 of every vendored file is recorded
in `pristine.json` and checked in `pnpm run check`. If you meant to change the solver's behaviour,
the answer is a wrapper in `src/sketch/`, not an edit here.

## The four shims

Upstream compiles inside FreeCAD and reaches for four of its headers. Each shim is the smallest
thing that satisfies the include; each file says at its top what upstream's version does and what
was dropped.

| Shim | Upstream is | Here it is |
|---|---|---|
| `SketcherGlobal.h` | dllimport/dllexport selection | empty macros — static library, nothing to export |
| `FCConfig.h` | the application's generated build config | one derived macro, `EIGEN_SPARSEQR_COMPATIBLE` |
| `Base/Console.h` | the FreeCAD console singleton | two methods over a function-pointer sink, silent by default |
| `boost_graph_adjacency_list.hpp` | warning-suppressing wrapper | plain forward to Boost |

## Dependencies

Eigen (MPL-2.0) and Boost headers (BSL-1.0) — both already cleared by Architecture §7 as
PlaneGCS's transitive dependencies. Supplied by the vcpkg `solver` feature.

Worth knowing for later: the Boost surface is **five symbols in one function** —
`adjacency_list`, `add_vertex`, `add_edge`, `num_vertices` and `connected_components`, used by
`System::identifyDependentParameters` to split the constraint graph into independent subsystems.
Boost is by far the heaviest thing in this build for that. Replacing it with a ~40-line union-find
is tempting and is deliberately **not** done: it would be the first patch to a pristine tree, and
the rule above is worth more than the build time. Recorded in `docs/BACKLOG.md` in case that
trade ever changes.

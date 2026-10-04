# CoCAD - LGPL source code

CoCAD (https://cocad.ai) is a desktop CAD application made by CoCAD LLC. It uses two libraries
licensed under the **GNU Lesser General Public License, version 2.1**:

| Library | Version CoCAD ships | Shipped as | Upstream |
| --- | --- | --- | --- |
| **Open CASCADE Technology (OCCT)** | 8.0.0, tag `V8_0_0_p1` | the `TK*.dll` files beside `cocad-core.exe` | https://github.com/Open-Cascade-SAS/OCCT |
| **PlaneGCS** (FreeCAD's sketch solver) | FreeCAD 1.1.3, commit `145529fe` | `planegcs.dll` beside `cocad-core.exe` | https://github.com/FreeCAD/FreeCAD |

This repository is CoCAD's public, free source offer for both, as the LGPL requires: the
**complete corresponding source** of the copies CoCAD distributes, with everything needed to
rebuild them and replace the DLLs in your CoCAD install. Nothing here is CoCAD's own code; CoCAD
links to these libraries only as separate, replaceable DLLs.

Each CoCAD release has a **tag and a GitHub Release here with the same version number**. Pick the
one that matches your copy of CoCAD (Help > About shows the version). Old releases are never
removed.

## Open CASCADE (`opencascade/`)

CoCAD builds OCCT with [vcpkg](https://github.com/microsoft/vcpkg), triplet `x64-windows`
(shared libraries, MSVC 2022), with the `freetype` and `rapidjson` features.

- **The source:** `Open-Cascade-SAS-OCCT-V8_0_0_p1.tar.gz`, attached to this repository's
  Release. It is byte-for-byte the archive of upstream tag `V8_0_0_p1`. SHA-512:
  `f150f73a5b0cfd202838465d4fffabfc1177b1edbf175a1fa375bcec575896a35b22422bee711d5ae948c4fc242a0a89ed68f1a45a693c2b54b9b8326eabf669`
- **The modifications:** `opencascade/vcpkg-port/` is the vcpkg port exactly as built (vcpkg
  commit `fd3dde77905b1b1890df3c8daa496d9b2fe44f7f`, port tree `b2379b35`): `portfile.cmake`, the
  build options, and the five patches applied to the source (`0001-` to `0005-`). CoCAD applies
  no other changes to OCCT.
- **The build record:** `opencascade/vcpkg_abi_info.txt` is vcpkg's own record of the build
  (features, triplet, the hash of every input).
- **Licence:** `opencascade/LICENSE_LGPL_21.txt` and `opencascade/OCCT_LGPL_EXCEPTION.txt`, from
  the source archive.

To rebuild (Windows, Visual Studio 2022 with the C++ workload, and vcpkg at the commit above):

```
cd opencascade\rebuild
vcpkg install --triplet x64-windows
```

vcpkg downloads the archive from GitHub and checks it against the SHA-512 above; you can also put
the copy from the Release into vcpkg's `downloads` folder first. The DLLs land in
`vcpkg_installed\x64-windows\bin\`. Close CoCAD and copy them over the `TK*.dll` files in the
CoCAD install folder.

## PlaneGCS (`planegcs/`)

`planegcs/` is the directory CoCAD vendors, unchanged:

- `App/planegcs/` - the 11 upstream files, byte-for-byte unmodified (`pristine.json` holds their
  SHA-256 hashes; `README.md` the provenance).
- Four small shim headers (`FCConfig.h`, `SketcherGlobal.h`, `Base/Console.h`,
  `boost_graph_adjacency_list.hpp`) that stand in for the parts of FreeCAD the solver includes.
- `CMakeLists.txt` - how CoCAD compiles it into `planegcs.dll`.
- `LICENSE.LGPL-2.1.txt` - the licence, verbatim from upstream.

To rebuild `planegcs.dll` (Windows, Visual Studio 2022, vcpkg):

```
cd planegcs-standalone
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=<vcpkg>\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release
```

Close CoCAD and copy `build\Release\planegcs.dll` over the one in the CoCAD install folder.

## Replacing a DLL

Replacing either library is your right under the LGPL. A replacement with a different interface
may stop CoCAD from starting or computing correctly; CoCAD support covers the libraries as shipped.

## Written offer

For at least three years after CoCAD LLC last distributes a version of CoCAD, CoCAD LLC will also
provide this source on written request to **support@cocad.ai**, for no more than the cost of
providing it (CoCAD End User License Agreement, the third-party components section).

## Licence of this repository

The library sources are under their own licences (LGPL-2.1 for PlaneGCS; LGPL-2.1 with the Open
CASCADE exception for OCCT). The few build-helper files CoCAD LLC wrote here
(`opencascade/rebuild/`, `planegcs-standalone/`, this README) are offered under the same
LGPL-2.1 (`LICENSE`).

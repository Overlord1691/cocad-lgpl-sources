// CoCAD shim for FreeCAD's src/Mod/Sketcher/SketcherGlobal.h.
//
// The vendored PlaneGCS sources include this by the relative path "../../SketcherGlobal.h",
// which is why the upstream files sit under App/planegcs/ and this file sits here: the
// relative include resolves without editing a single vendored line. See README.md.
//
// Upstream's version picks a dllimport/dllexport attribute depending on whether the Sketcher
// module is being built or consumed. CoCAD links PlaneGCS as a STATIC library into one
// executable, so there is nothing to import or export and the macro is empty. Defining it to
// __declspec(dllexport) here would be actively wrong: it would put ~16 symbols in an export
// table of an .exe for no reader.
#ifndef COCAD_PLANEGCS_SHIM_SKETCHERGLOBAL_H
#define COCAD_PLANEGCS_SHIM_SKETCHERGLOBAL_H

#define SketcherExport
#define SketcherGuiExport

#endif  // COCAD_PLANEGCS_SHIM_SKETCHERGLOBAL_H

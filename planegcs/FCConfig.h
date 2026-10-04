// CoCAD shim for FreeCAD's FCConfig.h.
//
// Upstream this is the generated build-configuration header for the whole application: version
// strings, feature toggles, platform macros. GCS.cpp includes it for exactly one symbol,
// EIGEN_SPARSEQR_COMPATIBLE, which upstream defines when the Eigen it found is 3.2.2 or newer.
//
// CoCAD pins Eigen well past that (ADR 0013), so rather than leave the macro undefined -- which
// makes PlaneGCS fall back to dense QR at runtime and print a warning -- derive it from Eigen's
// own version macros. This is a decision about our build, not a patch to the solver.
#ifndef COCAD_PLANEGCS_SHIM_FCCONFIG_H
#define COCAD_PLANEGCS_SHIM_FCCONFIG_H

#include <Eigen/Core>

#if EIGEN_VERSION_AT_LEAST(3, 2, 2)
#define EIGEN_SPARSEQR_COMPATIBLE
#endif

#endif  // COCAD_PLANEGCS_SHIM_FCCONFIG_H

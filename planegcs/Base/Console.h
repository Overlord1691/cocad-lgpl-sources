// CoCAD shim for FreeCAD's src/Base/Console.h.
//
// The vendored solver reports through Base::Console() in two places: solver diagnostics behind
// its own `debugMode` flag (off by default), and one warning when the available Eigen is too old
// for SparseQR. Upstream's console is a large singleton wired into the FreeCAD application --
// GUI panels, log levels, observers, Python bindings. PlaneGCS uses two of its methods.
//
// So this shim reimplements those two methods over a single function-pointer sink instead of
// vendoring an application subsystem. Two properties matter:
//
//   * The default sink is null and formatting is skipped entirely when it is. That is not just an
//     optimisation: several upstream call sites pass a std::string as the *format* argument
//     (`log(tmp.c_str())`), so a solver message containing a stray '%' would be undefined
//     behaviour the moment it were formatted. Silent-by-default means CoCAD never formats a
//     string it did not itself supply a format for, and the hazard cannot fire unless a caller
//     opts in.
//   * planegcs stays a leaf. It links nothing of CoCAD's, so cocad-core can install a sink that
//     forwards into ADR 0028's rotating log without the solver ever depending on the logger --
//     which is what keeps the LGPL isolation boundary a compile-time fact rather than a promise.
#ifndef COCAD_PLANEGCS_SHIM_BASE_CONSOLE_H
#define COCAD_PLANEGCS_SHIM_BASE_CONSOLE_H

#include <cstdarg>
#include <cstdio>
#include <string>

namespace Base {

enum class ConsoleLevel { Log, Warning };

/// Sink for solver diagnostics. Null means "discard, and do not even format". Set it from
/// cocad-core (see kernel/solver.cpp) to route PlaneGCS's chatter into the CoCAD log.
using ConsoleSink = void (*)(ConsoleLevel level, const char* message);

inline ConsoleSink& console_sink() {
    static ConsoleSink sink = nullptr;
    return sink;
}

class ConsoleSingleton {
public:
    void log(const char* format, ...) {
        va_list args;
        va_start(args, format);
        emit(ConsoleLevel::Log, format, args);
        va_end(args);
    }

    void warning(const char* format, ...) {
        va_list args;
        va_start(args, format);
        emit(ConsoleLevel::Warning, format, args);
        va_end(args);
    }

private:
    static void emit(ConsoleLevel level, const char* format, va_list args) {
        const ConsoleSink sink = console_sink();
        if (sink == nullptr || format == nullptr) {
            return;
        }
        va_list measure;
        va_copy(measure, args);
        const int needed = std::vsnprintf(nullptr, 0, format, measure);
        va_end(measure);
        if (needed < 0) {
            return;
        }
        std::string text(static_cast<std::size_t>(needed) + 1U, '\0');
        std::vsnprintf(text.data(), text.size(), format, args);
        text.resize(static_cast<std::size_t>(needed));
        sink(level, text.c_str());
    }
};

inline ConsoleSingleton& Console() {
    static ConsoleSingleton instance;
    return instance;
}

}  // namespace Base

#endif  // COCAD_PLANEGCS_SHIM_BASE_CONSOLE_H

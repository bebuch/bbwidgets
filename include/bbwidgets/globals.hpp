#pragma once

#include <QtGlobal>

#include <concepts>


#ifdef SHARED_LIBRARY
#define DLLEXPORT Q_DECL_EXPORT
#else
#define DLLEXPORT Q_DECL_IMPORT
#endif


namespace bbwidgets {


    template <std::signed_integral T>
    [[nodiscard]] constexpr T cyclic_adapt(T const value, T const begin, T const end) {
        auto const diff = end - begin;
        auto const based = value - begin;
        auto mod = based % diff;
        if(mod < 0) {
            mod += diff;
        }
        return begin + mod;
    }


}

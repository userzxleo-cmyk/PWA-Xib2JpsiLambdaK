#pragma once

namespace Static_Check {
    namespace Resonances {
        template <unsigned _number_of_Lz, unsigned _number_of_Pc, unsigned _number_of_Zcs>
        void check_count() {
            static_assert(Constant::number_of_Lz == _number_of_Lz);
            static_assert(Constant::number_of_Pc == _number_of_Pc);
            static_assert(Constant::number_of_Zcs == _number_of_Zcs);
        }
        // Hack to avoid checking defaults during compile-time
        template <bool check = false> void use_default() {
            if constexpr (check)
                check_count<5, 12, 0>();
        }
    } // namespace Resonances

    namespace Systematics {
        template <bool _isospin_sigmacd,
                  bool _linear_kmnr,
                  bool _self_scattering,
                  bool _lineshape_lbplusone>
        void check_flags() {
            static_assert(ISOSPIN_SIGMACD == _isospin_sigmacd);
            static_assert(LINEAR_KMNR == _linear_kmnr);
            static_assert(SELF_SCATTERING == _self_scattering);
            static_assert(LINESHAPE_LBPLUSONE == _lineshape_lbplusone);
        }
        // Hack to avoid checking defaults during compile-time
        template <bool check = false> void use_default() {
            if constexpr (check)
                check_flags<false, false, false, false>();
        }
    } // namespace Systematics

    namespace Counters {
        template <unsigned _number_of_knots, unsigned _max_ikmcount>
        void check_spline_and_KM_channels() {
            static_assert(_number_of_knots == NUMSP);
            static_assert(Constant::max_ikmcount == _max_ikmcount);
        }
        // Hack to avoid checking defaults during compile-time
        template <bool check = false> void use_default() {
            if constexpr (check)
                check_spline_and_KM_channels<24, 6>();
        }
    } // namespace Counters

    template <bool check = false> void use_default() {
        if constexpr (check) {
            Resonances::use_default<true>();
            Systematics::use_default<true>();
            Counters::use_default<true>();
        }
    }
} // namespace Static_Check

#define NUMSP 24

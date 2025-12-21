#pragma once

// These need to change depending number of resonances
// Otherwise, we can template kernel_calEva (not a bad idea given GPU ID thing)
namespace Constant {
    static constexpr unsigned number_of_Lz = 5;
    static constexpr unsigned number_of_Pc = 12;
    static constexpr unsigned number_of_Zcs = 0;
    static constexpr unsigned number_of_resonances = number_of_Lz + number_of_Pc + number_of_Zcs;
    static constexpr unsigned max_ikmcount = 6; // 7 if systematic for broad Pc
}; // namespace Constant
static constexpr unsigned MYRN = Constant::number_of_Lz;
static constexpr unsigned MYZN = Constant::number_of_Pc;
static constexpr unsigned MYXN = Constant::number_of_Zcs;

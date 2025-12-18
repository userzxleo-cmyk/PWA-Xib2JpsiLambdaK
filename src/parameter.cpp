#include "RooTDalitz/parameter.h"
#include "RooTDalitz/random.h"

#include "RooAbsReal.h"
#include "RooTDalitz/Resonances.h"

namespace Parameter {
    template <typename T> T *CastFromList(RooArgList *arg_list, const unsigned index) {
        return dynamic_cast<T *>(arg_list->at(index));
    }

    void random_smear(RooRealVar *variable, double delta) {
        double initial_value = variable->getVal();
        double smeared_value = delta + initial_value;
        if (variable->hasMin() && smeared_value < variable->getMin()) {
            double lower_limit = variable->getMin();
            double reflection = lower_limit - smeared_value;
            double reflected_value = lower_limit + reflection;
            smeared_value = reflected_value;
        } else if (variable->hasMax() && smeared_value > variable->getMax()) {
            double upper_limit = variable->getMax();
            double reflection = smeared_value - upper_limit;
            double reflected_value = upper_limit - reflection;
            smeared_value = reflected_value;
        }
        variable->setVal(smeared_value);
    }

    void configure_error(RooRealVar *variable, bool setError) {
        if (setError) {
            variable->setError(0.1);
        }
        if (variable->hasMax() &&
            (variable->getVal() > (variable->getMax() - 1.1 * variable->getError()))) {
            variable->setVal(variable->getMax() - 1.1 * variable->getError());
        }
        if (variable->hasMin() &&
            (variable->getVal() < (variable->getMin() + 1.1 * variable->getError()))) {
            variable->setVal(variable->getMin() + 1.1 * variable->getError());
        }
    }

    // Randomize a parameter
    void Randomize_par(RooRealVar &variable) {
        if (variable.isConstant()) {
            std::cout << "Randomize failed due to constant parameter " << variable.GetName()
                      << std::endl;
            return;
        }
        variable.setVal(Random::get_generator().Gaus() * variable.getVal() * 0.2 +
                        variable.getVal());
        variable.setError(0.01);
        if (variable.hasMax() &&
            (variable.getVal() > (variable.getMax() - 1.1 * variable.getError()))) {
            variable.setVal(variable.getMax() - 1.1 * variable.getError());
        }
        if (variable.hasMin() &&
            (variable.getVal() < (variable.getMin() + 1.1 * variable.getError()))) {
            variable.setVal(variable.getMin() + 1.1 * variable.getError());
        }
        variable.setConstant(0);
    }

    // set how many SL of Lambda to float
    void SetLPar(RooArgList *argli,
                 int maxind = 6,
                 bool dornd_LS = false,
                 bool fixfirst = false,
                 bool dornd_LS_crazy = false) {
        auto spin_variable = CastFromList<RooAbsReal>(argli, argli->getSize() - 2);
        int spin = static_cast<int>(spin_variable->getVal());
        int maxi = maxind - 1;

        if (spin == 1 && maxi > 3)
            maxi = 3;

        int mini = 0;
        if (fixfirst)
            mini = 1;

        const double smear_scale = (dornd_LS_crazy) ? 0.3 : 0.2;

        for (int ind = mini; ind <= maxi; ++ind) {
            RooRealVar *varA = CastFromList<RooRealVar>(argli, 2 * ind);
            RooRealVar *varB = CastFromList<RooRealVar>(argli, 2 * ind + 1);
            if (varA == nullptr || varB == nullptr)
                continue;
            for (auto var : {varA, varB}) {
                if (dornd_LS && !(var->isConstant())) {
                    double delta = Random::get_generator().Gaus() * var->getVal() * smear_scale;
                    random_smear(var, delta);
                }
                configure_error(var, true);
                var->setConstant(0);
            }
        }
        for (int ind = maxi + 1; ind < 6; ++ind) {
            RooRealVar *varA = CastFromList<RooRealVar>(argli, 2 * ind);
            RooRealVar *varB = CastFromList<RooRealVar>(argli, 2 * ind + 1);
            for (auto var : {varA, varB}) {
                if (var != nullptr) {
                    var->setVal(0.);
                    var->setConstant(1);
                }
            }
        }
        if (dornd_LS) {
            if (((RooAbsReal &)(*argli)[argli->getSize() - 3]).getVal() == MODELINDEPENDENT) {
                std::cout << "do rnd for Spline" << std::endl;
                int noffset = 12;
                int nsp = NUMSP;
                for (int i = 0; i < nsp; ++i) {
                    RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(noffset + i * 3));
                    RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(noffset + i * 3 + 1));

                    if (!(var->isConstant())) {
                        double delta = Random::get_generator().Gaus() * var->getVal() * smear_scale;
                        random_smear(var, delta);
                        configure_error(var, false);
                    }

                    if (!(var1->isConstant())) {
                        double delta =
                            Random::get_generator().Gaus() * var1->getVal() * smear_scale;
                        random_smear(var1, delta);
                        configure_error(var1, false);
                    }
                }
            }
        }
    }

    void floatList(TList *fixedPars) {
        TIterator *coefIter = fixedPars->MakeIterator();
        while (RooRealVar *tmp = (RooRealVar *)coefIter->Next()) {
            std::cout << tmp->GetName() << std::endl;
            tmp->setConstant(0);
        }
    }

    void FixLPar(RooArgList *argli, int num = 7) {
        for (int ind = 0; ind <= num; ++ind) {
            RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
            var->setConstant(1);
            RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
            var1->setConstant(1);
        }
    }

    void FloatPar(RooArgList *argli, int index) {
        // Get the spin
        RooAbsReal *spin_var = CastFromList<RooAbsReal>(argli, argli->getSize() - 2);
        assert(spin_var);
        int spin = static_cast<int>(spin_var->getVal());

        // Check if the index is valid
        if (index > 5 || (spin == 1 && index > 3))
            return;

        RooRealVar *varA = CastFromList<RooRealVar>(argli, 2 * index);
        RooRealVar *varB = CastFromList<RooRealVar>(argli, 2 * index + 1);
        for (auto var : {varA, varB}) {
            if (var != nullptr) {
                var->setError(0.01);
                var->setConstant(0);
            }
        }
    }

    void ResetPar(RooArgList *argli) {
        int J = ((RooAbsReal &)(*argli)[argli->getSize() - 2]).getVal();
        int ind(0);
        for (int S = abs(J - 2); S <= J + 2; S += 2) {
            for (int L = S - 1; L <= S + 1; L += 2) {
                RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
                var->setVal(var->getVal() * sqrt(2. / ((double)S + 1.)));
                RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
                var1->setVal(var1->getVal() * sqrt(2. / ((double)S + 1.)));
                ind++;
            }
        }
    }

    // fix the lowest L to reference for Spline fit
    void FixZPar(RooArgList *argli) {
        int spin = ((RooAbsReal &)(*argli)[argli->getSize() - 2]).getVal();
        for (int ind = 0; ind <= 3; ++ind) {
            RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
            var->setError(0.01);
            var->setConstant(0);
            RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
            var1->setError(0.01);
            var1->setConstant(0);
        }
        // fix lowest L to coupling to (1,0)
        {
            int index = -1;
            RooAbsReal *parity_variable = CastFromList<RooAbsReal>(argli, argli->getSize() - 1);
            int parity = static_cast<int>(parity_variable->getVal());

            const bool is_1_half_plus = (spin == 1 && parity > 0);
            const bool is_3_half_plus = (spin == 3 && parity > 0);
            const bool is_5_half_minus = (spin == 5 && parity < 0);
            const bool is_7_half_plus = (spin == 7 && parity > 0);

            const bool is_3_half_minus = (spin == 3 && parity < 0);
            const bool is_5_half_plus = (spin == 5 && parity > 0);
            const bool is_7_half_minus = (spin == 7 && parity < 0);
            const bool is_1_half_minus = (spin == 1 && parity < 0);

            if (is_1_half_plus || is_3_half_plus || is_5_half_minus || is_7_half_plus)
                index = 1;
            else if (is_3_half_minus || is_5_half_plus || is_7_half_minus)
                index = 2;
            else if (is_1_half_minus)
                index = 1;

            if (index > 0) {
                RooRealVar *real = CastFromList<RooRealVar>(argli, 2 * index);
                RooRealVar *imag = CastFromList<RooRealVar>(argli, 2 * index);
                real->setVal(1.0);
                real->setConstant(1);
                imag->setVal(0.0);
                imag->setConstant(1);
            }
        }
        if (abs(spin) <= 1) {
            int ind = 3;
            RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
            var->setVal(0.);
            var->setConstant(1);
            RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
            var1->setVal(0.);
            var1->setConstant(1);
        }
    }

    void ResetZPar(RooArgList *argli) {
        for (int i = 0; i <= 1; ++i) {
            RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(i));
            double v1 = var->getVal();
            if (fabs(v1) > 1.)
                var->setVal(1. / v1);
        }
    }

    void FixToZero(RooRealVar *var) {
        if (var != nullptr) {
            var->setVal(0);
            var->setConstant(1);
        }
    }

    bool CheckNotNullAndFloat(RooRealVar *var) { return (var != nullptr && !(var->isConstant())); }

    void FloatZPar(RooArgList *argli,
                   bool reset,
                   bool dornd_LS,
                   bool lowestL,
                   bool dornd_mass,
                   bool largeRandom) {
        // configure smear scale
        double scale_LS_coupling = (largeRandom) ? 0.3 : 0.2; // helicity coupling
        double scale_KM_coupling = (largeRandom) ? 0.3 : 0.1; // K-matrix coupling
        double scale_mass = (largeRandom) ? 0.002 : 0.001;

        // Get the spin
        RooAbsReal *spin_var = CastFromList<RooAbsReal>(argli, argli->getSize() - 2);
        assert(spin_var);
        int spin = static_cast<int>(spin_var->getVal());

        // Configure the 4 LS couplings
        // Each coupling has real and imaginary part, 8 RooRealVar in total
        for (int index = 0; index < 8; index++) {
            RooRealVar *var = CastFromList<RooRealVar>(argli, index);
            if (reset)
                var->setVal(0.001);
            if (CheckNotNullAndFloat(var)) {
                if (dornd_LS) {
                    // Threshold value to be at least 0.5
                    double value = std::max(0.5, std::abs(var->getVal()));
                    double scale = scale_LS_coupling * value;
                    double delta = Random::get_generator().Gaus() * scale;
                    random_smear(var, delta);
                }
                configure_error(var, true);
            }
    }

        if (dornd_mass) {
            RooRealVar *var_mass = CastFromList<RooRealVar>(argli, 12);
            // Randomize bare mass in K-matrix
            if (CheckNotNullAndFloat(
                    var_mass)) { // var_mass is nullptr if non-resonant or it is RooFormulaVar
                double delta = Random::get_generator().Gaus() * scale_mass;
                random_smear(var_mass, delta);
                configure_error(var_mass, false);
            }

            // Randomize K-matrix couplings
            for (int index = 8; index < 12; index++) {
                RooRealVar *var_KM_coupling = CastFromList<RooRealVar>(argli, index);
                if (CheckNotNullAndFloat(var_KM_coupling)) {
                    // Randomize the coupling with minimum value of Gaussian width
                    // Threshold value to be at least 1.0
                    double value = std::max(1.0, std::abs(var_KM_coupling->getVal()));
                    double scale = scale_KM_coupling * value;
                    double delta = Random::get_generator().Gaus() * scale;
                    // Avoid overly large deltas
                    delta = std::max(delta, -5 * scale_KM_coupling);
                    delta = std::min(delta, 5 * scale_KM_coupling);
                    random_smear(var_KM_coupling, delta);
                    configure_error(var_KM_coupling, false);
                }
            }
        }

        // Only float the helicity couplings for lowest orbital angular momentum L
        if (lowestL) {
            // index = 0 is the B->(Z K) decay coupling, index = 3 is the Z->(jpsi p) decay coupling
            for (int index : {0, 3}) {
                RooRealVar *varA = CastFromList<RooRealVar>(argli, 2 * index);
                RooRealVar *varB = CastFromList<RooRealVar>(argli, 2 * index + 1);
                for (auto *var : {varA, varB})
                    FixToZero(var);
            }

            int index = -1;

            // Dependent on spin parity of particle
            RooAbsReal *parity_var = CastFromList<RooAbsReal>(argli, argli->getSize() - 1);
            assert(parity_var);
            int parity = parity_var->getVal();
            const bool is_3_half_minus = (spin == 3 && parity < 0);
            const bool is_5_half_plus = (spin == 5 && parity > 0);
            const bool is_7_half_minus = (spin == 7 && parity < 0);
            const bool is_1_half_minus = (spin == 1 && parity < 0);

            if (is_3_half_minus || is_5_half_plus || is_7_half_minus)
                index = 1;
            else if (is_1_half_minus)
                index = 2;

            if (index >= 0) {
                RooRealVar *varA = CastFromList<RooRealVar>(argli, 2 * index);
                RooRealVar *varB = CastFromList<RooRealVar>(argli, 2 * index + 1);
                for (auto *var : {varA, varB})
                    FixToZero(var);
            }
        }

        // Set non-related helicity couplings to 0 for S=1/2 waves
        if (abs(spin) <= 1) {
            // only affects the coupling parameters at [6, 7]
            for (int index = 6; index < 8; index++) {
                RooRealVar *var = CastFromList<RooRealVar>(argli, index);
                FixToZero(var);
            }
        }

        // Randomize the SplineZ parameters
        RooAbsReal *type_var = CastFromList<RooAbsReal>(argli, argli->getSize() - 3);
        assert(type_var);
        if (dornd_LS && type_var->getVal() == MODELINDEPENDENT) {
            std::cout << "do rnd for SplineZ" << std::endl;
            int noffset = 8;
            int nsp = NUMSPZ;
            for (int index = 0; index < nsp; index++) {
                RooRealVar *varA = CastFromList<RooRealVar>(argli, noffset + index * 3);
                RooRealVar *varB = CastFromList<RooRealVar>(argli, noffset + index * 3 + 1);
                for (auto *var : {varA, varB}) {
                    if (CheckNotNullAndFloat(var)) {
                        double delta =
                            Random::get_generator().Gaus() * var->getVal() * scale_LS_coupling;
                        random_smear(var, delta);
                        configure_error(var, false);
                    }
                }
            }
        }
    }

    void FloatRGPar(RooArgList *argli, bool dornd, bool dornd_crazy) {
        if (!dornd)
            return; // Do nothing if we don't randomize

        double smear_scale = dornd_crazy ? 0.2 : 0.1;
        for (int i = 0; i < argli->getSize(); i++) {
            RooRealVar *variable = CastFromList<RooRealVar>(argli, i);
            if (CheckNotNullAndFloat(variable)) {
                // Threshold value to be at least 1.0
                double value = std::max(1.0, std::abs(variable->getVal()));
                double scale = smear_scale * value;
                double delta = Random::get_generator().Gaus() * scale;
                random_smear(variable, delta);
                configure_error(variable, false);
            }
        }
    }
} // namespace Parameter

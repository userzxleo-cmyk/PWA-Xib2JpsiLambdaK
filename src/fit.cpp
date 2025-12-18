// std library
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

// ROOT Minuit
#include "Minuit2/FunctionMinimum.h"
#include "TPluginManager.h"
#include "TSystem.h"

// Core ROOT classes
#include "TFile.h"
#include "TTree.h"

// RooFit classes
#include "RooAddition.h"
#include "RooConstVar.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooFormulaVar.h"
#include "RooMinimizer.h"
#include "RooRealConstant.h"
#include "RooRealVar.h"

// Custom classes
#include "RooTDalitz/Minuit3Minimizer.h"
#include "RooTDalitz/RooDalitzAmplitude.h"
#include "RooTDalitz/RooFGaussian.h"
#include "RooTDalitz/memory.h"

namespace Fitter {
    RooAddition
    ConstrMW(RooArgSet &setdlz, TString constraint_file = "errlist", bool setmean = false) {
        RooArgSet sumset;
        char varname[100];
        double mean1, mean2, err1, err2, r1, r2;
        std::ifstream in;

        auto &memory_manager = Memory::get_manager<RooFGaussian>();

        in.open(constraint_file);

        int isys(0);
        while (1) {
            in >> varname >> mean1 >> mean2 >> err1 >> err2 >> r1 >> r2;
            std::cout << varname << mean1 << mean2 << err1 << err2 << r1 << r2 << std::endl;
            if (!in.good())
                break;
            std::cout << "float " << varname << " " << mean1 << " " << mean2 << " " << err1 << " "
                      << err2 << std::endl;
            RooRealVar *var = (RooRealVar *)setdlz.find(varname);
            if (var != NULL) {
                var->setConstant(0);
                //      var->setConstant(1);
                var->setRange(r1, r2);
                if (setmean)
                    var->setVal((mean1 + mean2) / 2.);
                if (var->getVal() < r1 * 1.00001)
                    var->setVal(r1 * 1.00001);
                if (var->getVal() > r2 * 0.99999)
                    var->setVal(r2 * 0.99999);
                TString varname_str = TString(varname) + "_cons";
                RooFGaussian *constraint = memory_manager.create(varname_str,
                                                                 "",
                                                                 *var,
                                                                 RooRealConstant::value(mean1),
                                                                 RooRealConstant::value(err1),
                                                                 RooRealConstant::value(mean2),
                                                                 RooRealConstant::value(err2));
                constraint->Print("V");
                sumset.add(*constraint);
            }
            isys++;
        }
        sumset.Print("V");
        RooAddition sumvar("sumvar", "", sumset);
        return sumvar;
    }

    void setupMyMinuitMinimizer() {
        gInterpreter->AddIncludePath(HEADER_DIR);
        gInterpreter->Declare("#include \"Minuit3Minimizer.h\"");
        gSystem->AddDynamicPath(MYLIB_DIR);
        gSystem->Load("libMinuit3Minimizer");
        // Use Minuit3 to get better logging for first derivatives
        gPluginMgr->AddHandler("ROOT::Math::Minimizer",
                               "MyMinuitMinimizer",
                               "ROOT::Minuit2::Minuit3Minimizer",
                               "Minuit3Minimizer",
                               "Minuit3Minimizer(const char *)");
    }

    void storeFitHistory(const RooMinimizer &minimizer, const TString filepath) {

        // Try to get the Minuit3Minimizer which exposes gradient history information
        // Do nothing if the minimizer is not Minuit3Minimizer
        const auto minuit = dynamic_cast<const ROOT::Minuit2::Minuit3Minimizer *>(
            minimizer.fitter()->GetMinimizer());
        if (minuit == nullptr) {
            std::cout << "Cannot cast to Minuit3" << std::endl;
            std::cout << "Unable to store gradients!" << std::endl;
            return;
        }

        // Get the fit history
        const auto *minima = minuit->Minima();
        const auto &states = minima->States();
        const auto &parameters = minima->UserParameters().Parameters();
        const auto &transformer = minima->UserState().Trafo();

        // Get number of parameters and their names
        size_t nParameters = parameters.size();
        std::vector<TString> names;
        std::transform(
            parameters.begin(),
            parameters.end(),
            std::back_inserter(names),
            [](const ROOT::Minuit2::MinuitParameter &par) { return par.GetName().c_str(); });

        // Open file and create trees
        TFile file(filepath, "recreate");
        file.cd();
        TTree values_tree("value", "True value");
        TTree internal_values_tree("internal_value", "Internal value seen by Minuit.");
        TTree external_gradients_tree("external_gradient",
                                      "Gradients of parameters in external form.");
        TTree gradients_tree("first_derivative", "Gradients of parameters in internal form.");
        TTree second_derivatives_tree("second_derivative", "Second derivatives");
        TTree step_sizes_tree("step_size", "Finite difference step size");
        TTree history_tree("fit_history", "History of EDM, FCN and nFCN");

        std::vector<double> values_vector(nParameters);
        std::vector<double> internal_values_vector(nParameters);
        std::vector<double> external_gradients_vector(nParameters);
        std::vector<double> gradients_vector(nParameters);
        std::vector<double> second_derivatives_vector(nParameters);
        std::vector<double> step_sizes_vector(nParameters);

        // Define std::array of pointers for easier iteration
        std::array<TTree *, 7> trees = {&values_tree,
                                        &internal_values_tree,
                                        &external_gradients_tree,
                                        &gradients_tree,
                                        &second_derivatives_tree,
                                        &step_sizes_tree,
                                        &history_tree};

        // Initialize the branches in parameter trees
        for (size_t i = 0; i < nParameters; i++) {
            values_tree.Branch(names[i], &(values_vector[i]), names[i] + "/D");
            internal_values_tree.Branch(names[i], &(internal_values_vector[i]), names[i] + "/D");
            external_gradients_tree.Branch(
                names[i], &(external_gradients_vector[i]), names[i] + "/D");
            gradients_tree.Branch(names[i], &(gradients_vector[i]), names[i] + "/D");
            second_derivatives_tree.Branch(
                names[i], &(second_derivatives_vector[i]), names[i] + "/D");
            step_sizes_tree.Branch(names[i], &(step_sizes_vector[i]), names[i] + "/D");
        }

        // Initlialize the branches in the history tree
        double FCN, EDM;
        unsigned int nFCN;
        history_tree.Branch("FCN", &FCN, "FCN/D");
        history_tree.Branch("EDM", &EDM, "EDM/D");
        history_tree.Branch("nFCN", &nFCN, "nFCN/i");

        // Record the history into the TTrees
        for (const auto &state : states) {
            const auto &values = state.Parameters().Vec();
            const auto &first_derivatives = state.Gradient().Grad();
            const auto &second_derivatives = state.Gradient().G2();
            const auto &step_sizes = state.Gradient().Gstep();

            // Record one minimization step
            for (size_t i = 0; i < nParameters; i++) {
                values_vector[i] = transformer.Int2ext(i, values[i]);
                internal_values_vector[i] = values[i];
                external_gradients_vector[i] = transformer.DInt2Ext(i, first_derivatives[i]);
                gradients_vector[i] = first_derivatives[i];
                second_derivatives_vector[i] = second_derivatives[i];
                step_sizes_vector[i] = step_sizes[i];
            }
            FCN = state.Fval();
            EDM = state.Edm();
            nFCN = state.NFcn();

            // Fill the information into the trees
            for (auto *tree : trees) {
                tree->Fill();
            }
        }
        // Write the trees to the file
        // And close the file
        for (auto *tree : trees) {
            tree->Write();
        }
        file.Close();
    }

    double calculate_alpha_factor(RooDataSet *data_fit) {
        double sum_w(0.);
        double sum_w2(0.);
        for (int i = 0; i < data_fit->numEntries(); ++i) {
            data_fit->get(i);
            double nsw = data_fit->weight();
            sum_w += nsw;
            sum_w2 += pow(nsw, 2);
        }
        const double alpha = sum_w / sum_w2;

        std::cout.precision(12);
        const double sum_data_weights = data_fit->sumEntries();

        std::cout << "Sum of weights in data      : " << sum_w << std::endl;
        std::cout << "With RooDataSet->sumEntries : " << sum_data_weights << std::endl;
        std::cout << "Alpha to factor the -2*lnL  : " << alpha << std::endl;

        return alpha;
    }

    void run_minimizer(const double alpha,
                       const RooAddition &conMWterm,
                       const RooAbsReal *nll0,
                       RooFormulaVar *nll,
                       const TString &fit_result_path,
                       const TString &history_path,
                       const size_t maxCalls = 300000,
                       bool refit = false,
                       bool do_hesse = false,
                       RooArgSet *minosList = nullptr) {
        // Printout some info
        printf("Before Fit\t||\tValue of unconstrained NLL: %.8f\n",
               (nll0->getVal() - NLL_SHIFT) * alpha);
        printf("Before Fit\t||\tParameter constraint term : %.8f\n", conMWterm.getVal());
        printf("Before Fit\t||\tTotal NLL value           : %.8f\n", nll->getVal());

        setupMyMinuitMinimizer(); // Use a modified Minuit2 minimizer to store gradients

        RooMinimizer m(*nll);
        m.setStrategy(0);
        m.setEps(3.0);
        m.setMaxFunctionCalls(maxCalls);
        m.setMinimizerType("MyMinuitMinimizer");
        m.setPrintLevel(1);
        m.setErrorLevel(0.5);
        m.setVerbose(0);
        int fit_status = m.migrad();

        // do 1 more fit if initial fit did not converge
        if (refit && fit_status != 0)
            m.migrad();
        // Do HESSE
        if (do_hesse)
            m.hesse();
        // Do MINOS
        if (minosList != nullptr) {
            if (!do_hesse)
                m.hesse(); // Call HESSE if not already called
            m.minos(*minosList);
        }

        // Printout some info
        printf("After Fit\t||\tValue of unconstrained NLL: %.7f\n",
               (nll0->getVal() - NLL_SHIFT) * alpha);
        printf("After Fit\t||\tParameter constraint term : %.7f\n", conMWterm.getVal());
        printf("After Fit\t||\tTotal NLL value           : %.7f\n", nll->getVal());

        // Store the fit results
        if (fit_result_path.Length() != 0) {
            std::cout << "Storing fit results to path : " << fit_result_path << std::endl;
            TFile *result_file = new TFile(fit_result_path, "recreate");
            auto fit_result = m.save();
            fit_result->Write();
            result_file->Close();
            delete fit_result;
        } else {
            std::cout << "Fit result path not parsed. Skipping store fit results..." << std::endl;
        }

        // Store the fit history
        if (history_path.Length() != 0) {
            std::cout << "Storing fit history to path : " << history_path << std::endl;
            storeFitHistory(m, history_path);
        } else {
            std::cout << "History path not parsed. Skipping store fit history..." << std::endl;
        }
    }

    template <bool do_fit>
    double execute(RooDalitzAmplitude *amplitude,
                   RooArgList *obs,
                   RooDataSet *data_fit,
                   TString fit_result_path,
                   TString history_path,
                   TString constraint_file = "errlist",
                   size_t maxCalls = 300000,
                   bool refit = false,
                   bool do_hesse = false,
                   RooArgSet *minosList = nullptr) {

        auto fit_parameters = std::unique_ptr<RooArgSet>(amplitude->getParameters(*data_fit));

        // Create the raw NLL
        // one_dataset is not actually used to calculate the NLL, but to "trick" RooFit into
        // creating a log-likelihood object The datasets have been loaded during the construction of
        // the RooDalitzAmplitude
        RooDataSet one_dataset("One data", "", *obs);
        one_dataset.add(*(data_fit->get(0)));
        auto nll0 = std::unique_ptr<RooAbsReal>(amplitude->createSUM(one_dataset));
        // Setup the NLL formula
        const double alpha = calculate_alpha_factor(data_fit);
        RooAddition conMWterm = ConstrMW(*fit_parameters, constraint_file);
        RooConstVar alpha_var("alpha", "alpha", alpha);
        RooConstVar nll_shift_var("nll_shift", "nll_shift", NLL_SHIFT);
        auto nll = std::make_unique<RooFormulaVar>(
            "nll", "(@0-@3)*@1+@2", RooArgSet(*nll0, alpha_var, conMWterm, nll_shift_var));

        std::cout << "Printing parameters before first nll calculation..." << std::endl;
        fit_parameters->Print("V");
        if constexpr (do_fit)
            run_minimizer(alpha,
                          conMWterm,
                          nll0.get(),
                          nll.get(),
                          fit_result_path,
                          history_path,
                          maxCalls,
                          refit,
                          do_hesse,
                          minosList);

        const double NLL_value = 2.0 * nll->getVal();
        printf("Total log-likelihood : %.7f\n", NLL_value);

        return NLL_value;
    }

    double doFit(RooDalitzAmplitude *amplitude,
                 RooArgList *obs,
                 RooDataSet *data_fit,
                 TString fit_result_path,
                 TString history_path,
                 TString constraint_file = "errlist",
                 size_t maxCalls = 300000,
                 bool refit = false,
                 bool do_hesse = false,
                 RooArgSet *minosList = nullptr) {
        // Do the fit
        return execute<true>(amplitude,
                             obs,
                             data_fit,
                             fit_result_path,
                             history_path,
                             constraint_file,
                             maxCalls,
                             refit,
                             do_hesse,
                             minosList);
    }

    double calculate_nll(RooDalitzAmplitude *amplitude,
                         RooArgList *obs,
                         RooDataSet *data_fit,
                         TString fit_result_path,
                         TString history_path,
                         TString constraint_file = "errlist",
                         size_t maxCalls = 300000,
                         bool refit = false,
                         bool do_hesse = false,
                         RooArgSet *minosList = nullptr) {
        // Just calculate NLL
        return execute<false>(amplitude,
                              obs,
                              data_fit,
                              fit_result_path,
                              history_path,
                              constraint_file,
                              maxCalls,
                              refit,
                              do_hesse,
                              minosList);
    }
} // namespace Fitter

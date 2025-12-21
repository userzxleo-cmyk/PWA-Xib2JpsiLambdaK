// C++ std library
#include <vector>
#include <memory>

// ROOT classes
#include "TFile.h"
#include "TTree.h"

// RooFit classes
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooAbsData.h"
#include "RooFitResult.h"

// Custom classes
#include "RooTDalitz/memory.h"
#include "RooTDalitz/execute.h"
#include "RooTDalitz/config.h"
#include "RooTDalitz/fit.h"
#include "RooTDalitz/random.h"
#include "RooTDalitz/parameter.h"
#include "RooTDalitz/Resonances.h"
#include "RooTDalitz/kernel_calEva.h"
#include "RooTDalitz/RooDalitzAmplitude.h"

namespace Execution{
    std::unique_ptr<RooArgList> get_observables(){
        // Use static global memory manager
        // Persist the variables outside the scope of this function
        auto& memory_manager = Memory::get_manager<RooRealVar>();
        std::vector<RooRealVar*> base_observables = {
            memory_manager.create("mpk","",1.0,2.6),
            memory_manager.create("mjpsip","",4.,5.5),
            memory_manager.create("mjpsik","",3.2,4.7),
            memory_manager.create("cosTheta_L","cosTheta_L",-1,1),
            memory_manager.create("cosTheta_Jpsi","cosTheta_Jpsi",-1,1),
            memory_manager.create("cosTheta_Lb","cosTheta_Lb",-1,1),
            memory_manager.create("Z_cosTheta_Lb","Z_cosTheta_Lb",-1,1),
            memory_manager.create("Z_cosTheta_Z","Z_cosTheta_Z",-1,1),
            memory_manager.create("Z_cosTheta_Jpsi","Z_cosTheta_Jpsi",-1,1),
            memory_manager.create("cosTheta_p","cosTheta_p",-1,1),
            memory_manager.create("phipi","phipi",-TMath::Pi(), TMath::Pi()),
            memory_manager.create("phiMu","phiMu",-TMath::Pi(), TMath::Pi()),
            memory_manager.create("Z_phiZ","Z_phiZ",-TMath::Pi(), TMath::Pi()),
            memory_manager.create("Z_phiJpsi","Z_phiJpsi",-TMath::Pi(), TMath::Pi()),
            memory_manager.create("Z_phiMu","Z_phiMu",-TMath::Pi(), TMath::Pi()),
            memory_manager.create("alpha_Mu","alpha_Mu",-TMath::Pi(), TMath::Pi()),
            // memory_manager.create("X_phiX","X_phiX",-TMath::Pi(), TMath::Pi()),
            // memory_manager.create("X_phiJpsi","X_phiJpsi",-TMath::Pi(), TMath::Pi()),
            // memory_manager.create("X_phiMu","X_phiMu",-TMath::Pi(), TMath::Pi()),
            // memory_manager.create("X_alpha_Mu","X_alpha_Mu",-TMath::Pi(), TMath::Pi()),
            // memory_manager.create("X_cosTheta_Lb","X_cosTheta_Lb",-1,1),
            // memory_manager.create("X_cosTheta_X","X_cosTheta_X",-1,1),
            // memory_manager.create("X_cosTheta_Jpsi","X_cosTheta_Jpsi",-1,1),
            // memory_manager.create("X_cosTheta_p","X_cosTheta_p",-1,1)
        };
        auto observables = std::make_unique<RooArgList>();
        for (auto* obs : base_observables){
            observables->add(*obs);
        }
        return observables;
    }

    // Create the amplitude model
    std::shared_ptr<RooDalitzAmplitude> get_amplitude(
        const Arguments& arguments,
        RooArgList& observables,
        RooDataSet& data_fit){
        // Use static global memory manager
        // Persist the variables outside the scope of this function
        auto& memory_manager = Memory::get_manager<RooRealVar>();
        // Create the BlattWeisskopf variables
        auto FFB_var = memory_manager.create("FFB_var","FFB_var",3.0f, 1.f, 5.f);
        auto FFR_var = memory_manager.create("FFR_var","FFR_var",3.0f, 1.f, 5.f);
        // We only float these for the Blatt Weisskopf systematics
        FFB_var->setConstant(arguments.fixed_BlattWeisskopf);
        FFR_var->setConstant(arguments.fixed_BlattWeisskopf);
        FFB_var->setError(0.1);
        FFR_var->setError(0.1);

        // Create Pc width variables
        auto widthUpperLimit_var = memory_manager.create("widthUpperLimit_var", "widthUpperLimit_var", arguments.widthUpperLimit);
        auto widthLowerLimit_var = memory_manager.create("widthLowerLimit_var", "widthLowerLimit_var", arguments.widthLowerLimit);
        auto CalFWHM_nbin_var = memory_manager.create("CalFWHM_nbin_var", "CalFWHM_nbin_var", 1000.0f);
        auto CalFWHM_range_var = memory_manager.create("CalFWHM_range_var", "CalFWHM_range_var", 5 * arguments.scale_gJpsip_4457);
        widthUpperLimit_var->setConstant(1);
        widthLowerLimit_var->setConstant(1);
        CalFWHM_nbin_var->setConstant(1);
        CalFWHM_range_var->setConstant(1);

        auto amplitude = std::make_shared<RooDalitzAmplitude>(
            "amplitude",
            "",
            observables,
            arguments.listL,
            arguments.listZ,
            arguments.listX,
            arguments.ratioList,
            *FFR_var,
            *FFB_var,
            arguments.mcTuplePath,
            data_fit,
            *widthUpperLimit_var,
            *widthLowerLimit_var,
            *CalFWHM_nbin_var,
            *CalFWHM_range_var
        );

        return amplitude;
    }

    std::shared_ptr<RooDataSet> get_data_fit(std::unique_ptr<RooArgList>& observables, TString dataTuplePath){
        // Create additional variables beyond the base observables
        auto& memory_manager = Memory::get_manager<RooRealVar>();
        RooRealVar * index = memory_manager.create("index","index",0);
        RooRealVar * nsig_sw = memory_manager.create("nsig_sw","nsig_sw",0);

        // Create the observables + weights list
        // Then read the data tree
        std::unique_ptr<RooArgSet> observables_with_weights = std::make_unique<RooArgSet>(*observables); // Use copy constructor first
        observables_with_weights->add(*nsig_sw);

        TFile data_file(dataTuplePath, "READ");
        TTree * data_tree = data_file.Get<TTree>("DecayTree");
        // Use vector storage for the data so that TTree is copied to memory
        RooAbsData::setDefaultStorageType(RooAbsData::Vector);
        std::unique_ptr<RooDataSet> temp_data = std::make_unique<RooDataSet>(
            "temp_data",
            "",
            *observables_with_weights,
            RooFit::Import(*data_tree),
            RooFit::Cut("mpk>1.0778425&&mpk<2.5227&&mjpsip>4.0351721&&mjpsip<5.4800296"),
            RooFit::WeightVar(*nsig_sw)
        );
        data_file.Close();

        // Create the index dataset
        std::unique_ptr<RooDataSet> index_dataset = std::make_unique<RooDataSet>("index_dataset", "", RooArgSet(*index));
        for (int i = 0; i < temp_data->numEntries(); ++i){
            index->setVal(static_cast<double>(i));
            index_dataset->add(RooArgSet(*index));
        }
        temp_data->merge(index_dataset.get());

        // Create the fit dataset
        // Has to be done this way, otherwise the weight variable setting with be reset
        observables_with_weights->add(*index);
        std::shared_ptr<RooDataSet> data_fit = std::make_shared<RooDataSet>(
            "data_fit",
            "",
            temp_data.get(),
            *observables_with_weights,
            nullptr,
            "nsig_sw"
        );
        data_fit->Print("V");

        std::cout << "Sum of weights in data after merge: " << data_fit->sumEntries() << std::endl;

        return data_fit;
    }

    void configure_from_roofitresult(std::unique_ptr<RooArgSet>& fit_variables, const TString& fitresultPath) {
        TFile roofitresult_file(fitresultPath, "READ");
        std::unique_ptr<RooFitResult> pars_values(roofitresult_file.Get<RooFitResult>("nll"));
        roofitresult_file.Close();
        for ( auto iter = pars_values->constPars().begin(); iter != pars_values->constPars().end(); iter++ ) {
            const RooRealVar * var = dynamic_cast<const RooRealVar *>(*iter);
            TString varName(var->GetName());
            RooRealVar * varToSet = dynamic_cast<RooRealVar *>( fit_variables->find(varName) );
            if ( varToSet != nullptr ){
            var->hasMin() ? varToSet->setMin( var->getMin() ) : varToSet->removeMin();
            var->hasMax() ? varToSet->setMax( var->getMax() ) : varToSet->removeMax();
            varToSet->setVal( var->getVal() );
            if ( var->hasError() ) varToSet->setError( var->getError() );
            }
            printf("Setting the value for: %s\t to %.3e\n", var->GetName(), var->getVal());
        }

        for ( auto iter = pars_values->floatParsFinal().begin(); iter != pars_values->floatParsFinal().end(); iter++ ) {
            const RooRealVar * var = dynamic_cast<const RooRealVar *>(*iter);
            TString varName(var->GetName());
            RooRealVar * varToSet = dynamic_cast<RooRealVar *>( fit_variables->find(varName) );
            var->hasMin() ? varToSet->setMin( var->getMin() ) : varToSet->removeMin();
            var->hasMax() ? varToSet->setMax( var->getMax() ) : varToSet->removeMax();
            varToSet->setVal( var->getVal() );
            varToSet->setError( var->getError() );
            printf("Setting the value for: %s\t to %.3e\n", var->GetName(), var->getVal());
        }
    }

    void configure_uncertainties(std::unique_ptr<RooArgSet>& fit_variables, const bool away_from_limit) {
        // Set the uncertainty to be a minimum of 0.05% of value
        for ( auto var = fit_variables->begin(); var != fit_variables->end(); var++ ) {
            auto * parameter = dynamic_cast<RooRealVar*>(*var);
            if ( parameter->isConstant() ) continue;
            if ( parameter->getError() < std::abs( 0.0005 * parameter->getVal() ) ) {
                    parameter->setError( 0.0005 * std::abs( parameter->getVal() ) );
            }
            if ( parameter->hasMin() && parameter->hasMax() &&
                parameter->getError() > 0.2 * (parameter->getMax() - parameter->getMin()) ){
                    parameter->setError(0.2 * (parameter->getMax() - parameter->getMin()));
            }
            
            if ( away_from_limit && parameter->hasMin() && 
                ( parameter->getVal() - parameter->getError() <= parameter->getMin() ) ) {
                    parameter->setVal( parameter->getMin() + 1.1 * parameter->getError() );
            }

            if ( away_from_limit && parameter->hasMax() && 
                ( parameter->getVal() + parameter->getError() >= parameter->getMax() ) ) {
                    parameter->setVal( parameter->getMax() - 1.1 * parameter->getError() );
            }
        }
    }

    void randomize_and_fix_Lambda_parameters(RooArgList * resonance, const bool randomLs, bool dornd_LS_crazy = false){
        int spin = dynamic_cast<RooAbsReal*>(resonance->at(resonance->getSize() - 2))->getVal();
        constexpr int max_couplings = 6;
        const int nCouplings = (spin == 1) ? 4 : 6;
        const double smear_scale = dornd_LS_crazy ? 0.3 : 0.2;
        auto& generator = Random::get_generator();

        // Randomize the couplings
        for (int index = 0; index < 2 * nCouplings; index++){
            RooRealVar * var = dynamic_cast<RooRealVar *>(resonance->at(index));
            if (var != nullptr && !(var->isConstant()) ){
                if (randomLs){
                    // Threshold value to be at least 0.5
                    double value = std::max(0.5, std::abs(var->getVal()));
                    double scale = smear_scale * value;
                    double delta = generator.Gaus() * scale;
                    Parameter::random_smear(var, delta);
                }
                Parameter::configure_error(var, true);
            }
        }
        // Fix the couplings for 1/2 waves
        for (int index = 2 * nCouplings; index < 2 * max_couplings; index ++){
            RooRealVar * var = dynamic_cast<RooRealVar *>(resonance->at(index));
            if (var != nullptr){
                var->setVal(0.);
                var->setConstant(1);
            }
        }

        // Randomize spline
        const int lineshape = dynamic_cast<RooAbsReal*>(resonance->at(resonance->getSize() - 3))->getVal();
        const bool is_spline = ( lineshape == MODELINDEPENDENT );
        if (is_spline){
            const int start = 12;
            const int end = 12 + 3 * NUMSP;
            for (int index = start; index < end; index += 3){
                RooRealVar * varA = dynamic_cast<RooRealVar *>(resonance->at(index));
                RooRealVar * varB = dynamic_cast<RooRealVar *>(resonance->at(index + 1));
                for (auto var : {varA, varB}){
                    if (randomLs && var != nullptr && !(var->isConstant())){
                        // Threshold value to be at least 0.5
                        double value = std::max(0.5, std::abs(var->getVal()));
                        double scale = smear_scale * value;
                        double delta = generator.Gaus() * scale;
                        Parameter::random_smear(var, delta);
                        Parameter::configure_error(var, false);
                    }
                }
            }
        }
    }

    void configure_Lambda_parameters(TList * listL, const bool randomLs, const bool largeRandom){
        for ( auto resonanceL = listL->begin(); resonanceL != listL->end(); ++resonanceL ) {
            auto * list = dynamic_cast<RooArgList*>(*resonanceL);
            if ( list != nullptr ) randomize_and_fix_Lambda_parameters(list, randomLs, largeRandom);
        }
    }

    void configure_Pc_parameters(TList * listZ, const bool randomPc, const bool largeRandom){
        for ( auto resonanceZ = listZ->begin(); resonanceZ != listZ->end(); ++resonanceZ ) {
            auto * list = dynamic_cast<RooArgList*>(*resonanceZ);
            if ( list != nullptr ) Parameter::FloatZPar(list, false, randomPc, false, randomPc, largeRandom);
        }
    }

    void execute(Arguments& arguments, Config::Mode mode){
        // If we run in store weights mode but the paths are not provided, throw an error
        if (mode == Config::Mode::Store_Weights){
            if (arguments.dataWeightsPath.Length() == 0 || arguments.mcWeightsPath.Length() == 0){
                throw std::logic_error("Requested to store weights, but data weights path or mc weights path not parsed.\n");
            }
        }

        // Create the observables and dataset
        auto observables = get_observables();
        auto data_fit = get_data_fit(observables, arguments.dataTuplePath);
        // Create the amplitude model
        auto amplitude = get_amplitude(arguments, *observables, *data_fit);

        // Configure the fit parameters
        std::unique_ptr<RooArgSet> fit_variables(amplitude->getParameters(*data_fit));
        if (mode == Config::Mode::Fit || mode == Config::Mode::NLL){
            // Read from initial function file
            fit_variables->readFromFile(arguments.initial_func_path);

            // Configure the fit parameters
            configure_Lambda_parameters(arguments.listL, arguments.randomLs, arguments.largeRandom);
            configure_Pc_parameters(arguments.listZ, arguments.randomPc, arguments.largeRandom);
            Parameter::FloatRGPar(arguments.ratioList, arguments.randomPc, arguments.largeRandom);
            configure_uncertainties(fit_variables, arguments.away_from_limit);
        }
        else{
            // Read from RooFitResult in store weights mode
            // This is to prevent precision loss when reading from func file
            //  which converts double into low precision string
            configure_from_roofitresult(fit_variables, arguments.fitresultPath);
        }

        // Perform fit or NLL calculation if running in those modes
        if (mode == Config::Mode::Fit){
            // Perform the fit
            Fitter::doFit(
                amplitude.get(),
                observables.get(),
                data_fit.get(),
                arguments.fitresultPath,
                arguments.historyPath,
                arguments.errlist,
                arguments.maxCalls,
                arguments.refit,
                arguments.do_hesse,
                arguments.minosList
            );
            fit_variables->writeToFile(arguments.final_func_path);
        }
        else if (mode == Config::Mode::NLL){
            // Perform the NLL calculation
            Fitter::calculate_nll(
                amplitude.get(),
                observables.get(),
                data_fit.get(),
                arguments.fitresultPath,
                arguments.historyPath,
                arguments.errlist,
                arguments.maxCalls,
                arguments.refit,
                arguments.do_hesse,
                arguments.minosList
            );
        }

        // Store the weights
        // TODO: Need to clean up RooDalitzAmplitude to avoid copying before storing weights
        const bool has_data_weights_path = arguments.dataWeightsPath.Length() != 0;
        const bool has_mc_weights_path = arguments.mcWeightsPath.Length() != 0;
        if (has_data_weights_path && has_mc_weights_path){
            std::cout << "Calculating Amp^2 weights." << std::endl;
            auto copy = std::make_unique<RooDalitzAmplitude>(*amplitude, "storeweights");
            copy->storeResonanceWeights(arguments.dataWeightsPath, arguments.mcWeightsPath);
        }
        else{
            std::cout << "Data weights path or mc weights path not parsed. Not storing per-event weights..." << std::endl;
        }
    }
} // namespace Execution

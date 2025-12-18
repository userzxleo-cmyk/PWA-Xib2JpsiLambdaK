#include "TPluginManager.h"
#include "TSystem.h"
#include "RooTDalitz/Minuit3Minimizer.h"
#include "Minuit2/FunctionMinimum.h"

#define iverbose 0
inline bool exists(const std::string &name)
{
    return (access(name.c_str(), F_OK) != -1);
    //  struct stat buffer;
    //  return (stat (name.c_str(), &buffer) == 0);
}

void random_smear(RooRealVar * variable, double delta){
    double initial_value = variable->getVal();
    double smeared_value = delta + initial_value;
    if (variable->hasMin() && smeared_value < variable->getMin()){
        double lower_limit = variable->getMin();
        double reflection = lower_limit - smeared_value;
        double reflected_value = lower_limit + reflection;
        smeared_value = reflected_value;
    }
    else if (variable->hasMax() && smeared_value > variable->getMax()){
        double upper_limit = variable->getMax();
        double reflection = smeared_value - upper_limit;
        double reflected_value = upper_limit - reflection;
        smeared_value = reflected_value;
    }
    variable->setVal(smeared_value);
}

void configure_RooRealVar(RooRealVar * variable, bool setError = false){
    if (setError) {
        variable->setError(0.1);
    }
    if (variable->hasMax() && (variable->getVal() > (variable->getMax() - 1.1 * variable->getError()))){
        variable->setVal( variable->getMax() - 1.1 * variable->getError() );
    }
    if (variable->hasMin() && (variable->getVal() < (variable->getMin() + 1.1 * variable->getError()))){
        variable->setVal( variable->getMin() + 1.1 * variable->getError() );
    }
}

//Randomize a parameter
void Randomize_par(RooRealVar &var)
{
        if(var.isConstant()) {
                std::cout << "Randomize failed due to constant parameter " << var.GetName() << std::endl;
                return;
        }
        var.setVal(g_rnd->Gaus() * var.getVal() * 0.2 + var.getVal());
        var.setError(0.01);
        if (var.hasMax() && (var.getVal() > (var.getMax() - 1.1 * var.getError()))){
            var.setVal( var.getMax() - 1.1 * var.getError() );
        }
        if (var.hasMin() && (var.getVal() < (var.getMin() + 1.1 * var.getError()))){
            var.setVal( var.getMin() + 1.1 * var.getError() );
        } 
        var.setConstant(0);
}
       
       


//set how many SL of Lambda to float
void SetLPar(RooArgList *argli, int maxind = 6, bool dornd_LS = false, bool fixfirst = false, bool dornd_LS_crazy = false)
{
    int spin = ((RooAbsReal &)(*argli)[argli->getSize() - 2]).getVal();
    int maxi = maxind - 1;

    if (spin == 1 && maxi > 3) maxi = 3;

    int mini = 0;
    if (fixfirst) mini = 1;

    const double smear_scale = (dornd_LS_crazy) ? 0.3 : 0.2;

    for (int ind = mini; ind <= maxi; ++ind)
    {
        RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
        RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
        if (var == NULL || var1 == NULL) continue;
        if (dornd_LS && !(var->isConstant())) {
            double delta = g_rnd->Gaus() * var->getVal() * smear_scale;
            random_smear(var, delta);
        }

        if (dornd_LS && !(var1->isConstant())) {
            double delta = g_rnd->Gaus() * var1->getVal() * smear_scale;
            random_smear(var1, delta);
        }

        configure_RooRealVar(var, true);
        configure_RooRealVar(var1, true);
        var->setConstant(0);
        var1->setConstant(0);
    }
    for (int ind = maxi + 1; ind < 6; ++ind)
    {
        RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
        var->setVal(0.);
        var->setConstant(1);
        RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
        var1->setVal(0.);
        var1->setConstant(1);
    }
    if (dornd_LS)
    {
        if (((RooAbsReal &)(*argli)[argli->getSize() - 3]).getVal() == MODELINDEPENDENT)
        {
            std::cout << "do rnd for Spline" << std::endl;
            int noffset = 12;
            int nsp = NUMSP;
            for (int i = 0; i < nsp; ++i)
            {
                RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(noffset + i*3));
                RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(noffset + i*3 + 1));

                if (!(var->isConstant())){
                    double delta = g_rnd->Gaus() * var->getVal() * smear_scale;
                    random_smear(var, delta);
                    configure_RooRealVar(var, false);
                }

                if (!(var1->isConstant())){
                    double delta = g_rnd->Gaus() * var1->getVal() * smear_scale;
                    random_smear(var1, delta);
                    configure_RooRealVar(var1, false);
                }
            }
        }
    }
}

void FixLPar(RooArgList *argli, int num = 7)
{
    //    int  spin = ((RooAbsReal&)(*argli)[argli->getSize()-2]).getVal();
    //    std::cout << spin << std::endl;
    for (int ind = 0; ind <= num; ++ind)
    {
        RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
        var->setConstant(1);
        RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
        var1->setConstant(1);
    }
}

void FloatPar(RooArgList *argli, int ind)
{
    if (ind > 5)
        return;
    int spin = ((RooAbsReal &)(*argli)[argli->getSize() - 2]).getVal();
    //    std::cout << spin << std::endl;
    if (spin == 1 && ind > 3)
        return;
    RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
    var->setError(0.01);
    var->setConstant(0);
    RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
    var1->setError(0.01);
    var1->setConstant(0);
}

void ResetPar(RooArgList *argli)
{
    int J = ((RooAbsReal &)(*argli)[argli->getSize() - 2]).getVal();
    int ind(0);
    for (int S = abs(J - 2); S <= J + 2; S += 2)
    {
        for (int L = S - 1; L <= S + 1; L += 2)
        {
            RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
            var->setVal(var->getVal() * sqrt(2. / ((double)S + 1.)));
            RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
            var1->setVal(var1->getVal() * sqrt(2. / ((double)S + 1.)));
            ind++;
        }
    }
}

//fix the lowest L to reference for Spline fit
void FixZPar(RooArgList *argli)
{
    bool lowestL = false;
    int spin = ((RooAbsReal &)(*argli)[argli->getSize() - 2]).getVal();
    //    std::cout << spin << std::endl;
    //  if(spin==1&&ind>3)  return;
    for (int ind = 0; ind <= 3; ++ind)
    {
        RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
        var->setError(0.01);
        var->setConstant(0);
        RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
        var1->setError(0.01);
        var1->setConstant(0);
    }
    //fix lowest L to coupling to (1,0)
    {
        int ind;
        int parity = (int)((RooAbsReal &)(*argli)[argli->getSize() - 1]).getVal();
        if ((spin == 1 && parity > 0) || (spin == 3 && parity > 0) || (spin == 5 && parity < 0) || (spin == 7 && parity > 0))
        {
            //id = 0, 3
            ind = 1;
        }
        else if ((spin == 3 && parity < 0) || (spin == 5 && parity > 0) || (spin == 7 && parity < 0))
        {
            //id = 0, 1, 3
            ind = 2;
        }
        else if ((spin == 1 && parity < 0))
        {
            //id = 0, 2, 3
            ind = 1;
        }
        RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
        var->setVal(1.);
        var->setConstant(1);
        RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
        var1->setVal(0.);
        var1->setConstant(1);
    }
    if (abs(spin) <= 1)
    {
        int ind = 3;
        RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
        var->setVal(0.);
        var->setConstant(1);
        RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
        var1->setVal(0.);
        var1->setConstant(1);
    }
}

void ResetZPar(RooArgList *argli)
{
    for (int i = 0; i <= 1; ++i)
    {
        RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(i));
        double v1 = var->getVal();
        if (fabs(v1) > 1.)
            var->setVal(1. / v1);
    }
}

void SetErrZPar(RooArgList *argli, bool lowestL = false)
{
    int spin = ((RooAbsReal &)(*argli)[argli->getSize() - 2]).getVal();
    //    std::cout << spin << std::endl;
    //  if(spin==1&&ind>3)  return;
    for (int ind = 0; ind <= 3; ++ind)
    {
        RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
        var->setError(0.01);
        var->setConstant(0);
        RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
        var1->setError(0.01);
        var1->setConstant(0);
    }
    if (lowestL)
    {
        for (int ind = 0; ind <= 3; ++ind)
        {
            if (ind == 1 || ind == 2)
                continue;
            RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
            var->setVal(0);
            var->setConstant(1);
            RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
            var1->setVal(0);
            var1->setConstant(1);
        }
        int ind;
        int parity = (int)((RooAbsReal &)(*argli)[argli->getSize() - 1]).getVal();
        //    if((spin==1&&parity>0)||(spin==3&&parity>0)||(spin==5&&parity<0)||(spin==7&&parity>0)) {
        //id = 0, 3

        if ((spin == 3 && parity < 0) || (spin == 5 && parity > 0) || (spin == 7 && parity < 0))
        {
            //id = 0, 1, 3
            ind = 1;
        }
        else if ((spin == 1 && parity < 0))
        {
            //id = 0, 2, 3
            ind = 2;
        }
        RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
        var->setVal(0.);
        var->setConstant(1);
        RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
        var1->setVal(0.);
        var1->setConstant(1);
    }
    if (abs(spin) <= 1)
    {
        int ind = 3;
        RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
        var->setVal(0.);
        var->setConstant(1);
        RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
        var1->setVal(0.);
        var1->setConstant(1);
    }
}

void FixToZero(RooRealVar * var){
    if (var != nullptr){
        var->setVal(0);
        var->setConstant(1);
    }
}

bool CheckNotNullAndFloat(RooRealVar * var){
    return (var != nullptr && !(var->isConstant()));
}

template <typename T>
T * CastFromList(RooArgList * arg_list, const unsigned index){
    return dynamic_cast<T*>(arg_list->at(index));
}

void FloatZPar(
    RooArgList *argli,
    bool reset = true,
    bool dornd_LS = false,
    bool lowestL = false,
    bool dornd_mass = false,
    bool dornd_crazy = false
)
{
    RooAbsReal * spin_var = CastFromList<RooAbsReal>(argli, argli->getSize() - 2);
    assert(spin_var);
    int spin = static_cast<int>(spin_var->getVal());
    double rnd_scale_LS_coupling = 0.2;
    double rnd_scale_mass_coupling = 0.1;
    double rnd_scale_mass = 0.001;
    if(dornd_crazy) {
            rnd_scale_LS_coupling = 0.3;
            rnd_scale_mass_coupling = 0.3;
            rnd_scale_mass = 0.002;
    }

    for (int ind = 0; ind <= 3; ++ind)
    {
        RooRealVar * var = CastFromList<RooRealVar>(argli, 2 * ind);
        RooRealVar * var1 = CastFromList<RooRealVar>(argli, 2 * ind+1);
        if (var == nullptr || var1 == nullptr) continue;

        if (reset){
            var->setVal(0.001);
            var1->setVal(0.001);
        }

        if (dornd_LS && !(var->isConstant())){
            double delta = g_rnd->Gaus() * var->getVal() * rnd_scale_LS_coupling;
            random_smear(var, delta);
        }

        if (dornd_LS && !(var1->isConstant())){
            double delta = g_rnd->Gaus() * var1->getVal() * rnd_scale_LS_coupling;
            random_smear(var1, delta);
        }
        configure_RooRealVar(var, true);
        configure_RooRealVar(var1, true);
    }
    //smear mass parameter
    if (dornd_mass) {
        RooRealVar *var_mass = CastFromList<RooRealVar>(argli, 12);
        if (CheckNotNullAndFloat(var_mass)){ // var_mass is nullptr if non-resonant or it is RooFormulaVar
            double delta = g_rnd->Gaus() * rnd_scale_mass;
            random_smear(var_mass, delta);
            configure_RooRealVar(var_mass, false);
        }

        size_t indices[4] = {8, 9, 10, 11};
        for (auto index : indices){
            RooRealVar *var_coupling = CastFromList<RooRealVar>(argli, index);
            if (CheckNotNullAndFloat(var_coupling)){
                double random_smear_scale = rnd_scale_mass_coupling * std::max(1.0, std::abs(var_coupling->getVal()));
                double delta = g_rnd->Gaus() * random_smear_scale;
                // Avoid overly large deltaning
                delta = std::max( delta, -3*rnd_scale_mass_coupling );
                delta = std::min( delta,  3*rnd_scale_mass_coupling );
                random_smear(var_coupling, delta);
                configure_RooRealVar(var_coupling, false);
            }
        }
    }
    if (lowestL)
    {
        for (int ind = 0; ind <= 3; ++ind)
        {
            if (ind == 1 || ind == 2)
                continue;
            RooRealVar *var = CastFromList<RooRealVar>(argli, 2 * ind);
            FixToZero(var);
            RooRealVar *var1 = CastFromList<RooRealVar>(argli, 2 * ind + 1);
            FixToZero(var1);
        }
        int ind = -1;
        RooAbsReal * parity_var = CastFromList<RooAbsReal>(argli, argli->getSize() - 1);
        assert(parity_var);
        int parity = parity_var->getVal();
        //    if((spin==1&&parity>0)||(spin==3&&parity>0)||(spin==5&&parity<0)||(spin==7&&parity>0)) {
        //id = 0, 3

        if ((spin == 3 && parity < 0) || (spin == 5 && parity > 0) || (spin == 7 && parity < 0))
        {
            //id = 0, 1, 3
            ind = 1;
        }
        else if ((spin == 1 && parity < 0))
        {
            //id = 0, 2, 3
            ind = 2;
        }
        if (ind >= 0)
        {
            RooRealVar *var = CastFromList<RooRealVar>(argli, 2 * ind);
            FixToZero(var);
            RooRealVar *var1 = CastFromList<RooRealVar>(argli, 2 * ind + 1);
            FixToZero(var1);
        }
    }
    if (abs(spin) <= 1)
    {
        int ind = 3;
        RooRealVar *var = CastFromList<RooRealVar>(argli, 2 * ind);
        FixToZero(var);
        RooRealVar *var1 = CastFromList<RooRealVar>(argli, 2 * ind + 1);
        FixToZero(var1);
    }
    if (dornd_LS)
    {
        RooAbsReal * type_var = CastFromList<RooAbsReal>(argli, argli->getSize() - 3);
        assert(type_var);
        if (type_var->getVal() == MODELINDEPENDENT)
        {
            std::cout << "do rnd for SplineZ" << std::endl;
            int noffset = 8;
            int nsp = NUMSPZ;
            for (int i = 0; i < nsp; ++i)
            {
                RooRealVar *var = CastFromList<RooRealVar>(argli, noffset + i*3);
                RooRealVar *var1 = CastFromList<RooRealVar>(argli, noffset + i*3 + 1);
                if (CheckNotNullAndFloat(var)){
                    double delta = g_rnd->Gaus() * var->getVal() * rnd_scale_LS_coupling;
                    random_smear(var, delta);
                    configure_RooRealVar(var, false);
                }
                if (CheckNotNullAndFloat(var1)){
                    double delta = g_rnd->Gaus() * var1->getVal() * rnd_scale_LS_coupling;
                    random_smear(var1, delta);
                    configure_RooRealVar(var1, false);
                }
            }
        }
    }
}

void FloatRGPar(RooArgList *argli, bool dornd = false, bool dornd_crazy = false){
    if(!dornd) return; // Do nothing if we don't randomize

    double smear_scale = dornd_crazy ? 0.2 : 0.1;
    for (size_t i = 0; i < argli->getSize(); i++){
        auto * variable = dynamic_cast<RooRealVar *>(argli->at(i));
        if (CheckNotNullAndFloat(variable)){
            double delta = g_rnd->Gaus() * variable->getVal() * smear_scale;
            random_smear(variable, delta);
            configure_RooRealVar(variable, false);
        }
    }
}

void RotatePar(RooArgList *argli, TComplex r, bool isPc = false)
{
    int maxi = 6;
    int mini = 0;
    if (isPc)
    {
        mini = 1;
        maxi = 4;
    }
    auto size = argli->getSize();

    TComplex r1;
    for (int ind = mini; ind < maxi; ++ind)
    {
        RooRealVar *var = dynamic_cast<RooRealVar *>(argli->at(2 * ind));
        RooRealVar *var1 = dynamic_cast<RooRealVar *>(argli->at(2 * ind + 1));
        if (var == NULL || var1 == NULL)
            continue;
        r1 = TComplex(var->getVal(), var1->getVal());
        r1 = r1 / r;
        var->setVal(r1.Re());
        var1->setVal(r1.Im());
    }
}

TList* fixPar_getfixedPar(TList* list_component){
    TList* fixedPars = new TList();
    TIterator* coefIter = list_component->MakeIterator();
//    std::cout << "wmz debug 1" << std::endl;
    while (RooArgList* Ires = (RooArgList *)coefIter->Next()){
    RooArgList ires = (*Ires);
    int iressize = ires.getSize();
    for(int i=0; i<ires.getSize(); ++i){
//    std::cout << "wmz debug 2" << std::endl;
                RooRealVar* tmp = (RooRealVar*)(ires.at(i));
                if (! (tmp->isConstant())) {
                        fixedPars->Add(tmp);
//                        std::cout << "wmz debug 3" << std::endl;
                        tmp->setConstant(1);
                }
        }
    }
    return fixedPars;
}

void floatList(TList* fixedPars){
//    std::cout << "wmz debug 4" << std::endl;
    TIterator* coefIter = fixedPars->MakeIterator();
    while (RooRealVar* tmp = (RooRealVar *)coefIter->Next()){
        std::cout << tmp->GetName() << std::endl;
        //std::cout << "wmz debug 5" << std::endl;
        tmp->setConstant(0);
    }
}

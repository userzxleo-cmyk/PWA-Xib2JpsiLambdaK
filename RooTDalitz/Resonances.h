#ifndef RESONANCES
#define RESONANCES
// #endif
// #define NUMSP 16
#define NUMSP 24
#define NUMSPZ 11
#define NKMC 4
#define SMEAR_NBINS 30
#define SMEAR_FINE_NBINS 150
#define SMEAR_BAD_NBINS 20 // for MC
#define SMARTBINNING

// #define RESPLUS
// #define RESMINUS
#define ComplexMassSwave

constexpr bool ISOSPIN_SIGMACD = false;
// Linear term in production vector of non-resonant components in K-matrix lineshape
constexpr bool LINEAR_KMNR = false;
// Self-scattering of non-resonant components in K-matrix lineshape
constexpr bool SELF_SCATTERING = false;
// Use L+1 for the lineshape amplitudes
constexpr bool LINESHAPE_LBPLUSONE = false;

// #define SigmacPlusDzero
// #define SigmacPlusPlusDMinus
// #define ZeroSigmacWidth

constexpr int nset = 6;

constexpr float _smear_nsigmas = 5;
constexpr float _smear_binning_scheme[SMEAR_NBINS] = {
    -2.128, -1.645, -1.383, -1.192, -1.036, -0.903, -0.783, -0.674, -0.573, -0.477,
    -0.385, -0.297, -0.210, -0.126, -0.042, 0.042,  0.126,  0.210,  0.297,  0.385,
    0.477,  0.573,  0.675,  0.784,  0.903,  1.037,  1.192,  1.383,  1.645,  2.128};

constexpr float _smear_fine_binning_scheme[SMEAR_FINE_NBINS] = {
    -2.713, -2.326, -2.128, -1.989, -1.881, -1.791, -1.713, -1.645, -1.583, -1.527, -1.476, -1.428,
    -1.383, -1.341, -1.301, -1.263, -1.226, -1.192, -1.158, -1.126, -1.095, -1.065, -1.036, -1.008,
    -0.981, -0.954, -0.928, -0.903, -0.878, -0.854, -0.830, -0.806, -0.783, -0.761, -0.739, -0.717,
    -0.696, -0.674, -0.654, -0.633, -0.613, -0.593, -0.573, -0.553, -0.534, -0.515, -0.496, -0.477,
    -0.458, -0.440, -0.421, -0.403, -0.385, -0.367, -0.349, -0.332, -0.314, -0.297, -0.279, -0.262,
    -0.245, -0.227, -0.210, -0.193, -0.176, -0.160, -0.142, -0.126, -0.109, -0.092, -0.075, -0.058,
    -0.042, -0.025, -0.008, 0.008,  0.025,  0.042,  0.059,  0.075,  0.092,  0.109,  0.126,  0.143,
    0.160,  0.176,  0.193,  0.210,  0.228,  0.245,  0.262,  0.279,  0.297,  0.314,  0.332,  0.350,
    0.367,  0.385,  0.403,  0.422,  0.440,  0.458,  0.477,  0.496,  0.515,  0.534,  0.553,  0.573,
    0.593,  0.613,  0.633,  0.654,  0.675,  0.696,  0.717,  0.739,  0.761,  0.784,  0.806,  0.830,
    0.854,  0.878,  0.903,  0.928,  0.954,  0.981,  1.008,  1.037,  1.066,  1.096,  1.126,  1.159,
    1.192,  1.226,  1.263,  1.301,  1.341,  1.383,  1.428,  1.476,  1.527,  1.583,  1.645,  1.713,
    1.791,  1.881,  1.989,  2.128,  2.326,  2.713};

constexpr float _smear_bad_binning_scheme[SMEAR_BAD_NBINS] = {
    -1.960, -1.440, -1.150, -0.934, -0.755, -0.598, -0.454, -0.319, -0.189, -0.063,
    0.063,  0.189,  0.319,  0.454,  0.598,  0.755,  0.935,  1.150,  1.440,  1.960};

constexpr float _mres0 = -0.134064;
constexpr float _mres1 = 0.0599014;
constexpr float _mres2 = -0.00655841;

// set Kmatrix channel masses

#ifndef SigmacPlusDzero
#ifndef SigmacPlusPlusDMinus
constexpr float Sigmacst_width = -0.00800f; // average of + and ++ states half width
constexpr float Sigmac_width = -0.00105f;   // average of + and ++ states half width
constexpr float Dst_mass = 2.008555f;       // average of 0 and -
constexpr float D_mass = 1.86719f;          // average of 0 and -
constexpr float Sigmac_mass = 2.453435f;    // average of + and ++
constexpr float Sigmacst_mass = 2.517955f;  // average of + and ++
#endif
#endif

#ifdef SigmacPlusDzero
constexpr float Sigmacst_width = -17.2f / 1000.f / 2.f; // half width of + state
constexpr float Sigmac_width = -2.3f / 1000.f / 2.f;    // half width of + state
constexpr float Sigmac_mass = 2.45265f;
constexpr float Sigmacst_mass = 2.5174f;
constexpr float D_mass = 1.86484f;
constexpr float Dst_mass = 2.00685f;
#endif

#ifdef SigmacPlusPlusDMinus
constexpr float Sigmacst_width = -14.78f / 1000.f / 2.f; // half width of ++ state
constexpr float Sigmac_width = -1.89f / 1000.f / 2.f;    // half width of ++ state
constexpr float Sigmac_mass = 2.45397f;
constexpr float Sigmacst_mass = 2.51841f;
constexpr float D_mass = 1.86966f;
constexpr float Dst_mass = 2.01026f;
#endif

#ifdef SigmacPlusDzero
#ifdef SigmacPlusPlusDMinus
std::cout << "wmz warning defined both SigmacPlusDzero and SigmacPlusPlusDMinus" << std::endl;
const float Sigmacst_width = -9999.f;
const float Sigmac_width = -9999.f;
const float Sigmac_mass = -999.f;
const float Sigmacst_mass = -12231.f;
const float D_mass = -1231.f;
const float Dst_mass = -332.f;
#endif
#endif

constexpr float Km1[5] = {0.938272046, Dst_mass, Dst_mass, D_mass, D_mass};
constexpr float Km2[5] = {3.096916, Sigmac_mass, Sigmacst_mass, Sigmac_mass, Sigmacst_mass};

constexpr int Npmax = 4;
enum { MASS = 0, WIDTH, SPIN, LRESON, TYPE, PARITY };
enum {
    BREITWIGNER = 1,
    FLATTE,
    MODELINDEPENDENT,
    CHEBYCHEV,
    MODIFIEDFLATTE,
    CUSP,
    TRIANGLE,
    PARTIALWAVE = 120,
    KMATRIX = 240,
    REVISEDKMATRIX = 360,
    NONRESONANT
};

#endif // RESONANCES

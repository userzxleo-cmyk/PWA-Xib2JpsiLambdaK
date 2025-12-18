#ifndef CU_JPSIHH_DLZ_H
#define CU_JPSIHH_DLZ_H

#include "../RooTDalitz/NDefine.h"
/* define data struct for an event, including varibles used for PWA calcuations, 
and these intermediate varaibles are not changed
*/

struct cu_Jpsihh_dlz{
  float mppi;
  float mjpsip;
  float mjpsipi;
  float w;
  float ARe[2][2][6];
  float AIm[2][2][6];
  float DL[5][2][6];    //For dfunc of L*(JL_lL,lp)J=1/2...9/2;

  float AZRe[2][2][12];
  float AZIm[2][2][12];
  float DZ[4][2][12];

  float AXRe[2][2][12];
  float AXIm[2][2][12];
  float DX[4][2][12];
};
#endif


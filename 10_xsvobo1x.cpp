////////////////////////////////////////////////////////////////////////////
// Model wheel.cc                 SIMLIB/C++
//
// Wheel on spring with damper model (Version 1)
//
// System description (governing equation):
//   y'' = ( F - D * y' - k * y ) / M
// where:
//   M is wheel mass
//   y    height of wheel center
//   D    damping coefficient
//   k    stiffness of spring
//   F    input force
//

#include "simlib.h"

#define HtcI 33.4
#define Htc1 36  // experimental
#define BT 1.0   // 4 * 500
#define BL 0.5   // blood loss during the operation
#define BV 4.5   // blood volume

// select, which simulation should be run
// 0 ... operation without hemodilution
// 1 ... 
int outputSelect = 0;

/*
https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3592192/pdf/586_2004_Article_755.pdf
In contrast, if no hemodilution were
used during the procedure, then the hematocrit following operative blood loss would be approximately 28%
*/
struct OperationWithoutANH {
  Integrator vl;
  OperationWithoutANH(Input hematocrit, double bloodVolume, double initialHematocrit) :
    vl( - (hematocrit / bloodVolume), initialHematocrit) {}
    double Value() {
      return vl.Value();
    }
};

struct ANH {
  Integrator vl;
  ANH(Input hematocrit, double bloodVolume, double initialHematocrit) :
    vl( - (hematocrit / bloodVolume), initialHematocrit) {}
    double Value() {
      return vl.Value();
    }
};

/*
struct ANH : public aContiBlock {                  // wheel model
  Integrator y;
  ANH(Input Hct, double EBV, int Initial):
	y( - ( Hct / EBV), Initial ) {
    printf("Init: %d\n", Initial);
  }    // height
  double Value () {
    return y.Value();
    printf("ANH y: %f\n", y.Value());
  }
};
*/

struct ANH1 : public aContiBlock {                  // wheel model
  Integrator y;
  ANH1(Input Hct, double EBV, int Initial):
	y( - ( Hct / EBV) - 5, Initial ) {
    printf("Init: %d\n", Initial);
    printf("ANH1 y: %f\n", y.Value());
  }    // height
  double Value () {
    return y.Value();
  }
};


// zredeni krve roztokem
struct Hemodilution : public aContiBlock {
  Hemodilution() {
    printf("HEYO\n");
  }
  int computeValue (double hematocritBefore, double bloodVolume, double bloodToTake) {
    double hematocritAfter = ((hematocritBefore * (bloodVolume - bloodToTake)) / bloodVolume);
    printf("hematocritAfter: %f\n", hematocritAfter);
    return hematocritAfter;
  }
  double Value () {
    return 42;    // FIXME
  }
};


// doplneni krve
struct HemodilutionWithdrawn : public aContiBlock {
  HemodilutionWithdrawn() {}
  double computeValue (double hematocritBefore, double hematocritAfter, double bloodVolume, double bloodWithdrawn) {
    double erythrocytesBefore = hematocritBefore/100.0 * bloodVolume;
    double erythrocytesAfter = hematocritAfter/100.0 * (bloodVolume - bloodWithdrawn);
    double erythrocytesWithdrawn = erythrocytesBefore - erythrocytesAfter;
    return erythrocytesWithdrawn;
  }
  double Value () {
    return 0;   // FIXME
  }
};

// doplneni krve
struct HemodilutionReplace : public aContiBlock {
  HemodilutionReplace() {}
  double computeValue (double hematocritCurrent, double erythrocytesWithdrawn, double bloodVolumeCurrent, double bloodVolumeWithdrawn) {
    double erythrocytesCurrent = hematocritCurrent/100.0 * bloodVolumeCurrent;
    double hematocritFinal = ((erythrocytesCurrent + erythrocytesWithdrawn) / (bloodVolumeCurrent + bloodVolumeWithdrawn)) * 100.0;
    return hematocritFinal;
  }
  double Value () {
    return 0;   // FIXME
  }
};


struct ANHadd : public aContiBlock {                  // wheel model
  Integrator y;
  //Hemodilution hd;
  ANHadd(Input Hct, double EBV, int Initial):
	//y( - ( Hct / EBV) + 20, Initial ) {
  y( - ( Hct / EBV) + 20, Initial ) {
    printf("Init: %d\n", Initial);
    printf("y: %f\n", y.Value());
  }    // height
  double Value () {
    return y.Value();
  }
};


// objects
//Constant Hct(40);             // constant input force
//ANH anh (HtcI, BV, HtcI);           // model
//ANH1 anh2(60, 10.0, 40);

//Hemodilution hd;
//ANHadd anhadd(hd.computeValue(28.0, BV, BT), 5.0, 28);
//ANH anh3(hd.computeValue(28.0, BV, BT), 5.0, 28);

// run operation without normovolemic hemodilution
//OperationWithoutANH operationWithoutANH(HtcI, BV, HtcI);

// run operation without normovolemic hemodilution
//OperationWithoutANH operationWithoutANH2(25, 5.0, 25);

//ANHadd anhadd(18, 3.5, 18);

//ANHdoplneni anh4()

// operation without anh - no hemodilution, experiment 0 and 1
ANH anh01(40, 5.0, 40.0);
// experiment 1, part 2
ANH anh2(32, 5.0, 32.0);
// experiment 1, part 3
ANH anh3(32.65, 4.5, 32.65);

// neco dalsiho - jineho
//ANH anh1(40, 5.0, 40.0);

HemodilutionReplace hemodilutionReplace;
HemodilutionWithdrawn hemodilutionWithdrawn;

// print model state
void Sample() {
  //if (T.Value() < BL) {
	//	Print("%6.3f %.4g\n", T.Value(),  anhadd.y.Value()); 
  //} else {
  //  Print("%6.3f %.4g\n", T.Value(),  anh2.y.Value()); 
  //}
  if (outputSelect == 0) {    // operation without hemodilution
    //Print("%6.3f %.4g\n", T.Value(),  operationWithoutANH.Value());
    Print("%6.3f %.4g\n", T.Value(), anh01.Value());
  } else if (outputSelect == 1) {
    Print("%6.3f %.4g\n", T.Value(), anh01.Value());
    //Print("%6.3f %.4g\n", T.Value(),  operationWithoutANH2.Value()); 
  } else if (outputSelect == 2) {
    Print("%6.3f %.4g\n", T.Value(), anh2.Value());
    //Print("%6.3f %.4g\n", T.Value(),  anhadd.y.Value()); 
  } else if (outputSelect == 4) {
    Print("%6.3f %.4g\n", T.Value(), anh3.Value());
  }
}
Sampler S(Sample, 0.001);       // periodic sampling

int main() {                    // exeriment description
  // experiment 0: operation without hemodilution
  /*
  outputSelect = 0;
  SetOutput("10_xsvobo1x_0.dat");
  Print("# Acute Normovolemic Hemodilution - experiment 0\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.5);                 // set expected blood volume loss range [l]
  SetStep(1e-3, 0.1);           // numerical method step size range
  SetAccuracy(1e-5, 0.001);     // allowed (abs.+rel.) error tolerance
  Run();                        // simulation run
  
  // experiment 1: 
  outputSelect = 1;
  SetOutput("10_xsvobo1x_1.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.0);                 // set expected blood volume loss range [l]
  Run();                        // simulation run
  */

  // experiment 1_1: 
  outputSelect = 2;
  SetOutput("10_xsvobo1x_2.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.5);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 3: print value in terminal
  //outputSelect = 3;
  //Init(0, 0);                   // the Init is compulsory before Run()
  //Run();                        // simulation run
  printf("experiment 3: ...\n");
  double erythrocytesWithdrawn = hemodilutionWithdrawn.computeValue(40.0, 28.0, 5.0, 1.5);
  printf("erythrocytesWithdrawn = %f\n", erythrocytesWithdrawn);
  double hematocritFinal = hemodilutionReplace.computeValue(17.5, erythrocytesWithdrawn, 3.5, 1.5);
  printf("hematocritFinal = %f\n", hematocritFinal);
  //outputSelect++;

  // experiment 1_3: 
  outputSelect = 4;
  SetOutput("10_xsvobo1x_3.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 0.5);                 // set expected blood volume loss range [l]
  Run();                        // simulation run


  outputSelect = 9;
  Init(0, 1.0);                   // set initial state and time
  Run();                        // simulation run

  SIMLIB_statistics.Output();   // print run statistics
}


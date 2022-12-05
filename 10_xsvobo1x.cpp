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

struct ANH {
  Integrator vl;
  ANH(Input hematocrit, double bloodVolume, double initialHematocrit) :
    vl( - (hematocrit / bloodVolume), initialHematocrit) {}
    double Value() {
      return vl.Value();
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

// operation without anh - no hemodilution, experiment 0 and 1
ANH anh01(40, 5.0, 40.0);
// experiment 1, part 2
ANH anh2(32, 5.0, 32.0);
// experiment 1, part 3
ANH anh3(32.65, 4.5, 32.65);

// experiment 2
ANH anh4(32, 5.0, 32.0);

ANH anh6(28, 5.0, 28.0);
// neco dalsiho - jineho
//ANH anh1(40, 5.0, 40.0);

HemodilutionReplace hemodilutionReplace;
HemodilutionWithdrawn hemodilutionWithdrawn;

// print model state
void Sample() {
  if (outputSelect < 2) {    // operation without hemodilution
    Print("%6.3f %.4g\n", T.Value(), anh01.Value());
  } else if (outputSelect == 2) {
    Print("%6.3f %.4g\n", T.Value(), anh2.Value());
  } else if (outputSelect == 3) {
    Print("%6.3f %.4g\n", T.Value(), anh3.Value());
  } else if (outputSelect == 4) {
    Print("%6.3f %.4g\n", T.Value(), anh4.Value());
  } else if (outputSelect == 5) {
    Print("%6.3f %.4g\n", T.Value(), anh01.Value());
  } else if (outputSelect == 6) {
    Print("%6.3f %.4g\n", T.Value(), anh6.Value());
  } else if (outputSelect == 7) {
    Print("%6.3f %.4g\n", T.Value(), anh01.Value());
  }
}
Sampler S(Sample, 0.001);       // periodic sampling

int main() {                    // exeriment description
  // experiment 0: operation without hemodilution
  outputSelect = 0;
  printf("experiment 0: operation without hemodilution, 1.5l blood loss, 40%% initial hematocrit...\n");
  SetOutput("10_xsvobo1x_0.dat");
  Print("# Acute Normovolemic Hemodilution - experiment 0\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.5);                 // set expected blood volume loss range [l]
  SetStep(1e-3, 0.1);           // numerical method step size range
  SetAccuracy(1e-5, 0.001);     // allowed (abs.+rel.) error tolerance
  Run();                        // simulation run

  // experiment 1: 
  outputSelect = 1;
  printf("experiment 1: operation with ANH, 1.5l blood loss, 40%% initial hematocrit:\n");
  printf("part 1: ANH blood withdrawal, 1 litre...\n");
  SetOutput("10_xsvobo1x_1.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.0);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 1_1: 
  outputSelect = 2;
  printf("part 2: operation after ANH, 1.5 litres blood loss...\n");
  SetOutput("10_xsvobo1x_2.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.5);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 1_2:TODO  print value in terminal
  printf("experiment 3: ...\n");
  printf("part 3: ANH transfusion after the operation...\n");
  // compute number of erythrocytes withdrawn in the first phase of ANH
  double erythrocytesWithdrawn = hemodilutionWithdrawn.computeValue(40.0, 28.0, 5.0, 1.5);
  // compute the final hematocrit after adding the withdrawn blood into patient's bloodstream
  printf("erythrocytesWithdrawn = %f\n", erythrocytesWithdrawn);
  double hematocritFinal = hemodilutionReplace.computeValue(17.5, erythrocytesWithdrawn, 3.5, 1.5);
  printf("hematocritFinal = %f\n", hematocritFinal);

  // TODO FIXME continue here... and don't forget to check the names of the experiments

  // experiment 1_3: 
  outputSelect = 3;
  SetOutput("10_xsvobo1x_3.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 0.5);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 2
  outputSelect = 4;
  SetOutput("10_xsvobo1x_4.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.5);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 3_1: 2 litres ANH withdrawal
  outputSelect = 5;
  SetOutput("10_xsvobo1x_5.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 2.0);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 3_2: 1.5 litres blood volume loss during operation
  outputSelect = 6;
  SetOutput("10_xsvobo1x_6.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.5);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 3_3: blood transfusion after operation
  printf("experiment 3_3: ...\n");
  erythrocytesWithdrawn = hemodilutionWithdrawn.computeValue(40.0, 24.0, 5.0, 2.0);
  printf("erythrocytes withdrawn = %f\n", erythrocytesWithdrawn);
  hematocritFinal = hemodilutionReplace.computeValue(19.5, erythrocytesWithdrawn, 3.0, 2.0);
  printf("hematocrit final = %f\n", hematocritFinal);
  // final hematocrit je 37.6 - cool


  // experiment 4 - 2l replace, 1.5 during operation, no ANH
  outputSelect = 7;
  SetOutput("10_xsvobo1x_7.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.5);                 // set expected blood volume loss range [l]
  Run();                        // simulation run
  // final hematocrit je 28
  // rozdil s ANH a bez ANH je velky - napsat do dokumentace presne tyto experimenty
  // napsat vzorecky a postup, jakym se vse po sobe pocita - pri ANH i bez ANH
  // udelat screenshoty


  SIMLIB_statistics.Output();   // print run statistics
}


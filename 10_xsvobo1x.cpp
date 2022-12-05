/*
 * File:          10_xsvobo1x.cpp
 * Institution:   FIT BUT
 * Academic year: 2022/2023
 * Course:        IMS - Modelling and Simulation
 * Author:        Lucie Svobodová, xsvobo1x@stud.fit.vutbr.cz
 *
 * IMS project, variant 10: Acute Normovolemic Hemodilution
 */
#include "simlib.h"

// select simulation to be run
int outputSelect = 0;

/*
  The differential equation for the rate of change of hematocrit per
  volume loss to the total hematocrit and blood volume.
*/
struct ANH {
  Integrator vl;
  ANH(Input hematocrit, double bloodVolume, double initialHematocrit) :
    vl( - (hematocrit / bloodVolume), initialHematocrit) {}
    double Value() {
      return vl.Value();
    }
};

/*
  Structure that holds the equation for computing the volume
  of erythrocytes in the drawn blood.
*/
struct HemodilutionWithdrawn : public aContiBlock {
  HemodilutionWithdrawn() {}
  // function computes how many erythrocytes were withdrawn
  // -> they are stored and will be used in the post-operational transfusion
  double computeErythrocytesWithdrawn (double hematocritBefore, double hematocritAfter, double bloodVolume, double bloodWithdrawn) {
    double erythrocytesBefore = hematocritBefore/100.0 * bloodVolume;
    double erythrocytesAfter = hematocritAfter/100.0 * (bloodVolume - bloodWithdrawn);
    double erythrocytesWithdrawn = erythrocytesBefore - erythrocytesAfter;
    return erythrocytesWithdrawn;
  }
  // Value has to be defined but is not used in this program
  double Value () { return 0; }
};

/*
  Structure that holds the equation for post-operational transfusion.
*/
struct HemodilutionReplace : public aContiBlock {
  HemodilutionReplace() {}
  // function computes the final hematocrit value after the post-operational ANH transfusion
  double computeHematocrit(double hematocritCurrent, double erythrocytesWithdrawn, double bloodVolumeCurrent, double bloodVolumeWithdrawn) {
    double erythrocytesCurrent = hematocritCurrent/100.0 * bloodVolumeCurrent;
    double hematocritFinal = ((erythrocytesCurrent + erythrocytesWithdrawn) / (bloodVolumeCurrent + bloodVolumeWithdrawn)) * 100.0;
    return hematocritFinal;
  }
  // function has to be defined but is not used in this program
  double Value () { return 0; }
};

/*
  Experiments - initializing objects
*/
// experiment 0 (operation without ANH) and 1
ANH anh01(40, 5.0, 40.0);
// experiment 1, part 2
ANH anh2(32, 5.0, 32.0);
// experiment 1, part 3
ANH anh3(32.65, 4.5, 32.65);
// experiment 2, part 2
ANH anh5(24, 5.0, 24.0);
// experiments 1, part 3 and 2, part 3
HemodilutionReplace hemodilutionReplace;
HemodilutionWithdrawn hemodilutionWithdrawn;

/*
  Function prints value of the model.
  The variable outputSelect is used to print various experiments.
*/
void Sample() {
  if (outputSelect < 2) {    // operation without hemodilution
    Print("%6.3f %.4g\n", T.Value(), anh01.Value());
  } else if (outputSelect == 2) {
    Print("%6.3f %.4g\n", T.Value(), anh2.Value());
  } else if (outputSelect == 3) {
    Print("%6.3f %.4g\n", T.Value(), anh3.Value());
  } else if (outputSelect == 4) {
    Print("%6.3f %.4g\n", T.Value(), anh01.Value());
  } else if (outputSelect == 5) {
    Print("%6.3f %.4g\n", T.Value(), anh5.Value());
  } else if (outputSelect == 6) {
    Print("%6.3f %.4g\n", T.Value(), anh01.Value());
  }
}
Sampler S(Sample, 0.001);       // periodic sampling

/*
  Main function, experiment description.
*/
int main() {
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

  // experiment 1_1: 
  outputSelect = 1;
  printf("experiment 1: operation with ANH, 1.5l blood loss, 40%% initial hematocrit:\n");
  printf("part 1: ANH blood withdrawal, 1 litre...\n");
  SetOutput("10_xsvobo1x_1.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.0);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 1_2: 
  outputSelect = 2;
  printf("part 2: operation after ANH, 1.5 litres blood loss...\n");
  SetOutput("10_xsvobo1x_2.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.5);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 1_3: ANH transfusion after the operation
  printf("part 3: ANH transfusion after the operation...\n");
  // compute number of erythrocytes withdrawn in the first phase of ANH
  double erythrocytesWithdrawn = hemodilutionWithdrawn.computeErythrocytesWithdrawn(40.0, 28.0, 5.0, 1.5);
  printf("erythrocytes withdrawn = %f\n", erythrocytesWithdrawn);
  // compute the final hematocrit after adding the withdrawn blood into patient's bloodstream
  double hematocritFinal = hemodilutionReplace.computeHematocrit(17.5, erythrocytesWithdrawn, 3.5, 1.5);
  printf("final hematocrit = %f\n", hematocritFinal);

  // experiment 1_4: post-operation blood volume loss
  outputSelect = 3;
  printf("part 4: post-operation blood volume loss, 0.5 litre...\n");
  SetOutput("10_xsvobo1x_3.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 0.5);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 2_1: 2 litres ANH withdrawal
  outputSelect = 4;
  printf("experiment 2: experiment with ANH, 1.5l blood loss, 2 litres withdrawn, 40%% initial hematocrit:\n");
  printf("part 1: 2 litres ANH withdrawal...\n");
  SetOutput("10_xsvobo1x_4.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 2.0);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 2_2: 1.5 litres blood volume loss during operation
  outputSelect = 5;
  printf("part 2: 1.5 litres blood volume loss during operation...\n");
  SetOutput("10_xsvobo1x_5.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.5);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  // experiment 2_3: blood transfusion after operation
  printf("part 3: blood transfusion after operation...\n");
  // compute number of erythrocytes withdrawn in the first phase of ANH
  erythrocytesWithdrawn = hemodilutionWithdrawn.computeErythrocytesWithdrawn(40.0, 24.0, 5.0, 2.0);
  printf("erythrocytes withdrawn = %f\n", erythrocytesWithdrawn);
  // compute the final hematocrit after adding the withdrawn blood into patient's bloodstream
  hematocritFinal = hemodilutionReplace.computeHematocrit(16.8, erythrocytesWithdrawn, 3.0, 2.0);
  printf("hematocrit final = %f\n", hematocritFinal);

  // experiment 3: 1.5 litres blood volume loss during operation, no ANH
  outputSelect = 6;
  printf("experiment 3: 1.5 litres blood volume loss during operation, no ANH...\n");
  SetOutput("10_xsvobo1x_6.dat");
  Print("# Acute Normovolemic Hemodilution\n");
  Print("# BloodVolumeLoss   Hematocrit \n");
  Init(0, 1.5);                 // set expected blood volume loss range [l]
  Run();                        // simulation run

  SIMLIB_statistics.Output();   // print run statistics
}

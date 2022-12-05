# File:          10_xsvobo1x.plt
# Institution:   FIT BUT
# Academic year: 2022/2023
# Course:        IMS - Modelling and Simulation
# Author:        Lucie Svobodová, xsvobo1x@stud.fit.vutbr.cz
#
# IMS project, variant 10: Acute Normovolemic Hemodilution
#
# GnuPlot script for Acute Normovolemic Hemodilution model
# Data format: BloodVolumeLoss[l]   Hematocrit[%]

reset  # defaults
set output "10_xsvobo1x.png"; set term png # merlin output to png file

set grid
set style data lines
set key
set xlabel "Blood Volume Loss [l]"
set ylabel "Hematocrit [%]"

set title "Acute Normovolemic Hemodilution - experiment 0"
plot "10_xsvobo1x_0.dat" title "hematocrit"
pause 1

set title "Acute Normovolemic Hemodilution - experiment 1, part 1"
plot "10_xsvobo1x_1.dat" title "hematocrit"
pause 1

set title "Acute Normovolemic Hemodilution - experiment 1, part 2"
plot "10_xsvobo1x_2.dat" title "hematocrit"
pause 1

set title "Acute Normovolemic Hemodilution - experiment 1, part 3"
plot "10_xsvobo1x_3.dat" title "hematocrit"
pause 1

set title "Acute Normovolemic Hemodilution - experiment 2, part 1"
plot "10_xsvobo1x_4.dat" title "hematocrit"
pause 1

set title "Acute Normovolemic Hemodilution - experiment 2, part 2"
plot "10_xsvobo1x_5.dat" title "hematocrit"
pause 1

set title "Acute Normovolemic Hemodilution - experiment 3"
plot "10_xsvobo1x_6.dat" title "hematocrit"
pause 1

############################################################################
# GnuPlot script for model Acute Normovolemic Hemodilution
#
# Data format: Time y v
#
reset  # defaults

#set output "wheel.ps"; set term postscript

set grid
set style data lines


set key
set xlabel "Blood Volume Loss [l]"
set ylabel "Hematocrit [%]"

#set title "Acute Normovolemic Hemodilution - experiment 0"
#plot "10_xsvobo1x_0.dat" title "hematocrit"
#pause 10

#set title "Acute Normovolemic Hemodilution - experiment 1, part 1"
#plot "10_xsvobo1x_1.dat" title "hematocrit"
#pause 10

#set title "Acute Normovolemic Hemodilution - experiment 1, part 2"
#plot "10_xsvobo1x_2.dat" title "hematocrit"
#pause 10

set title "Acute Normovolemic Hemodilution - experiment 1, part 3"
plot "10_xsvobo1x_3.dat" title "hematocrit"
pause 10



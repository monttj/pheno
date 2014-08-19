echo "creating histograms..."

root -b -l <<EOF
.x makeHistogramPhoton.C("../../PROC_AnoTopCouplingsFull_h2aa/Events/run_01/output.root","h2aa")
.x makeHistogramPhoton.C("../../PROC_AnoTopCouplingsFull_h2aa/Events/run_01/output_100p.root","h2aa_100p")
.x makeHistogramPhoton.C("../../PROC_AnoTopCouplingsFull_tch2aa/Events/run_01/output.root","tch2aa")
.x makeHistogramPhoton.C("../../ttbarLO/Events/run_01/output.root","ttbarLO")
.q
EOF

echo "plotting histograms..."

root -b -l <<EOF
.x PlotPhoton.C
EOF

echo "calculating efficiency..."

root -b -l << EOF
.x Eff.C
EOF


echo "creating histograms..."

root -b -l <<EOF
.x makeHistogramPhoton.C("../../sample/h2aa/output.root","h2aa")
.x makeHistogramPhoton.C("../../sample/h2aa/output_p100.root","h2aa_100p")
.x makeHistogramPhoton.C("../../sample/tch2aa/output.root","tch2aa")
.x makeHistogramPhoton.C("../../sample/ttbar/output.root","ttbarLO")
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


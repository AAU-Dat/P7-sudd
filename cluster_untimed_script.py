import jajapy as ja
import os.path

name = "results_cluster_untimed.txt"
file1 = open(name, "w")
original_model = ja.loadPrism("examples/materials/cluster.sm")
original_model.instantiate(["ws_fail", "switch_fail", "line_fail"], [1/500, 1/4000, 1/5000])
for iter in range (1,11):
  file1.write("\nrun "+str(iter)+"\n")
  training_set = original_model.generateSet(100,30,timed=False)
  initial_hypothesis = ja.loadPrism("examples/materials/cluster.sm")
  output_val = ja.BW().fit_nonInstantiatedParameters(training_set, initial_hypothesis, min_val=0.00025, max_val=0.0025, return_data=True, compute_alpha_beta_how=ja.ComputeAlphaBetaHow.MATRIX)
  file1.write(str(output_val))

file1.close()
import jajapy as ja
import os.path


name = "results_tandem_untimed.txt"
file1 = open(name, "w")
original_model = ja.loadPrism("examples/materials/tandem_3.sm")
original_model.instantiate(["mu1a", "mu1b", "mu2", "kappa"], [0.2, 1.8, 2.0, 4.0])
for iter in range (1,11):
  file1.write("\nrun "+str(iter)+"\n")
  training_set = original_model.generateSet(100,30,timed=False)
  initial_hypothesis = ja.loadPrism("examples/materials/tandem_3.sm")
  output_val = ja.BW().fit_nonInstantiatedParameters(training_set, initial_hypothesis,min_val=0.1, max_val=5.0, return_data=True)
  file1.write(str(output_val))

file1.close()
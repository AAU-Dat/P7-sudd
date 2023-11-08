import jajapy  as ja
import os.path

name = "results_philosophers_timed.txt"
file1 = open(name, "w")
original_model = ja.loadPrism("examples/materials/philosophers1.sm")
original_model.instantiate(["alpha", "betax", "zetax"], [0.3, 0.1, 0.2])
for iter in range (1,11):
	file1.write("run"+ str(iter) + "\n")
	training_set = original_model.generateSet(100, 30, timed=True)
	initial_hypothesis = ja.loadPrism("examples/materials/philosophers1.sm")
	output_val = ja.BW().fit_nonInstantiatedParameters(training_set, initial_hypothesis, min_val=0.00025, max_val=0.0025, return_data=True)
	file1.write(str(output_val))
	file1.write("\n")
	
file1.close()
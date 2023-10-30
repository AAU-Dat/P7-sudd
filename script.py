import jajapy  as ja
import os.path
path = "C:\Users\test\PycharmProjects\jajapy\results"
name = raw_input(results_philosophers-gamma.txt)
completeName = os.path.join(path, name)
file1 = open(completeName, "w")
original_model = ja.loadPrism("examples/materials/philosophers-gamma.sm")
original_model.instantiate(["alpha", "betax", "gammax", "zetax"], [0.3, 0.1, 1.0, 0.2])
while i <= 10:
	file1.write("run"+ str(i) + "\n")
	original_model.savePrism('test_save' + str(i) '.sm')
	training_set = original_model.generateSet(10, 10, timed=False)
	initial_hypothesis = ja.loadPrism("examples/materials/philosophers-gamma.sm")
	output_val = ja.BW().fit_nonInstantiatedParameters(training_set, initial_hypothesis, min_val=0.00025, max_val=0.0025, return_data=True)
	file1.write(output_val + "\n")
	i++
file1.close()
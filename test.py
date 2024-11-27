from random import uniform

import pandas as pd

from jajapy import loadPrism, BW, loadSet
from jajapy.base.BW import ComputeAlphaBetaHow

polling_hypothesis = loadPrism("examples/materials/polling.sm")

results = pd.DataFrame(
    columns=['model', 'run', 'timed', 'implementation', 'initial_parameters', 'final_parameters', 'learning_time', 'learning_rounds', 'training_set_loglikelihood'])
initial_polling_parameter_values = [1, 200]



polling_observations = loadSet("training_sets/training_set_polling_timed1.txt")



polling_hypothesis.instantiate(["mu", "gammax"], initial_polling_parameter_values)

(polling_parameters, polling_result) = BW().fit_parameters(polling_observations, polling_hypothesis,["mu", "gammax"], return_data=True, compute_alpha_beta_how=ComputeAlphaBetaHow.SYMBOLIC)

polling_dataframe = pd.DataFrame([
                        {"model": "Polling", 
                         "run": 1, 
                         "timed": "timed", 
                         "implementation": ComputeAlphaBetaHow.SYMBOLIC.name,
                         "initial_parameters": initial_polling_parameter_values, 
                         "final_parameters": polling_parameters,
                         "learning_time": polling_result['learning_time'],
                         "learning_rounds": polling_result['learning_rounds'],
                         "training_set_loglikelihood": polling_result['training_set_loglikelihood']}])

results = pd.concat([results, polling_dataframe], ignore_index=True, axis=0)


print(results)
results.to_csv("test_results.csv")



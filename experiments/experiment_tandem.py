from random import uniform

import pandas as pd
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from jajapy import loadPrism, BW, loadSet
from jajapy.base.BW import ComputeAlphaBetaHow

NUMBER_OF_RUNS = 11
tandem_hypothesis = loadPrism("examples/materials/tandem_3.sm")

results = pd.DataFrame(
    columns=['model', 'run', 'timed', 'implementation', 'initial_parameters', 'final_parameters', 'learning_time', 'learning_rounds', 'training_set_loglikelihood'])

for run in range(1,NUMBER_OF_RUNS):
    
    initial_tandem_parameter_values = [uniform(0.1, 5.0), uniform(0.1, 5.0), uniform(0.1, 5.0), uniform(0.1, 5.0)]
    
    for timed in [True, False]:
        
        if timed:
            tandem_observations = loadSet(f"training_sets/training_set_tandem_timed"+str(run)+".txt")
        else:
            tandem_observations = loadSet(f"training_sets/training_set_tandem_untimed" + str(run) + ".txt")

        for impl in [ComputeAlphaBetaHow.CLASSIC, ComputeAlphaBetaHow.SYMBOLIC, ComputeAlphaBetaHow.SYMBOLIC_LOG_SEMIRING]:
            
            tandem_hypothesis.instantiate(["mu1a", "mu1b", "mu2", "kappa"], initial_tandem_parameter_values)
            
            (tandem_parameters, tandem_result) = BW().fit_parameters(tandem_observations,tandem_hypothesis, ["mu1a", "mu1b", "mu2", "kappa"], return_data=True, compute_alpha_beta_how=impl)

            tandem_dataframe = pd.DataFrame([
                {"model": "Tandem", "run": run, "timed": timed, "implementation": impl.name,
                 "initial_parameters": initial_tandem_parameter_values, 
                 "final_parameters": tandem_parameters,
                 "learning_time": tandem_result['learning_time'],
                 "learning_rounds": tandem_result['learning_rounds'],
                 "training_set_loglikelihood": tandem_result['training_set_loglikelihood']}])

            results = pd.concat([results, tandem_dataframe], ignore_index=True, axis=0)


print(results)
results.to_csv("experiments/results/tandem_results.csv")



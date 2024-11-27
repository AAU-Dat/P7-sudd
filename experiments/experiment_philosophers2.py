from random import uniform

import pandas as pd
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from jajapy import loadPrism, BW, loadSet
from jajapy.base.BW import ComputeAlphaBetaHow

NUMBER_OF_RUNS = 11
philosophers2_hypothesis = loadPrism("examples/materials/philosophers-gamma.sm")

results = pd.DataFrame(
    columns=['model', 'run', 'timed', 'implementation', 'initial_parameters', 'final_parameters', 'learning_time', 'learning_rounds', 'training_set_loglikelihood'])

for run in range(1,NUMBER_OF_RUNS):
    
    initial_philosophers2_parameter_values = [uniform(0.00025, 0.0025), uniform(0.00025, 0.0025), uniform(0.00025, 0.0025), uniform(0.00025, 0.0025)]
    
    for timed in [True, False]:
        
        if timed:
            philosophers2_observations = loadSet(f"training_sets/training_set_philosophers2_timed"+str(run)+".txt")
        else:
            philosophers2_observations = loadSet(f"training_sets/training_set_philosophers2_untimed" + str(run) + ".txt")

        for impl in [ComputeAlphaBetaHow.CLASSIC, ComputeAlphaBetaHow.SYMBOLIC, ComputeAlphaBetaHow.SYMBOLIC_LOG_SEMIRING]:
            
            philosophers2_hypothesis.instantiate(["alpha", "betax", "gammax", "zetax"], initial_philosophers2_parameter_values)
            
            (philosophers2_parameters, philosophers2_result) = BW().fit_parameters(philosophers2_observations, philosophers2_hypothesis, ["alpha", "betax", "gammax", "zetax"], return_data=True, compute_alpha_beta_how=impl)

            philosophers2_dataframe = pd.DataFrame([
                {"model": "Philosophers2", "run": run, "timed": timed, "implementation": impl.name,
                 "initial_parameters": initial_philosophers2_parameter_values, 
                 "final_parameters": philosophers2_parameters,
                 "learning_time": philosophers2_result['learning_time'],
                 "learning_rounds": philosophers2_result['learning_rounds'],
                 "training_set_loglikelihood": philosophers2_result['training_set_loglikelihood']}])

            results = pd.concat([results, philosophers2_dataframe], ignore_index=True, axis=0)


print(results)
results.to_csv("experiments/results/philosophers2_results.csv")



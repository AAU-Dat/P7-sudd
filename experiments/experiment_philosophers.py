from random import uniform
import pandas as pd
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from jajapy import loadPrism, BW, loadSet
from jajapy.base.BW import ComputeAlphaBetaHow

NUMBER_OF_RUNS = 11
philosophers_hypothesis = loadPrism("examples/materials/philosophers1.sm")

results = pd.DataFrame(
    columns=['model', 'run', 'timed', 'implementation', 'initial_parameters', 'final_parameters', 'learning_time', 'learning_rounds', 'training_set_loglikelihood'])

for run in range(1,NUMBER_OF_RUNS):
    
    initial_philosophers_parameter_values = [uniform(0.00025, 0.0025), uniform(0.00025, 0.0025), uniform(0.00025, 0.0025)]
    
    for timed in [True, False]:
        
        if timed:
            philosophers_observations = loadSet(f"training_sets/training_set_philosophers_timed"+str(run)+".txt")
        else:
            philosophers_observations = loadSet(f"training_sets/training_set_philosophers_untimed" + str(run) + ".txt")

        for impl in [ComputeAlphaBetaHow.CLASSIC, ComputeAlphaBetaHow.SYMBOLIC, ComputeAlphaBetaHow.SYMBOLIC_LOG_SEMIRING]:
            
            philosophers_hypothesis.instantiate(["alpha", "betax", "zetax"], initial_philosophers_parameter_values)
            
            (philosophers_parameters, philosophers_result) = BW().fit_parameters(philosophers_observations,philosophers_hypothesis, ["alpha", "betax", "zetax"], return_data=True, compute_alpha_beta_how=impl)

            philosophers_dataframe = pd.DataFrame([
                {"model": "Philosophers", "run": run, "timed": timed, "implementation": impl.name,
                 "initial_parameters": initial_philosophers_parameter_values, "final_parameters": philosophers_parameters,
                 "learning_time": philosophers_result['learning_time'],
                 "learning_rounds": philosophers_result['learning_rounds'],
                 "training_set_loglikelihood": philosophers_result['training_set_loglikelihood']}])

            results = pd.concat([results, philosophers_dataframe], ignore_index=True, axis=0)


print(results)
results.to_csv("experiments/results/philosophers_results.csv")



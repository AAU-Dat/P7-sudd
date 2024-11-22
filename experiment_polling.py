from random import uniform

import pandas as pd

from jajapy import loadPrism, BW, loadSet
from jajapy.base.BW import ComputeAlphaBetaHow

NUMBER_OF_RUNS = 11

polling_hypothesis = loadPrism("examples/materials/polling.sm")
results = pd.DataFrame(
        columns=['model', 'run', 'timed', 'implementation', 'initial_parameters', 'final_parameters', 'time'])
for run in range(1,NUMBER_OF_RUNS):
    initial_polling_parameter_values = [uniform(0.00025, 0.0025), uniform(0.00025, 0.0025)]

    for timed in [True, False]:
        if timed:
            polling_observations = loadSet(f"training_sets/training_set_polling_timed" + str(run) + ".txt")
        else:
            polling_observations = loadSet(f"training_sets/training_set_polling_untimed" + str(run) + ".txt")

        for impl in [ComputeAlphaBetaHow.CLASSIC, ComputeAlphaBetaHow.SYMBOLIC,
                     ComputeAlphaBetaHow.SYMBOLIC_LOG_SEMIRING]:
            polling_hypothesis.instantiate(["mu", "gammax"], initial_polling_parameter_values)

            (polling_parameters, polling_result) = BW().fit_parameters(polling_observations, polling_hypothesis,
                                                                       ["mu", "gammax"], return_data=True,
                                                                       compute_alpha_beta_how=impl)

            polling_dataframe = pd.DataFrame([
                {"model": "Polling", "run": 1, "timed": "untimed", "implementation": impl.name,
                 "initial_parameters": initial_polling_parameter_values, "final_parameters": polling_parameters,
                 "learning_time": polling_result['learning_time'],
                 "learning_rounds": polling_result['learning_rounds'],
                 "training_set_loglikelihood": polling_result['training_set_loglikelihood']}])

            results = pd.concat([results], ignore_index=True, axis=0)

print(results)
results.to_csv("polling_results.csv")
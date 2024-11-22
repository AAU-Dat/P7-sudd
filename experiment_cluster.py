from random import uniform

import pandas as pd

from jajapy import loadPrism, BW, loadSet
from jajapy.base.BW import ComputeAlphaBetaHow

NUMBER_OF_RUNS = 11
cluster_hypothesis = loadPrism("examples/materials/cluster.sm")

results = pd.DataFrame(
        columns=['model', 'run', 'timed', 'implementation', 'initial_parameters', 'final_parameters', 'time'])
for run in range(1,NUMBER_OF_RUNS):
    initial_cluster_parameter_values = [uniform(1 / 5000, 1 / 500), uniform(1 / 5000, 1 / 500), uniform(1 / 5000, 1 / 500)]

    for timed in [True, False]:
        if timed:
            cluster_observations = loadSet(f"training_sets/training_set_cluster_timed" + str(run) + ".txt")
        else:
            cluster_observations = loadSet(f"training_sets/training_set_cluster_untimed" + str(run) + ".txt")

        for impl in [ComputeAlphaBetaHow.CLASSIC, ComputeAlphaBetaHow.SYMBOLIC, ComputeAlphaBetaHow.SYMBOLIC_LOG_SEMIRING]:
            cluster_hypothesis.instantiate(["ws_fail", "switch_fail", "line_fail"], initial_cluster_parameter_values)
            (cluster_parameters, cluster_result) = BW().fit_parameters(cluster_observations, cluster_hypothesis,["ws_fail", "switch_fail", "line_fail"],return_data=True, compute_alpha_beta_how=impl)

            cluster_dataframe = pd.DataFrame([
                     {"model": "Cluster", "run": run, "timed": timed, "implementation": impl.name,
                      "initial_parameters": initial_cluster_parameter_values, "final_parameters": cluster_parameters,
                      "learning_time": cluster_result['learning_time'],
                      "learning_rounds": cluster_result['learning_rounds'],
                      "training_set_loglikelihood": cluster_result['training_set_loglikelihood']}])

            results = pd.concat([results, cluster_dataframe,
                                         ], ignore_index=True, axis=0)


print(results)
results.to_csv("cluster_results.csv")



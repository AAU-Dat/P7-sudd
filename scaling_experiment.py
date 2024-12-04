import sys
from random import uniform
from jajapy.base.BW import ComputeAlphaBetaHow
from jajapy import loadSet, loadPrism, BW
from numpy import array
from datetime import datetime
import pandas as pd

min_c = 3


def experiment3(args):
    steps = 7
    nb_rep = 3
    max_c = 19
    if len(args) > 1:
        if args[1] == '--quick':
            steps = 2
            nb_rep = 5
        elif args[1] == '--fastest':
            steps = 7
            nb_rep = 3
            max_c = 19

    execute(max_c, steps, nb_rep)


def execute(max_c, steps, nb_rep):
    models_info, learning_data = learn(max_c, steps, nb_rep)
    # Write results to CSV
    results_df = pd.concat(learning_data)
    results_df.to_csv("experiments/results/scalability_results.csv", index=False)
    print("experiments/results/scalability_results.csv")


def learn(max_c, steps, nb_rep):
    learning_data = []

    for timed in [True, False]:
        k = min_c
        while k < max_c:
            # Load the dataset for the current value of `c` and timing
            ts1 = loadSet(f"training_sets/scaling_training_set_tandem_{'timed' if timed else 'untimed'}_c{k}.txt")

            # Iterate for each repetition
            for r in range(nb_rep):
                # Iterate for each implementation
                initial_params = [uniform(0.1, 5.0) for _ in range(4)]

                for impl in [ComputeAlphaBetaHow.CLASSIC, ComputeAlphaBetaHow.SYMBOLIC,
                             ComputeAlphaBetaHow.SYMBOLIC_LOG_SEMIRING]:
                    # Initialize model parameters randomly

                    # Load and instantiate the model
                    m = loadPrism("examples/materials/tandem_3.sm")
                    m.instantiate(["mu1a", "mu1b", "mu2", "kappa"], initial_params)

                    # Run the learning process
                    tandem_params, tandem_result = BW().fit_parameters(
                        ts1, m, ["mu1a", "mu1b", "mu2", "kappa"],
                        return_data=True, compute_alpha_beta_how=impl
                    )

                    # Collect results
                    result_entry = {
                        "run": r,
                        "timed": timed,
                        "implementation": impl.name,
                        "c_value": k,
                        "learning_time": tandem_result['learning_time'],
                        "learning_rounds": tandem_result['learning_rounds'],
                        "mu1a": tandem_params['mu1a'],
                        "mu1b": tandem_params['mu1b'],
                        "mu2": tandem_params['mu2'],
                        "kappa": tandem_params['kappa']
                    }
                    learning_data.append(pd.DataFrame([result_entry]))
            k += steps

    return [], learning_data


if __name__ == '__main__':
    experiment3(sys.argv)

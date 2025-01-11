import sys
from jajapy import loadSet, loadPrism, BW
from numpy import array
import jajapy as ja
from datetime import datetime
import pandas as pd
import string
import random

def eksperiment():
    
    alphabet = ['a', 'b', 'c', 'd', 'e']
    results = pd.DataFrame(
    columns=['num_states', 'learning_time'])

    for states in range(20, 1021, 100):
        training_set = loadSet("training_sets/observations.txt")
        hypothesis = ja.loadHMM("training_sets/initial_hypothesis_" + str(states) + "states.txt")
        output_model, model_results = BW().fit(training_set=training_set, initial_model=hypothesis, return_data=True)
        
        model_dataframe = pd.DataFrame([
             {"num_states": states, "learning_time": model_results['learning_time']}])
        results = pd.concat([results, model_dataframe], ignore_index=True, axis=0)

    results.to_csv("experiments/results/cluster_results.csv")

    

def generate_random_hmm(num_states, num_emissions_per_state=3):
    transitions = []
    emissions = []

    # Generate random transitions
    for state in range(num_states):
        # Create random transition probabilities to other states
        trans_probs = [random.random() for _ in range(num_states)]
        total = sum(trans_probs)
        trans_probs = [p / total for p in trans_probs]  # Normalize to sum to 1

        for target_state in range(num_states):
            if trans_probs[target_state] > 0:  # Avoid zero-probability entries
                transitions.append((state, target_state, trans_probs[target_state]))

    # Generate random emissions
    emission_labels = ['a', 'b', 'c', 'd', 'e']
    for state in range(num_states):
        # Select a subset of emission labels for the current state
        labels = random.sample(emission_labels, num_emissions_per_state)

        # Create random emission probabilities
        emission_probs = [random.random() for _ in labels]
        total = sum(emission_probs)
        emission_probs = [p / total for p in emission_probs]  # Normalize to sum to 1

        for label, prob in zip(labels, emission_probs):
            if prob > 0:  # Avoid zero-probability entries
                emissions.append((state, label, prob))

    return transitions, emissions


def generate_initial_hypothesis(num_states, alphabet):
    
    for i in range(20, 1021, 100):
        initial_hypothesis = ja.HMM_random(i, alphabet=alphabet, random_initial_state=False)
        initial_hypothesis.save("initial_hypothesis_" + str(i) + "states.txt")
        print("Initial hypothesis with", i, "states saved to file")


if __name__ == "__main__":
    eksperiment()
    # generate_initial_hypothesis(5, ['a', 'b', 'c', 'd', 'e'])
    # transitions, emissions = generate_random_h
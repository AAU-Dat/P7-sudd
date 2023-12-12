import csv

# Specify the path to your input text file
input_file_path = '../SUDD_results/results_philosophers2_untimed_log.txt'

# Read the data from the input file
with open(input_file_path, 'r') as file:
    data = file.read()
# Split the data into individual runs
runs = data.split('})\n')
#runs.pop()
print(f"Number of runs: {len(runs)}")
# Create a CSV file and write the header
with open('results_philosophers2_untimed_log.csv', 'w', newline='') as csvfile:
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow(['Run', 'alpha', 'betax', 'gammax', 'zetax', 'Learning Rounds', 'Learning Time', 'Training Set Loglikelihood'])

    for run in runs:
        # Extract relevant information using regular expressions
        run_number = run.split('\n')[0]
        alpha = float(run.split("'alpha': ")[1].split(",")[0])
        betax = float(run.split("'betax': ")[1].split(",")[0])
        gammax = float(run.split("'gammax': ")[1].split(",")[0])
        zetax = float(run.split("'zetax': ")[1].split("}")[0])
        learning_rounds = int(run.split("'learning_rounds': ")[1].split(",")[0])
        learning_time = float(run.split("'learning_time': ")[1].split(",")[0])
        training_set_loglikelihood = float(run.split("'training_set_loglikelihood': ")[1].split("}")[0])

        # Write the data to the CSV file
        csvwriter.writerow([run_number, alpha, betax, gammax, zetax, learning_rounds, learning_time, training_set_loglikelihood])

print("CSV file created successfully.")

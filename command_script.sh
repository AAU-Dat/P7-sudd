#python3 -m memray run -o polling_timed_memory.bin polling_timed_script.py
#python3 -m memray run -o polling_untimed_memory.bin polling_untimed_script.py

#python3 -m memray flamegraph polling_timed_memory.bin
#python3 -m memray flamegraph polling_untimed_memory.bin

#RUN THESE!!!

python3 experiments/experiment_polling.py
#python3 experiments/experiment_cluster.py
#python3 experiments/experiment_tandem.py
#python3 experiments/experiment_philosophers.py
#python3 experiments/experiment_philosophers2.py

#python3 test.py

echo "Done"



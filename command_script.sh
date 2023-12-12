python3 -m memray run -o polling_timed_memory.bin polling_timed_script.py
python3 -m memray run -o polling_untimed_memory.bin polling_untimed_script.py

python3 -m memray run -o cluster_timed_memory.bin cluster_timed_script.py
python3 -m memray run -o cluster_untimed_memory.bin cluster_untimed_script.py

python3 -m memray run -o tandem_timed_memory.bin tandem_timed_script.py
python3 -m memray run -o tandem_untimed_memory.bin tandem_untimed_script.py

python3 -m memray run -o philosophers_timed_memory.bin philosophers_timed_script.py
python3 -m memray run -o philosophers_untimed_memory.bin philosophers_untimed_script.py

python3 -m memray flamegraph polling_timed_memory.bin
python3 -m memray flamegraph polling_untimed_memory.bin

python3 -m memray flamegraph cluster_timed_memory.bin
python3 -m memray flamegraph cluster_untimed_memory.bin

python3 -m memray flamegraph tandem_timed_memory.bin
python3 -m memray flamegraph tandem_untimed_memory.bin

python3 -m memray flamegraph philosophers_timed_memory.bin
python3 -m memray flamegraph philosophers_untimed_memory.bin

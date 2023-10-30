import ctypes
from typing import Tuple, Any
import numpy as np
import os

module = ctypes.CDLL("build/sudd.so")


def symbolic_compute_alphas_timed(
    phi: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
    n_states: int,
    n_obs: int
) -> np.ndarray[np.float64, Tuple[int, int]]:
    phi = numpy_to_file(phi, f"phi{os.getpid()}")
    tau = numpy_to_file(tau, f"tau{os.getpid()}")
    pi = numpy_to_file(pi, f"pi{os.getpid()}")
    return module.file_forwards(phi, tau, pi)


def symbolic_compute_betas_timed(
    phi: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
    n_states: int,
    n_obs: int
) -> np.ndarray[np.float64, Tuple[int, int]]:
    phi = numpy_to_file(phi, f"phi{os.getpid()}")
    tau = numpy_to_file(tau, f"tau{os.getpid()}")
    pi = numpy_to_file(pi, f"pi{os.getpid()}")
    return module.file_backwards(phi, tau, pi)


def numpy_to_file(
    arr: np.ndarray[Any, np.dtype[Any]],
    file: str
) -> ctypes.c_char_p:
    # c_char_p = ctypes.c_char_p(python_string.encode('utf-8'))
    NotImplemented

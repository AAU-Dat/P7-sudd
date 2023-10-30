import ctypes
from typing import List

module = ctypes.CDLL("build/sudd.so")


def symbolic_compute_alphas_timed(
    phi: List[List[float]],
    tau: List[List[float]],
    pi: List[float],
    n_states: int,
    n_obs: int
) -> List[List[float]]:
    NotImplemented


def symbolic_compute_betas_timed(
    phi: List[List[float]],
    tau: List[List[float]],
    pi: List[float],
    n_states: int,
    n_obs: int
) -> List[List[float]]:
    NotImplemented


def python_list_to_double_pointer(matrix):
    row_ptrs = []
    for row in matrix:
        row_array = (ctypes.c_double * len(row))(*row)
        row_ptrs.append(row_array)
    return (ctypes.POINTER(ctypes.c_double) * len(matrix))(*row_ptrs)

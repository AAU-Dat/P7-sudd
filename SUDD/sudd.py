import ctypes
from typing import Tuple, Any
import numpy as np
import os

lib = ctypes.CDLL("build/sudd.so")

lib.file_forwards.argtypes = [
        ctypes.POINTER(ctypes.c_char_p),
        ctypes.c_char_p,
        ctypes.c_char_p,
        ctypes.c_int,
        ctypes.c_int
    ]
lib.file_backwards.argtypes = [
        ctypes.POINTER(ctypes.c_char_p),
        ctypes.c_char_p,
        ctypes.c_char_p,
        ctypes.c_int,
        ctypes.c_int
    ]
lib.file_forwards.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))
lib.file_backwards.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))


def symbolic_forwards_timed(
    phis: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
) -> np.ndarray[np.float64, Tuple[int, int]]:
    return symbolic_fb_timed(
        lib.file_forwards,
        phis,
        tau,
        pi
    )


def symbolic_backwards_timed(
    phis: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
) -> np.ndarray[np.float64, Tuple[int, int]]:
    return symbolic_fb_timed(
        lib.file_backwards,
        phis,
        tau,
        pi
    )


def symbolic_fb_timed(
    fb: Any,
    phis: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
) -> np.ndarray[np.float64, Tuple[int, int]]:

    n_obs, n_states = phis.shape

    # convert / make files
    phi_file_names = [
        f"phi_{i}_{os.getpid()}"
        for i, phi in enumerate(phis)
    ]
    phis = [
        numpy_to_file(phi, f"phi_{i}_{os.getpid()}")
        for i, phi in enumerate(phis)
    ]
    # TODO len(phis) might be wrong
    phis_p = (ctypes.c_char_p * len(phis))(*phis)

    tau_file_name = f"tau_{os.getpid()}"
    tau = numpy_to_file(tau, tau_file_name)

    pi_file_name = f"pi_{os.getpid()}"
    pi = numpy_to_file(pi, pi_file_name)

    # TODO free ab
    ab_p = fb(phis_p, tau, pi, n_states, n_obs - 1)

    # convert ab to nparray
    ab = c_array_to_numpy_array(ab_p, n_obs, n_states)

    # cleanup
    for phi_file_name in phi_file_names:
        os.remove(phi_file_name)
    os.remove(tau_file_name)
    os.remove(pi_file_name)

    for i in range(n_obs):
        ctypes.CDLL('libc.so.6').free(ab_p[i])
    ctypes.CDLL('libc.so.6').free(ab_p)

    return ab


def c_array_to_numpy_array(c_array, rows, cols):
    # Create an uninitialized array of given shape and type
    np_array = np.empty((rows, cols), dtype=np.float64)
    for i in range(rows):
        for j in range(cols):
            np_array[i, j] = c_array[i][j]
    return np_array


def numpy_to_file(array: np.ndarray, filename: str) -> ctypes.c_char_p:
    with open(filename, 'w') as f:
        if array.ndim == 1:
            f.write(f"{array.shape[0]} 1\n")
            for (i,), value in np.ndenumerate(array):
                if value != 0:
                    f.write(f"{i} 0 {value}\n")
        elif array.ndim == 2:
            f.write(f"{array.shape[0]} {array.shape[1]}\n")
            for (i, j), value in np.ndenumerate(array):
                if value != 0:
                    f.write(f"{i} {j} {value}\n")
        else:
            raise ValueError("The input array must be one or two-dimensional")

    return ctypes.c_char_p(filename.encode('utf-8'))

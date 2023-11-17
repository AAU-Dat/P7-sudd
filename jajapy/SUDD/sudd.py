import ctypes
from typing import Tuple, Any
import numpy as np
import os
import math

from numpy._typing import NDArray

lib_path = os.path.join(os.path.dirname(__file__), 'build', 'sudd.so')
lib = ctypes.CDLL(lib_path)

lib.forwards.argtypes = [
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous'),
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous'),
        np.ctypeslib.ndpointer(dtype=float, ndim=1, flags='aligned, contiguous'),
        ctypes.c_int,
        ctypes.c_int,
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous, writeable'),
    ]
lib.backwards.argtypes = [
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous'),
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous'),
        np.ctypeslib.ndpointer(dtype=float, ndim=1, flags='aligned, contiguous'),
        ctypes.c_int,
        ctypes.c_int,
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous, writeable'),
    ]
lib.forwards.restype = ctypes.c_int
lib.backwards.restype = ctypes.c_int
lib.log_forwards.argtypes = [
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous'),
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous'),
        np.ctypeslib.ndpointer(dtype=float, ndim=1, flags='aligned, contiguous'),
        ctypes.c_int,
        ctypes.c_int,
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous, writeable'),
    ]
lib.log_backwards.argtypes = [
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous'),
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous'),
        np.ctypeslib.ndpointer(dtype=float, ndim=1, flags='aligned, contiguous'),
        ctypes.c_int,
        ctypes.c_int,
        np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous, writeable'),
    ]
lib.log_forwards.restype = ctypes.c_int
lib.log_backwards.restype = ctypes.c_int


def forwards_symbolic(
    phis: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
) -> np.ndarray[np.float64, Tuple[int, int]]:
    return fb_symbolic(
        lib.forwards,
        phis,
        tau,
        pi
    )


def backwards_symbolic(
    phis: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
) -> np.ndarray[np.float64, Tuple[int, int]]:
    return fb_symbolic(
        lib.backwards,
        phis,
        tau,
        pi
    )


def forwards_log_symbolic(
    phis: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
) -> np.ndarray[np.float64, Tuple[int, int]]:
    return fb_symbolic(
        lib.log_forwards,
        phis,
        tau,
        pi
    )


def backwards_log_symbolic(
    phis: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
) -> np.ndarray[np.float64, Tuple[int, int]]:
    return fb_symbolic(
        lib.log_backwards,
        phis,
        tau,
        pi
    )


def fb_symbolic(
    fb: Any,
    phi: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
) -> np.ndarray[np.float64, Tuple[int, int]]:
    phi = phi.astype(np.float64)
    tau = tau.astype(np.float64)
    pi = pi.astype(np.float64)
    n_obs, n_states = phi.shape
    alpha = np.zeros((n_obs + 1, n_states))
    err = fb(phi, tau, pi, n_states, n_obs, alpha)
    if err == 0:
        pass
    elif err == 1:
        raise Exception("Inconsistencies occurred in cudd manager")
    else:
        raise Exception("Cudd manager ran out memory")
    return alpha


def forwards_numeric(
    omega: np.ndarray[Any, np.dtype[Any]],
    P: np.ndarray[Any, np.dtype[Any]],
    pi: np.ndarray[Any, np.dtype[Any]],
) -> np.ndarray[Any, np.dtype[Any]]:
    n_obs, n_states = omega.shape
    alpha = np.empty((n_obs + 1, n_states))
    alpha[0] = pi
    for t in range(1, n_obs + 1):
        for s in range(n_states):
            temp = 0
            for ss in range(n_states):
                temp += P[ss][s] * omega[t-1][ss] * alpha[t-1][ss]
            alpha[t][s] = temp
    return alpha


def forwards_log_semiring(
    omega,
    P,
    pi
):
    n_obs, n_states = omega.shape
    omega = np.log(omega)
    P = np.log(P)
    pi = np.log(pi)
    alpha = np.empty((n_obs + 1, n_states))
    alpha[0] = pi
    for t in range(1, n_obs + 1):
        for s in range(n_states):
            temp = P[0][s] + omega[t-1][0] + alpha[t-1][0]
            for ss in range(1, n_states):
                temp = log_add(temp, P[ss][s] + omega[t-1][ss] + alpha[t-1][ss])
            alpha[t][s] = temp
    alpha = np.exp(alpha)
    return alpha


def forwards_matrix_numeric(
    omega: np.ndarray[Any, np.dtype[Any]],
    P: np.ndarray[Any, np.dtype[Any]],
    pi: np.ndarray[Any, np.dtype[Any]],
) -> np.ndarray[Any, np.dtype[Any]]:
    n_obs, n_states = omega.shape
    alpha = np.empty((n_obs + 1, n_states))
    alpha[0] = pi
    for t in range(1, n_obs + 1):
        alpha[t] = P.T @ (omega[t - 1] * alpha[t - 1])
    return alpha


def backwards_numeric(
    omega: np.ndarray[Any, np.dtype[Any]],
    P: np.ndarray[Any, np.dtype[Any]],
    pi: np.ndarray[Any, np.dtype[Any]],
) -> np.ndarray[Any, np.dtype[Any]]:
    n_obs, n_states = omega.shape
    beta = np.empty((n_obs + 1, n_states))
    beta[-1] = 1
    for t in range(n_obs - 1, -1, -1):
        for s in range(n_states):
            temp = 0
            for ss in range(n_states):
                temp += beta[t+1][ss] * P[s][ss]
            beta[t][s] = omega[t][s] * temp
    return beta


def backwards_log_semiring(
    omega,
    P,
    pi
):
    n_obs, n_states = omega.shape
    omega = np.log(omega)
    P = np.log(P)
    pi = np.log(pi)
    beta = np.empty((n_obs + 1, n_states))
    beta[-1] = 0
    for t in range(n_obs - 1, -1, -1):
        for s in range(n_states):
            temp = beta[t+1][0] + P[s][0]
            for ss in range(1, n_states):
                temp = log_add(temp, beta[t+1][ss] + P[s][ss])
            beta[t][s] = omega[t][s] + temp
    beta = np.exp(beta)
    return beta


def backwards_matrix_numeric(
    omega: np.ndarray[Any, np.dtype[Any]],
    P: np.ndarray[Any, np.dtype[Any]],
    pi: np.ndarray[Any, np.dtype[Any]],
) -> np.ndarray[Any, np.dtype[Any]]:
    n_obs, n_states = omega.shape
    beta = np.empty((n_obs + 1, n_states))
    beta[-1] = 1
    for t in range(n_obs - 1, -1, -1):
        beta[t] = omega[t] * (P @ beta[t + 1])
    return beta

def log_add(x, y):
    """ Perform log-space addition: log(exp(x) + exp(y)) """
    # Ensuring x is the larger number for numerical stability
    if y > x:
        x, y = y, x
    if x == float('-inf'):
        return x
    return x + math.log1p(math.exp(y - x))

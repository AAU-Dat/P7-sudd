import ctypes
import math
import os
from typing import Any, Tuple

import numpy as np
from numpy.typing import NDArray

lib_path = os.path.join(os.path.dirname(__file__), 'build', 'sudd.so')
lib = ctypes.CDLL(lib_path)


def set_function_types(func, argtypes, restype=ctypes.c_int):
    func.argtypes = argtypes
    func.restype = restype


common_argtypes = [
    np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous'),
    np.ctypeslib.ndpointer(dtype=float, ndim=2, flags='aligned, contiguous'),
    np.ctypeslib.ndpointer(dtype=float, ndim=1, flags='aligned, contiguous'),
    ctypes.c_int,
    ctypes.c_int,
    np.ctypeslib.ndpointer(dtype=float, ndim=2,
                           flags='aligned, contiguous, writeable'),
]


set_function_types(lib.forwards, common_argtypes)
set_function_types(lib.backwards, common_argtypes)
set_function_types(lib.log_forwards, common_argtypes)
set_function_types(lib.log_backwards, common_argtypes)
set_function_types(lib.forwards_numeric_c, common_argtypes)
set_function_types(lib.backwards_numeric_c, common_argtypes)


def forwards_symbolic(
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray,
) -> NDArray[np.float64]:
    return fb_symbolic(
        lib.forwards,
        omega,
        p,
        pi
    )


def backwards_symbolic(
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray,
) -> NDArray[np.float64]:
    return fb_symbolic(
        lib.backwards,
        omega,
        p,
        pi
    )


def forwards_log_symbolic(
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray,
) -> NDArray[np.float64]:
    return fb_symbolic(
        lib.log_forwards,
        omega,
        p,
        pi
    )


def backwards_log_symbolic(
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray,
) -> NDArray[np.float64]:
    return fb_symbolic(
        lib.log_backwards,
        omega,
        p,
        pi
    )


def fb_symbolic(
    fb: Any,
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray,
) -> NDArray[np.float64]:
    omega, p, pi = sanitize(omega, p, pi)
    n_obs, n_states = omega.shape
    alpha = np.zeros((n_obs + 1, n_states))
    err = fb(omega, p, pi, n_states, n_obs, alpha)
    if err == 0:
        pass
    elif err == 1:
        raise Exception("Inconsistencies occurred in cudd manager")
    else:
        raise Exception("Cudd manager ran out memory")
    return alpha


def forwards_numeric(
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray,
) -> NDArray[np.float64]:
    omega, p, pi = sanitize(omega, p, pi)
    n_obs, n_states = omega.shape
    alpha = np.empty((n_obs + 1, n_states))
    alpha[0] = pi
    for t in range(1, n_obs + 1):
        for s in range(n_states):
            temp = 0
            for ss in range(n_states):
                temp += p[ss][s] * omega[t-1][ss] * alpha[t-1][ss]
            alpha[t][s] = temp
    return alpha


def forwards_log_semiring(
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray,
) -> NDArray[np.float64]:
    omega, p, pi = sanitize(omega, p, pi)
    n_obs, n_states = omega.shape
    omega = np.log(omega)
    p = np.log(p)
    pi = np.log(pi)
    alpha = np.empty((n_obs + 1, n_states))
    alpha[0] = pi
    for t in range(1, n_obs + 1):
        for s in range(n_states):
            temp = p[0][s] + omega[t-1][0] + alpha[t-1][0]
            for ss in range(1, n_states):
                temp = log_add(temp, p[ss][s] + omega[t-1]
                               [ss] + alpha[t-1][ss])
            alpha[t][s] = temp
    alpha = np.exp(alpha)
    return alpha


def forwards_matrix_numeric(
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray,
) -> NDArray[np.float64]:
    omega, p, pi = sanitize(omega, p, pi)
    n_obs, n_states = omega.shape
    alpha = np.empty((n_obs + 1, n_states))
    alpha[0] = pi
    for t in range(1, n_obs + 1):
        alpha[t] = p.T @ (omega[t - 1] * alpha[t - 1])
    return alpha


def backwards_numeric(
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray,
) -> NDArray[np.float64]:
    omega, p, pi = sanitize(omega, p, pi)
    n_obs, n_states = omega.shape
    beta = np.empty((n_obs + 1, n_states))
    beta[-1] = 1
    for t in range(n_obs - 1, -1, -1):
        for s in range(n_states):
            temp = 0
            for ss in range(n_states):
                temp += beta[t+1][ss] * p[s][ss]
            beta[t][s] = omega[t][s] * temp
    return beta


def backwards_log_semiring(
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray,
) -> NDArray[np.float64]:
    omega, p, pi = sanitize(omega, p, pi)
    n_obs, n_states = omega.shape
    omega = np.log(omega)
    p = np.log(p)
    pi = np.log(pi)
    beta = np.empty((n_obs + 1, n_states))
    beta[-1] = 0
    for t in range(n_obs - 1, -1, -1):
        for s in range(n_states):
            temp = beta[t+1][0] + p[s][0]
            for ss in range(1, n_states):
                temp = log_add(temp, beta[t+1][ss] + p[s][ss])
            beta[t][s] = omega[t][s] + temp
    beta = np.exp(beta)
    return beta


def backwards_matrix_numeric(
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray,
) -> NDArray[np.float64]:
    omega, p, pi = sanitize(omega, p, pi)
    n_obs, n_states = omega.shape
    beta = np.empty((n_obs + 1, n_states))
    beta[-1] = 1
    for t in range(n_obs - 1, -1, -1):
        beta[t] = omega[t] * (p @ beta[t + 1])
    return beta

def forwards_numeric_c(
    phis: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
) -> np.ndarray[np.float64, Tuple[int, int]]:
    return fb_numeric_c(
        lib.forwards_numeric_c,
        phis,
        tau,
        pi
    )
    
def backwards_numeric_c(
    phis: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
) -> np.ndarray[np.float64, Tuple[int, int]]:
    return fb_numeric_c(
        lib.backwards_numeric_c,
        phis,
        tau,
        pi
    )

def fb_numeric_c(
    fw: Any,
    phi: np.ndarray[np.float64, Tuple[int, int]],
    tau: np.ndarray[np.float64, Tuple[int, int]],
    pi: np.ndarray[np.float64, Tuple[int]],
) -> np.ndarray[np.float64, Tuple[int, int]]:
    phi = phi.astype(np.float64)
    tau = tau.astype(np.float64)
    pi = pi.astype(np.float64)
    n_obs, n_states = phi.shape
    alpha = np.zeros((n_obs + 1, n_states))
    err = fw(phi, tau, pi, n_states, n_obs, alpha)
    if err == 0:
        pass
    elif err == 1:
        raise Exception("Inconsistencies occurred in cudd manager")
    else:
        raise Exception("Cudd manager ran out memory")
    return alpha

def sanitize(
    omega: np.ndarray,
    p: np.ndarray,
    pi: np.ndarray
) -> Tuple[NDArray[np.float64], NDArray[np.float64], NDArray[np.float64]]:
    assert omega.ndim == 2
    assert p.ndim == 2
    assert pi.ndim == 1

    assert p.shape[0] == omega.shape[1]
    assert p.shape[1] == omega.shape[1]
    assert pi.shape[0] == omega.shape[1]

    omega = omega.astype(np.float64)
    p = p.astype(np.float64)
    pi = pi.astype(np.float64)

    return omega, p, pi


def log_add(x, y):
    """ Perform log-space addition: log(exp(x) + exp(y)) """
    # Ensuring x is the larger number for numerical stability
    if y > x:
        x, y = y, x
    if x == float('-inf'):
        return x
    return x + math.log1p(math.exp(y - x))

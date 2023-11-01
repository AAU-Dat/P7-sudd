import numpy as np

from .. import sudd


def test_symbolic_forwards_timed():
    # Arrange
    tau = np.array([[1, 2, 3],
                    [4, 5, 6],
                    [7, 8, 9]])

    phi = np.array([[1, 2, 3],
                    [4, 5, 6],
                    [7, 8, 9]])

    pi = np.array([1, 2, 3])

    # Act
    alpha = sudd.symbolic_forwards_timed(phi, tau, pi)

    # Assert
    for s in range(3):
        assert pi[s] * phi[0][s] == alpha[0][s]
        assert phi[1][s] * (tau[0][s] * alpha[0][0] + tau[1][s] * alpha[0][1] + tau[2][s] * alpha[0][2]) == alpha[1][s]
        phi[2][s] * (tau[0][s] * alpha[1][0] + tau[1][s] * alpha[1][1] + tau[2][s] * alpha[1][2]) == alpha[2][s]


def test_symbolic_backwards_timed():
    # Arrange
    tau = np.array([[1, 2, 3],
                    [4, 5, 6],
                    [7, 8, 9]])

    phi = np.array([[1, 2, 3],
                    [4, 5, 6],
                    [7, 8, 9]])

    pi = np.array([1, 2, 3])

    # Act
    beta = sudd.symbolic_backwards_timed(phi, tau, pi)

    # Assert
    for s in range(3):
        assert tau[s][0] * phi[1][0] * beta[1][0] + tau[s][1] * phi[1][1] * beta[1][1] + tau[s][2] * phi[1][2] * beta[1][2] == beta[0][s]
        assert tau[s][0] * phi[2][0] * beta[2][0] + tau[s][1] * phi[2][1] * beta[2][1] + tau[s][2] * phi[2][2] * beta[2][2] == beta[1][s]
        assert 1 == beta[2][s]

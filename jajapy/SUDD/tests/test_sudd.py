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
    alpha = sudd.forwards_symbolic(phi, tau, pi)

    # Assert
    assert (alpha == sudd.forwards_numeric(phi, tau, pi)).all
    assert (alpha == sudd.forwards_matrix_numeric(phi, tau, pi)).all


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
    beta = sudd.backwards_symbolic(phi, tau, pi)

    # Assert
    assert (beta == sudd.backwards_numeric(phi, tau, pi)).all
    assert (beta == sudd.backwards_matrix_numeric(phi, tau, pi)).all

import numpy as np
from .. import sudd


def setup_test_variables():
    tau = np.array([[1, 2, 3],
                    [4, 5, 6],
                    [7, 8, 9]], dtype=np.float64)

    phi = np.array([[1, 2, 3],
                    [4, 5, 6],
                    [7, 8, 9]], dtype=np.float64)

    pi = np.array([1, 2, 3], dtype=np.float64)

    return tau, phi, pi


def test_symbolic_forwards_timed():
    # Arrange
    tau, phi, pi = setup_test_variables()

    # Act
    actual = sudd.forwards_symbolic(phi, tau, pi)

    # Assert
    expected = sudd.forwards_matrix_numeric(phi, tau, pi)
    assert np.array_equal(actual, expected)


def test_symbolic_backwards_timed():
    # Arrange
    tau, phi, pi = setup_test_variables()

    # Act
    actual = sudd.backwards_symbolic(phi, tau, pi)

    # Assert
    expected = sudd.backwards_matrix_numeric(phi, tau, pi)
    expected2 = sudd.backwards_numeric(phi, tau, pi)
    assert np.array_equal(actual, expected)
    assert np.array_equal(actual, expected2)

    
def test_numeric_forwards_timed_c():
    # Arrange
    tau, phi, pi = setup_test_variables() 

    # Act
    alpha = sudd.forwards_numeric_c(phi, tau, pi)

    # Assert
    assert np.array_equal(alpha, sudd.forwards_numeric(phi, tau, pi))
    assert np.array_equal(alpha, sudd.forwards_matrix_numeric(phi, tau, pi))
    

def test_numeric_backwards_timed_c():
    # Arrange
    tau, phi, pi = setup_test_variables()

    # Act
    beta = sudd.backwards_numeric_c(phi, tau, pi)

    # Assert
    assert np.array_equal(beta, sudd.backwards_numeric(phi, tau, pi))
    assert np.array_equal(beta, sudd.backwards_matrix_numeric(phi, tau, pi))


def test_log_symbolic_forwards_timed():
    # Arrange
    tau, phi, pi = setup_test_variables()

    # Act
    actual = sudd.forwards_log_symbolic(phi, tau, pi)

    # Assert
    expected = sudd.forwards_log_semiring(phi, tau, pi)
    assert np.allclose(actual, expected)


def test_log_symbolic_backwards_timed():
    # Arrange
    tau, phi, pi = setup_test_variables()

    # Act
    actual = sudd.backwards_log_symbolic(phi, tau, pi)

    # Assert
    expected = sudd.backwards_log_semiring(phi, tau, pi)
    assert np.array_equal(actual, expected)

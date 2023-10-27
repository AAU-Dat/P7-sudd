from ctypes import *
import ctypes
import os
import sys
import numpy as np

p = np.array([[1,2,3], [4, 5, 6], [7, 8, 9]], dtype=np.double)
p = p.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
omega =np.array([[1,2,3], [4, 5, 6], [7, 8, 9]], dtype=np.double)
omega = omega.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
pi = np.array([1,2,3], dtype=np.double)
pi = pi.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

testmod = CDLL("/usr/local/lib/libcudd-3.0.0.so.0")
module = CDLL("../build/lib.so")

forwards = module.forwards
forwards.restype = POINTER(c_double)
forwards.argtypes = [POINTER(c_double), POINTER(c_double), POINTER(c_double), c_int, c_int]

forwards(p, omega, pi, 3, 2)
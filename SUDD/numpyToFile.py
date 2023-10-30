import numpy as np
import ctypes

def numpyToFile(array, filename):
    with open(filename, 'w') as f:
        f.write(str(array.shape[0]) + ' ' + str(array.shape[1]) + '\n')
        for i in range(array.shape[0]):
            for j in range(array.shape[1]):
                if array[i, j] != 0:
            # Write the element along with its indices
                    f.write(f"{i} {j} {array[i, j]}\n")
    f.close()
    return ctypes.c_char_p(filename.encode('utf-8'))

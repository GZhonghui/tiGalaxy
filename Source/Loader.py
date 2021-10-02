import numpy as np
from ctypes import *

starCount = (c_int*1)(0)

try:
    Lib = cdll.LoadLibrary('./Loader.so')
except Exception as Error:
    print('Make First')
    exit(0)
else:
    libReset = Lib.Reset
    libReset.argtypes = None
    libReset.restype  = None

    libLoad = Lib.Load
    libLoad.argtypes = (c_char_p, POINTER(c_int))
    libLoad.restype  = None

    libFill = Lib.Fill
    libFill.argtypes = (POINTER(c_float),)
    libFill.restype  = None

def Reset():
    libReset()

def Load(fileName: str):
    libLoad(fileName.encode(), starCount)

def Fill():
    Data = np.ndarray(dtype=np.float32, shape=(starCount[0],7))
    libFill(Data.ctypes.data_as(POINTER(c_float)))
    return starCount[0],Data

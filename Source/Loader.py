import numpy as np
from ctypes import *

starCount = (c_int*1)(0)

libName = './Loader.dll'

try:
    Lib = cdll.LoadLibrary(libName)
except Exception as Error:
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
    return int(starCount[0]),Data

import numpy as np
import ctypes

try:
    Lib = ctypes.cdll.LoadLibrary('./Loader.so')
except Exception as Error:
    print('Make First')
    exit(0)
else:
    libReset = Lib.Reset
    libReset.argtypes = None
    libReset.restype  = None

    libLoad = Lib.Load
    libLoad.argtypes = (ctypes.c_char_p,)
    libLoad.restype  = None

    libFill = Lib.Fill
    libFill.argtypes = (ctypes.c_int_p, ctypes.c_float_p)
    libFill.restype  = None

def Reset():
    libReset()

def Load(fileName: str):
    libLoad(fileName.encode())

def Fill():
    return 0

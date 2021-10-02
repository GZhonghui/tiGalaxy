import numpy as np
import ctypes

try:
    Lib = ctypes.cdll.LoadLibrary('./Loader.so')
except Exception as Error:
    print('Make First')
    print(Error)
else:
    libLoad = Lib.Load
    libLoad.argtypes = (ctypes.c_char_p,)
    libLoad.restype  = None

def Load():
    libLoad(b'Galaxy20K.bin')


Load()

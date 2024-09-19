import ctypes
import numpy as np
import matplotlib.pyplot as plt
import cv2
import time
from functools import wraps

def timing_decorator(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        start_time = time.time()  
        result = func(*args, **kwargs)  
        end_time = time.time()  
        print(f"{func.__name__} running time: {end_time - start_time} second(s)")
        return result
    return wrapper

@timing_decorator
def Gabor_filter(gabor_lib,image_ptr,M,N,Orientation,Frequency,kernal_size):
    gabor_lib.gabor_filter(image_ptr, M, N, Orientation,Frequency,kernal_size)


gabor_lib = ctypes.cdll.LoadLibrary("./gabor_filter.dll")
gabor_lib.gabor_filter.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int, ctypes.c_int, ctypes.c_float, ctypes.c_float, ctypes.c_int]



image = np.random.rand(120, 160).astype(np.float32)  # example matrix
store_image = np.copy(image)
# get np.array pointer
image_ptr = image.ctypes.data_as(ctypes.POINTER(ctypes.c_float))

# use Cpp DLL
M, N = image.shape
print(image.shape)
Orientation = np.float32(np.pi /2)
Frequency = np.float32(8)
kernal_size = 8

try:
    Gabor_filter(gabor_lib,image_ptr, M, N, Orientation,Frequency,kernal_size)
except Exception as e:
    print(f"Error: {e}")

# open filtered Image
fig, axs = plt.subplots(1,2)
axs[1].imshow(image)
axs[0].imshow(store_image)
plt.show()

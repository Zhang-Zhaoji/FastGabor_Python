import ctypes
import numpy as np
import matplotlib.pyplot as plt
import time
from functools import wraps

def timing_decorator(func):
    """timer"""
    @wraps(func)
    def wrapper(*args, **kwargs):
        start_time = time.time()  
        result = func(*args, **kwargs)  
        end_time = time.time()  
        print(f"{func.__name__} running time: {end_time - start_time} second(s)")
        return result
    return wrapper

@timing_decorator
def specific_Gabor_filter(gabor_lib,image_ptr,M,N,Orientation,Frequency,kernal_size):
    """use dll to compute, you may check the computation cost here."""
    gabor_lib.gabor_filter(image_ptr, M, N, Orientation,Frequency,kernal_size)

def initialize_gabor_lib(path = "./gabor_filter.dll"):
    """ 
    if you edited my code, and you find that the code raise an error that it could not find the dll file, it might be triggerd by C++ function or class in the cpp file like iostream/string/vector/...

    use ctypes.POINTER(ctypes.c_float), ctypes.c_int, ctypes.c_int, ctypes.c_float, ctypes.c_float, ctypes.c_int as input datatype.
    
    return a gabor_lib, use it just like: gabor_lib.gabor_filter(image_ptr, int: M, int: N, float: Orientation, float: Frequency, int: kernal_size)
    """
    if path[-4::] != ".dll":
        raise AssertionError("maybe you should check if you use the right pathway. the end of the path is not .dll.")
    gabor_lib = ctypes.cdll.LoadLibrary(path)
    gabor_lib.gabor_filter.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int, ctypes.c_int, ctypes.c_float, ctypes.c_float, ctypes.c_int]
    return gabor_lib


def gabor_filter(gabor_lib,image, Orientation, Frequency,kernal_size):
    """ the core function. input gabor lib, the image and other essential parameters so the code can run."""
    image_ptr = image.ctypes.data_as(ctypes.POINTER(ctypes.c_float))
    # use Cpp DLL
    M, N = image.shape
    try:
        specific_Gabor_filter(gabor_lib,image_ptr, M, N, Orientation,Frequency,kernal_size)
    except Exception as e:
        print(f"Error: {e}")
        raise RuntimeError(e)

if __name__ == "__main__":
    gabor_lib = initialize_gabor_lib()
    image = np.random.rand(120, 160).astype(np.float32)  # example matrix
    store_image = np.copy(image)
    Orientation = np.float32(np.pi /4)
    Frequency = np.float32(2)
    kernal_size = 8
    gabor_filter(gabor_lib,image,Orientation, Frequency, kernal_size)
    # get np.array pointer
    # open filtered Image
    fig, axs = plt.subplots(1,2)
    axs[1].imshow(image)
    axs[0].imshow(store_image)
    plt.show()

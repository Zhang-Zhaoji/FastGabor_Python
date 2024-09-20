# FastGabor_Python

Based on J. Kim, S. Um, and D. Min, "Fast 2D Complex Gabor Filter With Kernel Decomposition," in IEEE Transactions on Image Processing, vol. 27, no. 4, pp. 1713-1722, April 2018,

I achieved **super fast** Gabor filter results:

For a 120x160 matrix, with a frequency of 8 pixels and a kernel size of 17x17 (equivalent), my code can process the matrix in just **0.0010004043579101562 seconds**.

https://github.com/user-attachments/assets/bfe9eeb8-0351-4e9e-83ef-c113a481366e

If you find any inaccuracies or have suggestions for improvement, feel free to submit a pull request or open an issue.

I've used the -O3 optimization flag, and it works well. You might also want to explore multi-processing techniques to further accelerate the processing!

**Be Careful:** the the maximum value to be send to c++ code is 1, please normalize it before use.

## Recommend Parameters:

Frequency = np.float32(np.pi**2/2)

kernal_size = 8 (it is not percisely the meaning of kernal size, maybe better description is (kernalSize - 1)/2 $\approx$ kernalSize // 2.) 

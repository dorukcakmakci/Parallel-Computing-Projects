import numpy as np
import matplotlib.pyplot as plt

# for compute sum, array elements denote 1,2,3,5,10,20,30,50,75,100 threads respectively
serial_sum = [204, 204, 204, 204, 204, 204, 204, 204, 204, 204]
paralel_v1_sum = [79, 75, 134, 101, 227, 367, 574, 1045, 1615, 2284]
paralel_v2_sum = [28, 68, 128, 196, 288, 5249, 13188, 28850, 41399, 46966]

# assuming the input matrix is 30x30, array elements denote 1, 4, 9, 25, 36, 100
serial_mm = [532, 532, 532, 532, 532, 532]
paralel_mm = [468, 1374, 2053, 14864, 18055, 86970]

x_sum = [1, 2, 3, 5, 10, 20, 30, 50, 75, 100]
x_mm = [1, 4, 9, 25, 36, 100]

# for compute sum
plt.figure()
plt.title('One-to-One Communication vs Collective Communication  vs Serial Implementation')
plt.xlabel("Thread #")
plt.ylabel('Runtime in microseconds')
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(x_sum, serial_sum, 'ro', label="Serial Compute Sum")
plt.plot(x_sum, paralel_v1_sum, 'co', label="Compute Sum With One-to-One Communication")
plt.plot(x_sum, paralel_v2_sum, 'mo', label="Compute Sum With Collective Communication")
plt.legend()
plt.show()

# for matrix multiplication
plt.figure()
plt.title('Parallel Implementation vs Serial Implementation For Matrix Multiplication')
plt.xlabel("Thread #")
plt.ylabel('Runtime in microseconds')
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(x_mm, serial_mm, 'ro', label="Serial Matrix Multiplication")
plt.plot(x_mm, paralel_mm, 'yo', label="Parallel Matrix Multiplication")
plt.legend()
plt.show()
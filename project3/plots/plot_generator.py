import numpy as np
import matplotlib.pyplot as plt
from textwrap import wrap

# accuracy wrt. K value
K = [1, 2, 5, 7, 10]
serial = parallel = [(329*100)/342, (315 * 100)/324, (269*100)/270, (233*100)/234, (100)]

plt.figure()
plt.title('\n'.join(wrap('Accuracy vs K')))
plt.xlabel("K")
plt.ylabel("Accuracy (%)")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(K, serial, 'ro', label="Sequential Implementation", alpha=0.7)
plt.legend()
plt.savefig("acc_vs_K_sequential.png")
plt.show()

plt.figure()
plt.title('\n'.join(wrap('Accuracy vs K')))
plt.xlabel("K")
plt.ylabel("Accuracy (%)")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(K, parallel, 'co', label="Parallel Implementation", alpha=0.7)
plt.legend()
plt.savefig("acc_vs_K_parallel.png")
plt.show()

# Execution Time vs K value for parallel implementation
K = [1, 2, 5, 7, 10]
serial = [0, 0, 0, 0, 0]
parallel = [1474, 1675, 2240, 2395, 2481] #num_of_threads = 6

plt.figure()
plt.title('\n'.join(wrap('Execution Time vs K for parallel implementation, num_of_threads = 6')))
plt.xlabel("K")
plt.ylabel("Execution Time (milliseconds)")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(K, serial, 'ro', label="Sequential Part Time", alpha=0.7)
plt.plot(K, parallel, 'co', label="Parallel Part Time", alpha=0.7)
plt.legend()
plt.savefig("time_vs_K_parallel_num_of_threads_6.png")
plt.show()

# Execution Time vs K value for sequential implementation
K = [1, 2, 5, 7, 10]
serial = [4738.31, 4795.7, 5226.11, 5027.90, 4926.34]
parallel = [0, 0, 0, 0, 0] 

plt.figure()
plt.title('\n'.join(wrap('Execution Time vs K for sequential implementation')))
plt.xlabel("K")
plt.ylabel("Execution Time (milliseconds)")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(K, serial, 'ro', label="Sequential Part Time", alpha=0.7)
plt.plot(K, parallel, 'co', label="Parallel Part Time", alpha=0.7)
plt.legend()
plt.savefig("time_vs_K_sequential.png")
plt.show()

# Execution Time vs num_of_threads for parallel implementation K = 5
num_of_threads = [1, 2, 4, 6, 8, 16]
serial = [0, 0, 0, 0, 0, 0]
parallel = [5061, 3011, 2311, 2240, 2293, 2151] 
sequential = 6*[4872]

plt.figure()
plt.title('\n'.join(wrap('Execution Time vs num_of_threads for parallel implementation, K = 5')))
plt.xlabel("num_of_threads")
plt.ylabel("Execution Time (milliseconds)")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(num_of_threads, serial, 'ro', label="Sequential Part Time", alpha=0.7)
plt.plot(num_of_threads, parallel, 'co', label="Parallel Part Time", alpha=0.7)
plt.plot(num_of_threads, sequential, 'go', label="Sequential Implementation Time", alpha=0.7)
plt.legend()
plt.savefig("time_vs_num_of_threads_parallel_K_5.png")
plt.show()

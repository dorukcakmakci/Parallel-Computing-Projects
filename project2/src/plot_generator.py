import numpy as np
import matplotlib.pyplot as plt
from textwrap import wrap

# x axis values
numproc = [1, 2, 5, 10, 20, 30]

# y axis values
serial = [0.032000, 0.038000, 0.037000, 0.034000, 0.036000, 0.034000]
parallel = [0.047628, 0.149745, 0.119739, 13.410217, 14.162709, 9.554851]
total = [0.079628, 0.187745, 0.156739, 13.444217, 14.198709, 9.588851]

plt.figure()
plt.title('\n'.join(wrap('Serial, Parallel and Total Runtime vs # of processes (input size = 50, K = 5, D = 3) ')))
plt.xlabel("# of processes")
plt.ylabel("Runtime in milliseconds")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numproc, serial, 'ro', label="Serial Part Runtime", alpha=0.5)
plt.plot(numproc, parallel, 'co', label="Parallel Part Runtime", alpha=0.5)
plt.plot(numproc, total, 'mo', label="Total Runtime", alpha=0.5)
plt.legend()
plt.savefig("input_size_50_K_5_D_3.png")
plt.show()


# x axis values
D = [2, 3, 4, 5]

# y axis values
serial = [0.023000, 0.023000, 0.023000, 0.023000]
parallel = [0.099408, 0.047676, 0.229113, 0.049573]
total = [0.122408, 0.070676, 0.252113, 0.072573]

plt.figure()
plt.title('\n'.join(wrap('Serial, Parallel and Total Runtime vs Dictionary Size (input size = 10, K = 5, # of processes = 2) ')))
plt.xlabel("Dictionary Size")
plt.ylabel("Runtime in milliseconds")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(D, serial, 'ro', label="Serial Part Runtime", alpha=0.5)
plt.plot(D, parallel, 'co', label="Parallel Part Runtime", alpha=0.5)
plt.plot(D, total, 'mo', label="Total Runtime", alpha=0.5)
plt.legend()
plt.savefig("input_size_10_K_5_numproc_2.png")
plt.show()

# x axis values
K = [1, 3, 5, 10, 30, 50]

# y axis values
serial = [0.051000, 0.032000, 0.037000, 0.041000, 0.033000, 0.032000]
parallel = [0.161062, 0.172192, 0.119739, 0.260115, 0.502862, 0.447328]
total = [0.212062, 0.204192, 0.156739, 0.301115, 0.535862, 0.479328]

plt.figure()
plt.title('\n'.join(wrap('Serial, Parallel and Total Runtime vs Output Size K (input size = 50, # of processes = 5, D = 3) ')))
plt.xlabel("K")
plt.ylabel("Runtime in milliseconds")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(K, serial, 'ro', label="Serial Part Runtime", alpha=0.5)
plt.plot(K, parallel, 'co', label="Parallel Part Runtime", alpha=0.5)
plt.plot(K, total, 'mo', label="Total Runtime", alpha=0.5)
plt.legend()
plt.savefig("input_size_50_numproc_5_D_3.png")
plt.show()


# x axis values
x = [2, 10, 50]

# y axis values
serial = [0.020000, 0.029000, 0.042000]
parallel = [0.046002, 0.071214, 0.128524]
total = [0.066002, 0.100214, 0.170524]

plt.figure()
plt.title('\n'.join(wrap('Serial, Parallel and Total Runtime vs # of documents (D = 3, K = 2, # of processes = 2) ')))
plt.xlabel("# of Documents")
plt.ylabel("Runtime in milliseconds")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(x, serial, 'ro', label="Serial Part Runtime", alpha=0.5)
plt.plot(x, parallel, 'co', label="Parallel Part Runtime", alpha=0.5)
plt.plot(x, total, 'mo', label="Total Runtime", alpha=0.5)
plt.legend()
plt.savefig("D_3_K_2_numproc_2.png")
plt.show()
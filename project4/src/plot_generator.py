import numpy as np
import matplotlib.pyplot as plt
from textwrap import wrap

#----------------------------------------------CAVITY

# x axis values
numthr = [256, 512, 1024]
numiter = [1, 2, 5, 10, 20]

# num_iter
parallel = [46.000000, 52.000000,  49.000000, 51.000000, 48.000000]
speedup = []
eff = []

plt.figure()
plt.title('\n'.join(wrap('# of iterations vs parallel time for 256 threads for cavity02.mtx')))
plt.xlabel("# of iterations")
plt.ylabel("Parallel runtime in milliseconds")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numiter, parallel, 'ro', label="Parallel Part Runtime", alpha=0.5)
plt.legend()
plt.savefig("cavity_iter_parallel.png")
plt.show()

# num of thr
parallel = [52.000000, 47.000000, 46.000000]
speedup = []
eff = []

plt.figure()
plt.title('\n'.join(wrap('# of threads vs parallel time for 5 iterations for cavity02.mtx')))
plt.xlabel("# of threads")
plt.ylabel("Parallel runtime in milliseconds")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numthr, parallel, 'ro', label="Parallel Part Runtime", alpha=0.5)
plt.legend()
plt.savefig("cavity_thr_parallel.png")
plt.show()

for ele in numthr:
    s = 1 / ( (1 - 7.5 / 10.5) + ( 7.5 / (10.5 * ele) ) )
    speedup.append(s)
    eff.append(s / ele)

# speedup
plt.figure()
plt.title('\n'.join(wrap('# of threads vs speedup for 5 iterations for cavity02.mtx')))
plt.xlabel("# of threads")
plt.ylabel("Speedup")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numthr, speedup, 'ro', label="Speedup", alpha=0.5)
plt.legend()
plt.savefig("cavity_thr_speedup.png")
plt.show()

#eff
plt.figure()
plt.title('\n'.join(wrap('# of threads vs efficiency for 5 iterations for cavity02.mtx')))
plt.xlabel("# of threads")
plt.ylabel("Efficiency")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numthr, eff, 'ro', label="Efficiency", alpha=0.5)
plt.legend()
plt.savefig("cavity_thr_eff.png")
plt.show()



#----------------------------------------------FIDAPM08

# x axis values
numthr = [256, 512, 1024]
numiter = [1, 2, 5, 10, 20]

# num_iter
parallel = [54.000000, 52.000000,  53.000000, 51.000000, 66.000000]
speedup = []
eff = []

plt.figure()
plt.title('\n'.join(wrap('# of iterations vs parallel time for 256 threads for fidapm08.mtx')))
plt.xlabel("# of iterations")
plt.ylabel("Parallel runtime in milliseconds")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numiter, parallel, 'ro', label="Parallel Part Runtime", alpha=0.5)
plt.legend()
plt.savefig("fidapm08_iter_parallel.png")
plt.show()

# num of thr
parallel = [53.000000, 52.000000, 53.000000]
speedup = []
eff = []

plt.figure()
plt.title('\n'.join(wrap('# of threads vs parallel time for 5 iterations for fidapm08.mtx')))
plt.xlabel("# of threads")
plt.ylabel("Parallel runtime in milliseconds")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numthr, parallel, 'ro', label="Parallel Part Runtime", alpha=0.5)
plt.legend()
plt.savefig("fidapm08_thr_parallel.png")
plt.show()

for ele in numthr:
    s = 1 / ( (1 - 7.5 / 10.5) + ( 7.5 / (10.5 * ele) ) )
    speedup.append(s)
    eff.append(s / ele)

# speedup
plt.figure()
plt.title('\n'.join(wrap('# of threads vs speedup for 5 iterations for fidapm08.mtx')))
plt.xlabel("# of threads")
plt.ylabel("Speedup")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numthr, speedup, 'ro', label="Speedup", alpha=0.5)
plt.legend()
plt.savefig("fidapm08_thr_speedup.png")
plt.show()

#eff
plt.figure()
plt.title('\n'.join(wrap('# of threads vs efficiency for 5 iterations for fidapm08.mtx')))
plt.xlabel("# of threads")
plt.ylabel("Efficiency")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numthr, eff, 'ro', label="Efficiency", alpha=0.5)
plt.legend()
plt.savefig("fidapm08_thr_eff.png")
plt.show()


#----------------------------------------------FIDAPM11

# x axis values
numthr = [256, 512, 1024]
numiter = [1, 2, 5, 10, 20]

# num_iter
parallel = [46.000000, 52.000000,  49.000000, 51.000000, 48.000000]
speedup = []
eff = []

plt.figure()
plt.title('\n'.join(wrap('# of iterations vs parallel time for 256 threads for fidapm11.mtx')))
plt.xlabel("# of iterations")
plt.ylabel("Parallel runtime in milliseconds")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numiter, parallel, 'ro', label="Parallel Part Runtime", alpha=0.5)
plt.legend()
plt.savefig("fidapm11_iter_parallel.png")
plt.show()

# num of thr
parallel = [52.000000, 47.000000, 46.000000]
speedup = []
eff = []

plt.figure()
plt.title('\n'.join(wrap('# of threads vs parallel time for 5 iterations for fidapm11.mtx')))
plt.xlabel("# of threads")
plt.ylabel("Parallel runtime in milliseconds")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numthr, parallel, 'ro', label="Parallel Part Runtime", alpha=0.5)
plt.legend()
plt.savefig("fidapm11_thr_parallel.png")
plt.show()

for ele in numthr:
    s = 1 / ( (1 - 7.5 / 10.5) + ( 7.5 / (10.5 * ele) ) )
    speedup.append(s)
    eff.append(s / ele)

# speedup
plt.figure()
plt.title('\n'.join(wrap('# of threads vs speedup for 5 iterations for fidapm11.mtx')))
plt.xlabel("# of threads")
plt.ylabel("Speedup")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numthr, speedup, 'ro', label="Speedup", alpha=0.5)
plt.legend()
plt.savefig("fidapm11_thr_speedup.png")
plt.show()

#eff
plt.figure()
plt.title('\n'.join(wrap('# of threads vs efficiency for 5 iterations for fidapm11.mtx')))
plt.xlabel("# of threads")
plt.ylabel("Efficiency")
plt.autoscale(enable=True, axis='both', tight=True)
plt.plot(numthr, eff, 'ro', label="Efficiency", alpha=0.5)
plt.legend()
plt.savefig("fidapm11_thr_eff.png")
plt.show()
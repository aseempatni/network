#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt

tm = []
td = []
dm = []
dd = []
jm = []
jd = []

with open("data","r") as f:
	for i in range(1,8):
		f.readline()
		mean = f.readline().split()
		tm.append (float(mean[0]))
		dm.append (float(mean[1]))
		jm.append (float(mean[2]))
		dev = f.readline().split()
		td.append (float(dev[0]))
		dd.append (float(dev[1]))
		jd.append (float(dev[2]))

# example data
x = np.array([16,32,64,128,256,512,1024])
y = dm

# First illustrate basic pyplot interface, using defaults where possible.
plt.figure()
plt.errorbar(x, tm, td)
plt.title("Throughput Performance")
plt.xlabel("Data Generation Rate(Kbps)")
plt.ylabel("Throughput")
plt.savefig("Throughput")
plt.show()

plt.figure()
plt.errorbar(x, dm, dd)
plt.title("Forward Delay Performance")
plt.xlabel("Data Generation Rate(Kbps)")
plt.ylabel("Forwarding Delay")
plt.savefig("ForwardDelay")
plt.show()

plt.figure()
plt.errorbar(x, jm, jd)
plt.title("Jitter Performance")
plt.xlabel("Data Generation Rate(Kbps)")
plt.ylabel("Jitter")
plt.savefig("Jitter")
plt.show()



# # Now switch to a more OO interface to exercise more features.
# fig, axs = plt.subplots(nrows=2, ncols=2, sharex=True)
# ax = axs[0,0]
# ax.errorbar(x, y, yerr=yerr, fmt='o')
# ax.set_title('Vert. symmetric')

# # With 4 subplots, reduce the number of axis ticks to avoid crowding.
# ax.locator_params(nbins=4)

# ax = axs[0,1]
# ax.errorbar(x, y, xerr=xerr, fmt='o')
# ax.set_title('Hor. symmetric')

# ax = axs[1,0]
# ax.errorbar(x, y, yerr=[yerr, 2*yerr], xerr=[xerr, 2*xerr], fmt='--o')
# ax.set_title('H, V asymmetric')

# ax = axs[1,1]
# ax.set_yscale('log')
# # Here we have to be careful to keep all y values positive:
# ylower = np.maximum(1e-2, y - yerr)
# yerr_lower = y - ylower

# ax.errorbar(x, y, yerr=[yerr_lower, 2*yerr], xerr=xerr,
#             fmt='o', ecolor='g', capthick=2)
# ax.set_title('Mixed sym., log y')

# fig.suptitle('Variable errorbars')

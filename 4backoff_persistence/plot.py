#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import sys

prefix = "backoff_"

d1 = sys.argv[1]
d2 = sys.argv[2]
nplot = len(sys.argv) - 1
pname = sys.argv[1:]

atm = []
atd = []
adm = []
add = []
ajm = []
ajd = []

for j in range(nplot):
	tm = []
	td = []
	dm = []
	dd = []
	jm = []
	jd = []
	with open(pname[j],"r") as f:
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
	atm.append(tm)
	atd.append(td)
	adm.append(dm)
	add.append(dd)
	ajm.append(jm)
	ajd.append(jd)

x = np.array([16,32,64,128,256,512,1024])
y = dm

# pname = ["p = 0.1","p = 0.5","p = 0.9","p = 1.0"]
pname = ["exponential","linear"]
# Plot throughput
plt.figure()
aplot = []
for j in range(nplot):
	aplot.append (plt.errorbar(x, atm[j], atd[j])[0])
plt.legend(aplot,pname)
plt.title("Throughput Performance")
plt.xlabel("Data Generation Rate(Kbps)")
plt.ylabel("Throughput")
plt.savefig(prefix+"Throughput")
plt.show()

# Plot forward delay
plt.figure()
aplot = []
for j in range(nplot):
	aplot.append (plt.errorbar(x, adm[j], add[j])[0])
plt.legend(aplot,pname)
plt.title("Forward Delay Performance")
plt.xlabel("Data Generation Rate(Kbps)")
plt.ylabel("Forwarding Delay")
plt.savefig(prefix+"ForwardDelay")
plt.show()

# Plot jitter
plt.figure()
aplot = []
for j in range(nplot):
	aplot.append (plt.errorbar(x, ajm[j], ajd[j])[0])
plt.legend(aplot,pname)
plt.title("Jitter Performance")
plt.xlabel("Data Generation Rate(Kbps)")
plt.ylabel("Jitter")
plt.savefig(prefix+"Jitter")
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

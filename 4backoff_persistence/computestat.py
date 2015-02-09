import numpy as np
import sys
def findstat(filename):
	with open(filename,"r") as f:
		tpa = []
		fda = []
		jia = []

		for line in f:
			tp,fd,ji = line.split()
			tpa.append(float(tp))
			fda.append(float(fd))
			jia.append(float(ji))
		tp = np.array(tpa)
		fd = np.array(fda)
		ji = np.array(jia)
		print np.mean(tp),np.mean(fd),np.mean(ji)
		print np.std(tp),np.std(fd),np.std(ji)

print sys.argv[1]
findstat(sys.argv[1])
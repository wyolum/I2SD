from pylab import *
from numpy import *

dat = []
for l in open('pw.dat').readlines():
    l = l.split()
    print l
    if len(l) == 2:
        dat.append([int(l[0]), int(l[1])])
dat = array(dat)

x = arange(len(dat))
l = 230
for i in range(len(dat) // l):
    plot(arange(l), i * 50000 + dat[i * l:(i + 1) * l])
ylim(0,1e6);

figure()
plot(dat[:,0])
show()


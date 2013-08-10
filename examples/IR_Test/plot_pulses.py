from pylab import *
from scipy import *
from numpy import *


dat = loadtxt('dat.csv', delimiter=',')
off = dat[:,0]
on = dat[:,1]
ns = dat[:,2].astype(int)
print ns[-1]

figure(1); clf()
bytes = []
msgs = {}
thresh = 1000
for i in range(1, ns[-1]):
    goff = off[where(equal(ns, i))[0]]
    gon = on[where(equal(ns, i))[0]]
    group = ravel(zip(goff, gon))
    ax = subplot(211)
    plot(arange(0, len(goff), .5), group)
    ylim(0, 2 * thresh)
    ylabel('Pulse Width uS')
    plot(arange(0, len(goff), 1), goff)
    plot([0, len(goff)], [thresh, thresh], 'r--')
    ng = len(group)
    bits = greater(goff, thresh) + 0
    subplot(212, sharex=ax)
    plot(arange(0, len(bits)), bits, 'b-o')
    ylim(-.3, 1.3)
    ylabel('Bit Values')
    yticks([0, 1])
    fill([2, 18, 18, 2, 2],
         [-.3, -.3, 1.3, 1.3, -.3], facecolor='b', alpha=.3)
    text(10, .3, 'Start Seq', rotation=60)
    fill([31, 35, 35, 31, 31],
         [-.3, -.3, 1.3, 1.3, -.3], facecolor='b', alpha=.3)
    text(32, .3, 'Stop Seq', rotation=60)
    fill([18, 31, 31, 18, 18],
         [-.3, -.3, 1.3, 1.3, -.3], facecolor='g', alpha=.3)
    text(20, -.2, 'Message', rotation=0)
    msgs[tuple(bits)] = 1
    bitstr = ''.join(map(str, bits))
    for i, bit in enumerate(bits):
        text(i + .1, 1.05, str(bit), rotation=0)
    bits = bits[18:31]
    show()
    bytes.append(bits)
    break
print len(msgs), len(bytes) / 3.
show()
vals = []
for msg in bytes:
    val = 0
    for i, bit in enumerate(msg):
        val += 1<<i * bit
    print '0x%04x' % val
    vals.append(val)
print len(vals), len(set(vals))
# subplot(211)
# ylim(0, 5000)
# subplot(212)
dat = cumsum(dat)
N = len(dat)
vals = ravel(zip(arange(N) % 2, (arange(N) + 1) % 2)) * .99

dat = ravel(zip(dat, dat))

# figure(2); clf()
# plot(dat, vals)
# show()

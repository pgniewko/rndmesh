#! /usr/bin/env python

import sys
import matplotlib.pylab as plt

def read_file(fname):
    fin = open(fname,'rU')
    x1 = []
    x2 = []
    x3 = []
    y1 = []
    y2 = []
    y3 = []

    for line in fin:
        pairs = line.split()

        n = int(pairs[1])
        nsteps = int(pairs[3])
        nanneals = int(pairs[5])
        time = float(pairs[13])

        if nsteps == 50 and nanneals == 50:
            x1.append(n)
            y1.append(time)
        elif nsteps == 100 and nanneals == 50:
            x2.append(n)
            y2.append(time)
        elif nsteps == 100 and nanneals == 100:
            x3.append(n)
            y3.append(time)

    fin.close()

    return x1,y1,x2,y2,x3,y3


x1,y1,x2,y2,x3,y3 = read_file(sys.argv[1])

fig, ax1 = plt.subplots(1, 1, sharey=True, figsize=(7,7) )

ax1.plot(x1,y1,'o-', color='blue',label=r'n_steps=%d, n_anneals=%d' %(50,50))
ax1.plot(x2,y2,'o-', color='green',label=r'n_steps=%d, n_anneals=%d' %(100,50))
ax1.plot(x3,y3,'o-', lw=2,color='orange',label=r'n_steps=%d, n_anneals=%d' %(100,100))
ax1.set_xscale("log", nonposx='clip')
ax1.set_yscale("log", nonposy='clip')

plt.xlabel('System size',fontsize=15)
plt.ylabel('time [ms]',fontsize=15)

plt.tick_params(which='both',  width=2)
plt.tick_params(which='major', length=8)
plt.xticks(fontsize=15, rotation=0)
plt.yticks(fontsize=15, rotation=0)

plt.legend(loc=0,fontsize=15)

plt.show()





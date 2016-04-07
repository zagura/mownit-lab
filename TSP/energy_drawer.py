#!/usr/bin/python3
import matplotlib.pyplot as plt
from math import sqrt

res = open('energy.csv').readlines()
x1 = []
y1 = []
for l in res:
	p1 = l.split(',')
	x1.append(p1[0])
	y1.append(sqrt(float(p1[1])))
plt.plot(x1, y1)
plt.draw()
plt.show()
#plt.savefig("solution.png")

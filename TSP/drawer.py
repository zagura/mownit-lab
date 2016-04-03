#!/usr/bin/python3
import matplotlib.pyplot as plt

res = open('last.csv').readlines()
x1 = []
x2 = []
y1 = []
y2 = []
for l in res:
	points = l.split('  ')
	p1 = points[0].split(',')
	p2 = points[1].split(',')
	x1.append(p1[0])
	x2.append(p2[0])
	y1.append(p1[1])
	y2.append(p2[1])
plt.plot(x1, y1, x2, y2)
plt.draw()
plt.show()
plt.savefig("solution.png")
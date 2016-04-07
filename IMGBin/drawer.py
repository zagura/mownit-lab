#!/usr/bin/python3
import matplotlib.pyplot as plt

res = open('final.csv').readlines()
x1 = []
y1 = []
for l in res:
	point = l.split(';')
	x1.append(point[0])
	y1.append(point[1])
plt.plot(x1, y1, 's')
plt.draw()
plt.show()
plt.savefig("solution.png")

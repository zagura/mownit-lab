#! /usr/bin/python3
import random
import sys
import math

class point(object):
	def __init__(self, a, b):
		self.x = a
		self.y = b
	def move_p(self, x_move, y_move):
		self.x += x_move
		self.y += y_move
	def print_point(self):
		print(str(self.x) + ' ' + str(self.y))

'''Normal division '''
def generate_normal(density):
	n = density*size*size // 100
	points = list()
	for x in range(size):
		for y in range(size):
			points.append(point(x, y))

	choosen = random.sample(points, n)
#	print('Size: ' + str(size))
	for p in choosen:
		p.print_point()

density = int(sys.argv[1])
#n = int(sys.argv[2])
size = 200
generate_normal(density)




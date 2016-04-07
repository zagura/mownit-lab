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
def generate_normal():
	points = list()
	for x in range(size):
		for y in range(size):
			points.append(point(x, y))

	choosen = random.sample(points, n)
#	print('Size: ' + str(size))
	for p in choosen:
		p.print_point()

def generate_4groups():
	small = size // 3
	points = list()
	for x in range(small):
		for y in range(small):
			points.append(point(x, y))

	i = 0
	choosen = random.sample(points, n)
	for p in choosen:
		x_move = i % 2 * ((size * 2) //  3)
		y_move = i // 2 * ((size * 2) // 3)
		p.move_p(x_move, y_move)
		p.print_point()
		i += 1
		i %= 4

def generate_9groups():
	small = size // 5
	points = list()
	for x in range(small):
		for y in range(small):
			points.append(point(x, y))

	choosen = random.sample(points, n)
	i = 0
	for p in choosen:
		x_move = i % 3 * ((size * 2) //  5)
		y_move = i // 3 * ((size * 2) // 5)
		p.move_p(x_move, y_move)
		p.print_point()
		i += 1
		i %= 9

a = int(sys.argv[1])
n = int(sys.argv[2])
size = 600
if a == 0:
	generate_normal()
if a == 1:
	generate_4groups()
if a == 2:
	generate_9groups()



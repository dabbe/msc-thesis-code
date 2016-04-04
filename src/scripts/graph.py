import matplotlib.pyplot as plt
import os
import sys
import numpy as np

markers = ['o', 's', '>']
labels = ['C', 'C#', 'Java']
titles = ['XML DOM Parser', 'XML SAX Parser', 'Snapshot', 'Motion Detection']
types = ['CPU usage', 'RAM usage']
yaxis = ['CPU usage in percent', 'RAM usage in kilobytes']
yaxis_subplot = ['CPU percentage', 'RAM usage (kB)']
colors = ['blue', 'green', 'red']

DIRECTORY = "data"
files = sorted(os.listdir(DIRECTORY))

def main():
	plot(0, len(files)/2 - 3)
	subplot(len(files)/2 - 3, len(files)/2)
	plot(len(files)/2, len(files) - 3)
	subplot(len(files) - 3, len(files))

def subplot(start, end):

	x_limit = -1;
	y_limit = -1;

	for i in range(start, end):
		f = files[i]
		lines = [line.rstrip('\n') for line in open(os.path.join(DIRECTORY, f))]

		length = len(lines)

		if length > x_limit:
			x_limit = length

		for line in lines:
			num = float(line)
			if num > y_limit:
				y_limit = num



	curr_max = -1

	for i in range(start, end):
		f = files[i]
		lines = [line.rstrip('\n') for line in open(os.path.join(DIRECTORY, f))]

		length = len(lines)
		x = np.arange(0.0, length, 1)
		y = lines

		plt.figure(1)

		plt.subplot(311 + (i - start))

		plt.plot(x, y, label=labels[(i - start) % 3], marker=markers[(i-start) % 3], markevery=10, color=colors[(i - start)])

		title = titles[(i / 3) % len(titles)]
		subtitle = " " + types[(2*(i))/len(files)]
		plt.title(title + subtitle)
		plt.legend(loc='best')
		plt.xlabel('Time in seconds', fontsize=14)
		plt.ylabel(yaxis_subplot[(2*i)/len(files)], fontsize=12)
		plt.grid(True)

		val = plt.ylim()[1]
		if val > curr_max:
			curr_max = val

		plt.gca().set_xlim(right=x_limit * 1.05)
		plt.gca().set_ylim(top=y_limit * 1.1)
		plt.gca().set_ylim(bottom=0)


	plt.tight_layout()
	filename = "output_imgs/motiondetection"

	if end > len(files)/2:
		filename += "ram"
	else:
		filename += "cpu"

	filename += "usage.eps"

	plt.savefig(filename, bbox_inches='tight', format='eps')
	plt.close()

def plot(start, end):

	curr_max = -1

	#for idx, f in enumerate(sorted(os.listdir(DIRECTORY))):
	for i in range(start, end):
		f = files[i]
		lines = [line.rstrip('\n') for line in open(os.path.join(DIRECTORY, f))]

		length = len(lines)

		x = np.arange(0.0, length, 1)
		y = lines

		plt.plot(x, y, marker=markers[i % 3], label=labels[i % 3])
		plt.legend(loc='best')
		plt.xlabel('Time in seconds', fontsize=18)
		plt.ylabel(yaxis[(2*i)/len(files)], fontsize=18)

		plt.grid(True)

		val = plt.ylim()[1]
		if val > curr_max:
			curr_max = val

		if i % 3 == 2:
			plt.gca().set_ylim(top=curr_max * 1.1)
			title = titles[(i / 3) % len(titles)]
			subtitle = " " + types[(2*i)/len(files)]
			plt.title(title + subtitle)
			plt.savefig((("output_imgs/" + title + subtitle).replace(" ", "") + ".eps").lower(), bbox_inches='tight', format='eps')
			plt.close()
			curr_max = -1


main()
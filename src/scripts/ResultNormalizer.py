
import os

DIR = "../result_parser/data/"

for f in os.listdir(DIR):
	infile = f
	if not (f.startswith("14") or f.startswith("26")):
		continue


	lines = [line.rstrip('\n') for line in open(os.path.join(DIR, f))]

	val = float(lines[0])

	new_list = []

	for l in lines:
		new_list.append(float(l)-val)

	with open(os.path.join(DIR, infile) + ".output", mode='wt') as myfile:
	    myfile.write('\n'.join(str(line) for line in new_list))


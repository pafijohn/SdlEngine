import sys
import struct

filename = sys.argv[1]

f = open(filename, 'r')
reformatted = open(filename + '.txt', 'w')

file_contents = f.read()

file_lines = file_contents.split('\n')

for line in file_lines:
	split_line = line.split(',')
	
	if line:
		for num in split_line:
			n = int(num)
			reformatted.write(struct.pack('@i', n))

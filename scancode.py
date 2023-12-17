#!/usr/bin/env python

ROW_COUNT = 8

def comp_code(ports):
	key = -1
	for row in range(ROW_COUNT):		
		for idx, val in enumerate(ports):
			if val > 0:
				mask = (1 << row)
				if val & mask:
					base = 8 if idx == 0 else 0
					key = mask << base
	#returns word of two key codes		
	return key

def main():
	ports = []
	for val in ['A', 'B']:
		ports.append(input("ppi port %s: " % val))

	ports = map(ports, int)

	print(comp_code(ports))

if __name__ == '__main__':
	main()
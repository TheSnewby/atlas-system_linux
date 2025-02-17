#!/usr/bin/python3
"""Utilize the /proc system to access read, write, heap info for a given PID"""
from sys import argv
from ctypes import *
import os

def get_heap_address_range(pid:int) -> tuple[int, int]:
	"""returns the heap address range of a given PID"""
	path = f"/proc/{pid}/maps"
	try:
		with open(path) as file:
			for line in file:
				if "heap" in line:
					substrings = line.split()
					heap_line_memory = substrings[0]
					break
	except PermissionError:
		print("Error: Permission denid. Please check file permissions.")
		exit(1)
	except FileNotFoundError:
		print("Error: File not found.")
		exit(1)
	except Exception as e:
		print(f"An unexpected error occurred: {e}")
		exit(1)
	mem_length : int = int(len(heap_line_memory) / 2)

	heap_range : tuple = (line[:mem_length], line [mem_length + 1:mem_length*2+1])
	print(heap_range)
	return heap_range

def find_and_replace(pid: int, heap_range: tuple[int, int], find: str, replace: str):
	"""finds and replaces a value in the memory range"""
	heap_rw_lib = CDLL("./heap_rw.so")  # comment these three lines out when uncommenting the other lines
	heap_rw_lib.heap_rw.argtypes = [c_int, c_int, c_int, c_char_p, c_char_p]
	heap_rw_lib.heap_rw(pid, heap_range[0], heap_range[1], find, replace)
	# path = f"proc/{pid}/mem"
	# try:
	# 	with open(path) as file:
	# 		print(file)

	# except PermissionError:
	# 	print("Error: Permission denid. Please check file permissions.")
	# 	heap_rw_lib = CDLL("./heap_rw.so")
	# 	heap_rw_lib.heap_rw.argtypes = [c_int, c_char_p, c_char_p, c_char_p, c_char_p]
	# 	heap_rw_lib.heap_rw(pid, heap_range[0], heap_range[1], find, replace)
	# except FileNotFoundError:
	# 	print("Error: File not found.")
	# 	exit(1)
	# except Exception as e:
	# 	print(f"An unexpected error occurred: {e}")
	# 	exit(1)

if __name__ == "__main__":
	if len(argv) != 4:
		raise TypeError("ruh roh")

	pid = int(argv[1])
	heap_range = get_heap_address_range(pid)
	find_and_replace(pid, heap_range, argv[2], argv[3])

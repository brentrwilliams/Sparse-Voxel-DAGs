import subprocess
import sys


def main():
   if len(sys.argv) == 1:
      print "ERROR: Invalid number of arguments. Expected only file name."
   fileName = sys.argv[1]
   levels = [7,8,9,10]
   for level in levels:
      subprocess.call(["./main", fileName, str(level)])

if __name__ == '__main__':
   main()
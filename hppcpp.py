# hppcpp.py
# Creates hpp and cpp files with a filled in starting comment block

import sys
import os.path

def main():
   if len(sys.argv) < 2:
      print "hppcpp.py <filename>"
      sys.exit(2)
   
   name = sys.argv[1]
   
   cppName = name + ".cpp"
   hppName = name + ".hpp"

   with open(hppName, 'w') as hppFile:
      commentBlock = ("/**\n"
                      " * " + hppName + "\n"
                      " * \n"
                      " * @author Brent Williams brent.robert.williams@gmail.com\n"
                      " */ \n")
      hppFile.write(commentBlock);
   
   
   with open(cppName, 'w') as cppFile:
      commentBlock = ("/**\n"
                      " * " + cppName + "\n"
                      " * \n"
                      " * @author Brent Williams brent.robert.williams@gmail.com\n"
                      " */ \n")
      cppFile.write(commentBlock);
   
if __name__ == "__main__":
   main()

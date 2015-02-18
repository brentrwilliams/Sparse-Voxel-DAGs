
def main():
   content = []
   with open('Leafs.out') as f:
      content = f.readlines()

   myDict = {}
   for line in content:
      line = line.strip()
      num = line
      if not num in myDict:
         myDict[num] = 1;
      else:
         myDict[num] += 1

   myList = []
   for key in myDict:
     myList.append((key,myDict[key]))

   myList.sort(key=lambda tup: tup[0])

   for (key,value) in myList:
      print(str(key) + ": " + str(value))

   print('')

   print('Expected # Leaf Updates: ' +str(512-myDict[0]))



if __name__ == '__main__':
   main()
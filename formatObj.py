import re

def main():
   content = ''
   with open('SimpleTable.obj', 'r') as content_file:
      content = content_file.read()

   print(re.sub(r'/\d*', "", content))


if __name__ == '__main__':
   main()
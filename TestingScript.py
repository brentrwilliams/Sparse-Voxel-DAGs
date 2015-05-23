import subprocess


def main():
   levels = [8,9,10]
   for level in levels:
      subprocess.call(["./main", "toyStore.obj", str(level)])

if __name__ == '__main__':
   main()
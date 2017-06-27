import sys
import os

def main():
    if len(sys.argv) != 3:
        print('usage: python.exe CombineCsv.py SrcFullPath TargetFileName')
        return
    fndst = sys.argv[2]
    print('target file: ', fndst)
    
    path = sys.argv[1]
    files = os.listdir(path)
    files.sort()
    if len(files) == 0:
        print("no file to combine")
        return

    with open(fndst, 'w') as df:
        # read title
        with open(path + '\\' + files[0], 'r') as sf:
            for line in sf:
                df.write(line)
                break
        # proc each file
        for fn in files:            
            with open(path + '\\' + fn, 'r') as sf:
                noSkip = True
                for line in sf:
                    if noSkip:
                        noSkip = False
                        continue
                    df.write(line)
    print('done')



# Start program
if __name__ == "__main__":
   main()
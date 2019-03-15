oList = []
tList = []
counter = 1
with open("outTask2", "r+") as f:
    data = f.readlines() # read the text file
    for line in data:
        if ">" in line:
            counter = 0
            #do all the printing here
            print("Input:",Input,end=' ')
            print("Order:",Order,end=' ')
            print("Insert:",Method)
            avg = (int(tList[2])-int(oList[2]))/(int(Input) + int(Input/3))
            print("Average Comparisons =",avg)
            print("Depth =",tList[1])
            print("----------")
            oList.clear()
            tList.clear()
        if counter == 3:
            Input = int(line)
        if counter == 4:
            Order = line.strip('\n')
        if counter == 5:
            Method = line.strip('\n')
        if counter == 6:
                oList += line.strip().split(" ")
        if counter == 8:
                tList += line.strip().split(" ")

        counter+=1
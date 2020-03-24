import matplotlib.pyplot as plt

with open("C:\\Users\\zhang\\Documents\\tmp.txt") as file:
    line = file.readlines()
    i = 0
    while (line[i] != "points\n"):
        l = line[i].split(' ')
        if (l[0] == 'L'):
            x1 = int(l[1])
            y1 = int(l[2])
            x2 = int(l[3])
            y2 = int(l[4])
            plt.plot([x1, x2], [y1, y2])
            plt.scatter([x1, x2], [y1, y2])
        i += 1
    i += 1
    while (i < len(line)):
        l = line[i].split(' ')
        i += 1
        x1 = float(l[0])
        y1 = float(l[1])
        plt.scatter([x1], [y1])
    plt.savefig("C:\\Users\\zhang\\Documents\\tmp.png")
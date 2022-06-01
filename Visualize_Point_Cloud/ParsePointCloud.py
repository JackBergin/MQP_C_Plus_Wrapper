# Imports the necessary plotting libraries
import matplotlib.pyplot as plt
import csv
from mpl_toolkits.mplot3d import Axes3D 
 
# Initialization of time, temperature, and humidity values
xcoord = []
ycoord = []
zcoord = []

# Converts values from the csv into lists for graphing
with open('/home/parallels/Desktop/GitHub_Work/MQP/Visualize_Point_Cloud/example.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter = ',')
    for row in plots:
        xcoord.append(row[0])
        ycoord.append(row[1])
        zcoord.append(row[2])

#Removes the first column values that are the labels
xcoord.pop(0)
ycoord.pop(0)
zcoord.pop(0)

size  = len(xcoord)

# Casts temperature to float values from string
xToFloat = []  
yToFloat = []  
zToFloat = []  

for j in range(size):
    xToFloat.append(float(xcoord[j]))
    yToFloat.append(float(ycoord[j]))
    zToFloat.append(float(zcoord[j]))

ax = plt.axes(projection='3d')
ax.scatter3D(xToFloat, yToFloat, zToFloat, c=zToFloat, cmap='viridis');
ax.set_xlabel('X', fontsize = 10)
ax.set_ylabel('Y', fontsize = 10)
ax.set_zlabel('Z', fontsize = 10)
ax.set_title('Point Cloud Visualization', fontsize = 20)
plt.show()
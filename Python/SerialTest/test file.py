
from sensorPoint import SensorPointType

pointlist = []

for i in range(0, 100):

    point = SensorPointType()

    point.heightCM = i

    pointlist.append(point)


for pointToPrint in pointlist:

    print(pointToPrint.heightCM)

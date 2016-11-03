import serial
import numpy as np
import matplotlib.pyplot as plt

win_port = 'COM3'
linux_port = '/dev/ttyACM0'

ser = serial.Serial(win_port, 115200, timeout=0.1)
# ser = serial.Serial(linux_port, 115200, timeout=0.1)

theta = np.linspace(0, 2 * np.pi, 360)
distance = np.zeros(360)
speed = 0

plt.ion()
plt.figure("Raw Data")
ax = plt.subplot(111, polar=True)
ax.grid(True)

while True:
    line = ser.readline()
    if (line[0:3] == "SPD" and len(line) == 9):
        speed = int(line[3:7])
    elif (line[0:3] == "DAT" and len(line) == 1805):
        for i in range(360):
            distance[i] = int(line[5 * i + 4: 5 * i + 9])
        ax.cla()
        ax.scatter(theta, distance)
        ax.set_rmax(2000)
    
    ax.set_title("Speed : " + str(speed) + "RPM")
    plt.pause(0.01)

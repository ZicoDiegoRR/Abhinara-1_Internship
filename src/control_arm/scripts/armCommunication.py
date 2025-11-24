import serial
import numpy as np
import rospy as ros
from control_arm.msg import pub_sub
from std_msgs.msg import String

port = "/dev/ttyACM0"
baudrate = 9600

rxBuffer = []
txBuffer = []
jointBuffer = [0]*6
jointDegree = [0]*6
# Titik referensi dari 0 sampai π
x_radian = np.array([0, np.pi/4, np.pi/2, 3*np.pi/4, np.pi])
y_values = np.array([1200, 2200, 3200, 4100, 5000])
grip_val = {1: 4300}

sensorDistance = 0
prismaticPosition = 0

try:
    ser = serial.Serial(port, baudrate, timeout=1)
except Exception as e:
    ser = serial.Serial(port[:-1]+"1", baudrate, timeout=1)

def prismaticCalculation(steps):
    return (steps/200) * 0.8

def prismaticInverse(cm):
    return (cm * 200) / 0.8

def sendSerial(stringData): 
    global ser
    # with serial.serial_for_url(port, baudrate) as ser:
    ser.write(stringData.encode())

def receiveSerial():
    # ser = serial.serial_for_url(port, baudrate, timeout=1)
    global ser
    payload = ser.readline().decode(errors="ignore").strip()
    return payload

def readWrite() :
    input_string = input("First Joint: ")
    jointBuffer[0] = (float(input_string))
    input_string = input("Second Joint: ")
    jointBuffer[1] = (float(input_string))
    input_string = input("Third Joint: ")
    jointBuffer[2] = (float(input_string))
    input_string = input("Fourth Joint: ")
    jointBuffer[3] = (float(input_string))
    input_string = input("Fifth Joint (0/1): ")
    jointBuffer[4] = (input_string)

    input_string = input("Prismatic Joint: ")
    jointBuffer[5] = (input_string)

    jointBuffer[2] = str(float(jointBuffer[2]) + np.pi/2)
    jointBuffer[3] = str(float(jointBuffer[3]) + np.pi/2)

    interpolationResult = list(
        map(
            int, 
            np.interp(np.array(jointBuffer[:4], dtype = "float64"), x_radian, y_values,)
        )
    ) + list(map(int, jointBuffer[4:]))

    for i,items in enumerate(jointBuffer):
        if i < 4:
            buff = float(np.rad2deg(float(items)))
            jointDegree[i] = buff
        else:
            jointDegree[i] = items

    msgRad = ",".join(list(map(str,jointBuffer)))
    print("Joint Value in rad : " + msgRad)

    msgDeg = ",".join(list(map(str,jointDegree)))
    print("Joint Value in deg : " + msgDeg)

    msgInterp = ",".join(list(map(str,interpolationResult)))
    print("Joint After Interp : ", msgInterp)

    sendSerial("SRCH:"+msgInterp + "\n")

def main(msg):
    print(msg.data)
    jointBuffer_temp = list(map(float, msg.data[5:].split(",")))
    timestep = int(jointBuffer_temp[-1])
    jointBuffer = jointBuffer_temp[:6]
    print(jointBuffer)

    jointBuffer[3] = str(np.pi - (float(jointBuffer[3]) + np.pi/2))
    jointBuffer[4] = str(float(jointBuffer[4]) + np.pi/2)
    jointBuffer[5] = grip_val.get(min(1, jointBuffer[5]), 1900)

    jointBuffer.append(prismaticInverse(jointBuffer[0] * 100))
    jointBuffer.pop(0)
            
    interpolationResult = list(
        map(
            int, 
            np.interp(np.array(jointBuffer[:4], dtype="float64"), x_radian, y_values,)
        )
    ) + list(map(int, jointBuffer[4:]))

    for i, items in enumerate(jointBuffer):
        if i < 4:
            buff = float(np.rad2deg(float(items)))
            jointDegree[i] = buff
        else:
            jointDegree[i] = items

    msgRad = ",".join(list(map(str,jointBuffer)))
    print("Joint Value in rad : " + msgRad)

    msgDeg = ",".join(list(map(str,jointDegree)))
    print("Joint Value in deg : " + msgDeg)

    msgInterp = ",".join(list(map(str,interpolationResult)))
    print("Joint After Interp : ", msgInterp)
            
    print("CMMD:"+msgInterp + "\n")
    sendSerial("CMMD:"+msgInterp + "\n") 
        
    if msg.data[:5] == "SEND:":
        rxBuffer = receiveSerial()
        print(rxBuffer)
        if rxBuffer.startswith("DATA:"):
            strDataBuffer = rxBuffer[5:].strip().split(",")
            dataBuffer = list(map(int, strDataBuffer))
            sensorDistance = dataBuffer[0]
            prismaticPosition = prismaticCalculation(dataBuffer[1])
            print("Raw Data = ", dataBuffer)
            print(f"Distance = {sensorDistance}")
            print(f"Prismatic Position = {prismaticPosition}")

            to_pub = pub_sub()
            to_pub.x = prismaticPosition
            to_pub.step = timestep
            if sensorDistance <= 10:
                to_pub.is_block = True
                to_pub.y = sensorDistance
            else:
                to_pub.is_block = False
                to_pub.y = 0

            pub.publish(to_pub)
            rate.sleep()

if __name__ == "__main__" :
    global pub, rate, sub

    ros.init_node("arm_com")
    pub = ros.Publisher("joint_calc", pub_sub, queue_size=10)
    sub = ros.Subscriber("joint_move", String, main)
    rate = ros.Rate(10) 

    print("Waiting for input...")
    while not ros.is_shutdown():
        ros.spin() 
        # Menjaga agar kode tetap berjalan sehingga fungsi callback dapat berjalan
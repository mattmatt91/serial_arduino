import time
import json
import serial
from pprint import pprint
import random
import numpy as np



def start_serial():
    ser  = serial.Serial("COM3", baudrate= 115200, 
           timeout=2.5, 
           parity=serial.PARITY_NONE, 
           bytesize=serial.EIGHTBITS, 
           stopbits=serial.STOPBITS_ONE
        )
    return ser


if __name__ == "__main__":
    ser = start_serial()


    values_x = []
    values_y = []
    last_x = 0



    if ser.isOpen():
        while 1:
            time.sleep(0.1)
            try:
                incoming = ser.readline().decode("utf-8")
                if len(incoming) >0:
                    data = json.loads(incoming)
                    if 'Temperature' in data.keys():
                        print(data)
                    else:
                        last_x = data['X']
                        # print(data['X'], (int(last_x)//10)*' ', end="\r")
                        offset_x = 0
                        value_x = (int(data['X'])//10)
                        print(data['X'], value_x*'#'+ (100-value_x)*' ' , end="\r")
                        values_x.append(data['X'])
                        if len(values_x)>10:
                            values_x = values_x[len(values_x)-10:]
                        # print(np.mean(values_x))
 
            except Exception as e:
                print (e)
                print('reset')
                ser.close()
                time.sleep(2)
                ser = start_serial()

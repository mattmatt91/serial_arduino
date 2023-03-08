import time
import json
import serial
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
    offset_x = 0
    offset_y = 0



    if ser.isOpen():
        while 1:
            time.sleep(0.1)
            try:
                incoming = ser.readline().decode("utf-8")
                try:
                    if len(incoming) >0:
                        data = json.loads(incoming)
                        if 'Temperature' in data.keys():
                            print(data)
                        else:
                            value_x = np.abs(int(data['X']))-offset_x
                            values_x.append(data['X'])
                            
                            value_y = np.abs(int(data['Y']))-offset_y
                            values_y.append(value_y)
                        
                            if len(values_x)>10:
                                values_x = values_x[len(values_x)-10:]
                            if len(values_y)>10:
                                values_y = values_y[len(values_y)-10:]
                            print(np.mean(values_x), np.mean(values_y), value_x, value_y)
                except:
                    pass
                    print(incoming)
                    
            except Exception as e:
                print (e)
                ser.close()
                exit()
              
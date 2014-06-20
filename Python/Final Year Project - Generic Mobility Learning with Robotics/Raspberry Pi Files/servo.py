import RPi.GPIO as GPIO
import smbus
import math
import time
import numpy as np


leftang = 0.0
rightang = 0.0
print 'test'
#SETUP GPIO I/O PINS
GPIO.setmode(GPIO.BCM)
GPIO.setup(18,GPIO.OUT)
GPIO.setup(17,GPIO.OUT)
GPIO.setup(22,GPIO.IN)
GPIO.setup(23,GPIO.IN)

bus = smbus.SMBus(1)
address = 0x68
power_mgmt_1=0x6b
power_mgmt_2=0x6c
INFRA = GPIO.input(23)
switch = GPIO.input(22)

#WAKE SENSORS
bus.write_byte_data(address,power_mgmt_1,0)
time.sleep(1)
bus.write_byte_data(address,power_mgmt_2,0)
time.sleep(1)
p2 = GPIO.PWM(17,200)
p = GPIO.PWM(18,200)
p.start(0)
p2.start(0)
p2.ChangeDutyCycle(20)
p.ChangeDutyCycle(20)

#SENSOR FUNCTIONS
def read_byte(adr):
        return bus.read_byte_data(address,adr)

def read_word(adr):
        high = bus.read_byte_data(address,adr)
        low = bus.read_byte_data(address,adr+1)
        val = (high << 8) + low
        return val

def read_word_2c(adr):
        val = read_word(adr)
        if (val >= 0x8000):
                return -((65535 - val) + 1)

def dist(a,b):
        return math.sqrt((a*a)+(b*b))

def get_y_rotation(x,y,z):
        radians = math.atan2(x,dist(y,z))
        return -math.degrees(radians)

def get_x_rotation(x,y,z):
        radians = math.atan2(y,dist(x,z))
        return math.degrees(radians)

def getSensors(): #Following order, accel xyz,gyro xyz, angle left right
        gyro_xout = int(read_word_2c(0x43) or 0)
        gyro_yout = int(read_word_2c(0x45) or 0)
        gyro_zout = int(read_word_2c(0x47) or 0)
        gx = gyro_xout/131.0
        gy = gyro_yout/131.0
        gz = gyro_zout/131.0
        accel_xout = int(read_word_2c(0x3b) or 0)
        accel_yout = int(read_word_2c(0x3d) or 0)
        accel_zout = int(read_word_2c(0x3f) or 0)
        ax = -accel_xout/16384.0
        ay = -accel_yout/16384.0
        az = -accel_zout/16384.0
#        print ay
#        print ax
#        print az
        return np.array([ax,ay,az,gx,gy,gz,leftang,rightang])

def setAngles(angles):
	print angles
        p.ChangeFrequency(180)
        p.ChangeDutyCycle(angles[0])
        leftang = angles[0]
        p2.ChangeFrequency(180)
        p2.ChangeDutyCycle(angles[1])
        rightang = angles[1]

def stop():
        p.stop()
        p2.stop()
        GPIO.cleanup()
        
##print "Starting loop"
##while 1:
##      if GPIO.input(23)==0:
##              break
##
##      #Retrieve sensor input
##      gyro_xout = int(read_word_2c(0x43) or 0)
##      gyro_yout = int(read_word_2c(0x45) or 0)
##      gyro_zout = int(read_word_2c(0x47) or 0)
##      gyro_xout_scaled = gyro_xout/131.0
##      gyro_yout_scaled = gyro_yout/131.0
##      gyro_zout_scaled = gyro_zout/131.0
##      
##      accel_xout = int(read_word_2c(0x3b) or 0)
##      accel_yout = int(read_word_2c(0x3d) or 0)
##      accel_zout = int(read_word_2c(0x3f) or 0)
##      accel_xout_scaled = accel_xout/16384.0
##      accel_yout_scaled = accel_yout/16384.0
##      accel_zout_scaled = accel_zout/16384.0
##
##      #Manage sensors and manipulate servos
##      curr_pos = ((get_y_rotation(accel_xout_scaled,accel_yout_scaled,accel_zout_scaled)*40)/360.0) + 0.0000000000000001
##      print curr_pos
##      p.ChangeFrequency(50)
##      p2.ChangeFrequency(50)
##      p.ChangeDutyCycle(curr_pos)
##      p2.ChangeDutyCycle(curr_pos)
##      time.sleep(0.1)
##      #for dc in range(0,60,5):
##      #       p2.ChangeFrequency(180)
##      #       p2.ChangeDutyCycle(dc)
##         #       p.ChangeFrequency(180)
##          #      p.ChangeDutyCycle(dc)
##           #     time.sleep(0.05)             
##      #for dc in range(40,-1,-5):
##         #       p.ChangeFrequency(180)
##      #       p.ChangeDutyCycle(dc)
##      #       p2.ChangeFrequency(180)
##      #       p2.ChangeDutyCycle(dc)
##         #       time.sleep(0.05)
##         
##print "Exiting loop"
##p.stop()
##GPIO.cleanup()
##
##






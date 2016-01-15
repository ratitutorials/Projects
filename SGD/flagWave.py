from hashlib import md5
import sys, serial

inputHash = md5()
inputString = sys.argv[1]
inputEncoded = inputString.encode('utf-8')
inputHash.update(inputEncoded)

outputHash = inputHash.hexdigest()

ser = serial.Serial("/dev/ttyAMA0")
ser.baudrate = 9600

ser.write(outputHash)
import RPi.GPIO as gpio
import time
import Adafruit_CharLCD as LCD
import math
import requests
import json
vmax = 0
frequencia = 0
frequencia_pwm = 100
gpio.setwarnings(False)
gpio.setmode(gpio.BCM)
gpio.setup(13,gpio.OUT)
gpio.setup(26,gpio.IN)
gpio.setup(3,gpio.OUT)
gpio.setup(2,gpio.OUT)
gpio.output(3, gpio.LOW)
gpio.output(2, gpio.LOW)

gpio.add_event_detect(26, gpio.RISING)
lcd = LCD.Adafruit_CharLCD(18,25,8,7,12,21,20,4,20)

count = 0

def atualiza():
    global frequencia
    global vmax     
    lcd.home()
    lcd.message("Frequencia obtida :")
    lcd.set_cursor(0,1)
   
    lcd.message(str(frequencia))
    print(frequencia)
    lcd.message(" Hz")
    time.sleep(1.5)
    lcd.clear()
    frequencia = float(frequencia)
    
    lcd.home()
    lcd.message("Velocidade Maxima :")
    lcd.set_cursor(0,1)
    lcd.message(str(vmax))
    lcd.message(" KM/H")
    lcd.set_cursor(0,2)
    lcd.message("Velocidade Atual :")
    lcd.set_cursor(15,3)
    lcd.message(" KM/H")
    vmax = float(vmax)
    
        
        
def speed(tempo):
    global frequencia
    global vmax
    global vAtual
    global vatual
        
    distancia = 0.0002328
    velocidade = 0.0
    vatual = 0.0
    
    velocidade = ((2 * math.pi * distancia)/tempo)/8
    
    vatual = velocidade     
    
    if(float(vatual) > float(vmax)):
        gpio.output(3, gpio.HIGH)
        gpio.output(2, gpio.HIGH)
    elif(float(vatual) < float(vmax)):
        gpio.output(3, gpio.LOW)
        gpio.output(2, gpio.LOW)    
        
    int(float(vatual)) # converte float para int
    vatual = str(vatual) # converte int para string
    lcd.set_cursor(0,3)
    lcd.message(vatual)
    time.sleep(0.1)
    vAtual = float(vatual)
    url = "http://0.0.0.0:8080/setVAtual"
    data = {'vAtual':vatual}
    headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}
    r = requests.post(url, data=json.dumps(data), headers=headers)
    
    print(r)
    return vAtual
        
fAtual = 1

pwm=gpio.PWM(13, fAtual)
pwm.start(50)

def loop():
    global pwm
    global frequencia
    global frequencia_pwm
    global vmax
    count = 0
    periodo = 0
            
    while True:
        #r = requests.get("http://0.0.0.0:8080/data").json()
        #vmax = float(r['v'])
        #frequencia_pwm = float(r['f'])
        
    #pwm.ChangeFrequency(frequencia_pwm)

            NUM_CYCLES = 10
            start = time.time()
            for impulse_count in range(NUM_CYCLES):
                gpio.wait_for_edge(26, gpio.FALLING)
            periodo = time.time() - start      #seconds to run for loop
            frequencia = NUM_CYCLES / periodo   #in Hz
            print(frequencia)
            print(periodo)
            tempo = periodo/36000
 
            atualiza()
            speed(tempo)
                
            frequencia = float(frequencia)
            count = 0
 
            r = requests.get("http://0.0.0.0:8080/data").json()
            vmax = float(r['v'])
            frequencia_pwm = float(r['f'])

            pwm.ChangeFrequency(frequencia_pwm)

loop()
gpio.cleanup()

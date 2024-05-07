
import serial 
from time import sleep
from openai import OpenAI
 # should use env variable OPENAI_API_KEY
client = OpenAI(api_key="sk-proj-SElpyijA7hjAWrso1wD1T3BlbkFJYvfxb3B3SdqZbyqaqnIN") 
ser = serial.Serial("COM3", 9600 )
sleep(2)


while True:
  line = ser.readline().decode('utf-8').strip()
  print("Arduino says:", line)
  
  completion = client.chat.completions.create(
    model="gpt-3.5-turbo",
    messages=[
      {"role": "user", "content": f"This is today's temperature {line}, what should I wear"},
      
    ]
  )
  print(completion)

        
#sk-proj-SElpyijA7hjAWrso1wD1T3BlbkFJYvfxb3B3SdqZbyqaqnIN
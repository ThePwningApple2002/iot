import serial
from time import sleep
from openai import OpenAI

# Initialize the OpenAI client with an API key
client = OpenAI(api_key="sk-proj-SElpyijA7hjAWrso1wD1T3BlbkFJYvfxb3B3SdqZbyqaqnIN")  # It's safer to use environment variables for API keys

# Set up serial connection to the Arduino
ser = serial.Serial("COM3", 9600)
sleep(2)  # Wait for the connection to establish

while True:
    # Read line from Arduino
    line = ser.readline().decode('utf-8').strip()
    print("Arduino says:", line)
    
    # Query the OpenAI API
    response = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=[
            {"role": "user", "content": f"This is today's temperature {line}, what should I wear?"},
        ]
    )
    
    # Extract text from response (customize as needed)
    advice = response.choices[0].message.content
    print("Advice from AI:", advice)
    
    # Send the response back to Arduino
    ser.write(advice.encode('utf-8'))

    # Optional: sleep to prevent rapid fire commands, adjust as needed
    sleep(1)

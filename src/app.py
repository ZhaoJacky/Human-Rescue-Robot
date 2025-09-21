from flask import Flask, request, render_template, redirect, url_for
import socket, time, os  #module for network communication

PI_IP = "10.5.14.14"  # Replace with your Raspberry Pi's IP
PI_PORT = 5005          # Port that Pi's listener will use
PHOTO_FOLDER = "static"
PHOTO_FILENAME = "test.jpg"

app = Flask(__name__) #creates instance of flask class


#python decorator
@app.route("/", methods=["GET"]) #GET is standard browser page load
def home(): #run this function version when someone visits url "/" via get method
    return render_template("index.html", time = time.time())

#python decorator
@app.route("/control", methods=["POST"])
def control():
    command = request.form["command"]
    print(f"Command received: {command}")
    
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((PI_IP, PI_PORT))
            s.sendall(command.encode())
            print("✅ Command sent to Raspberry Pi")
    except Exception as e:
        #exception is saved inside of e then prints it
        print(f"⚠️ Could not send command to Raspberry Pi: {e}")
    
    return redirect(url_for("home"))

@app.route("/capture", methods=["POST"])

def capture():
    #send command to pi so it runs camera.py
    command = "photo"
    try:
        with socket.socket(socket.AP_INET, socket.SOCK_STREAM) as s:
            s.connect((PI_IP, PI_PORT))
            s.sendall(command.encode())
            print("Capture command sent to Rasperry Pi")
    except Exception as e:
        print(f"Could not request photo from Raspberry Pi: {e}")
        
    os.system(f"scp jacky@{PI_IP}:/home/jacky/Car/latest.jpg {PHOTO_FOLDER}/{PHOTO_FILENAME}")



    return redirect(url_for("home"))

if __name__ == "__main__":
    app.run(debug=True, use_reloader=False)

    #git add .
    #git commit -m "app"
    #git push origin main

#to run the website  
#make sure in src folder then in terminal:
# python app.py
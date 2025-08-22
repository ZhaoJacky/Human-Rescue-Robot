from flask import Flask, request, render_template, redirect, url_for
import socket, time  #module for network communication

PI_IP = "10.5.14.14"  # Replace with your Raspberry Pi's IP
PI_PORT = 5005          # Port that Pi's listener will use

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

if __name__ == "__main__":
    app.run(debug=True, use_reloader=False)

    #git add .
    #git commit -m "app"
    #git push origin main

#to run the website
#make sure in src folder then in terminal:
# python app.py
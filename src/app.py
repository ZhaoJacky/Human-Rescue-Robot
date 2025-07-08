from flask import Flask, request, render_template, redirect, url_for
import serial

# Adjust the COM port and baud rate as needed
ser = serial.Serial('COM5', 9600, timeout=1)

app = Flask(__name__)

@app.route("/", methods=["GET"])
def home():
    return render_template("index.html")

@app.route("/control", methods=["POST"])
def control():
    command = request.form["command"]
    print(f"Command received: {command}")
    ser.write((command + "\n").encode())  # Send to STM32
    return redirect(url_for("home"))

if __name__ == "__main__":
    app.run(debug=True)

    #git add .
    #git commit -m "app"
    #git push origin main
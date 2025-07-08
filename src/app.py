from flask import Flask, request, render_template, redirect, url_for
import serial

app = Flask(__name__)

@app.route("/", methods=["GET"])
def home():
    return render_template("index.html")

@app.route("/control", methods=["POST"])
def control():
    command = request.form["command"]
    print(f"Command received: {command}")
    try:
        with serial.Serial('COM5', 9600, timeout=1) as ser:
            ser.write((command + "\n").encode())
            print("✅ Command sent to COM5")
    except Exception as e:
        print(f"⚠️ Could not send command to COM5: {e}")
    
    return redirect(url_for("home"))

if __name__ == "__main__":
    app.run(debug=True, use_reloader=False)  # Disable auto-reloader to avoid serial port conflict

    #git add .
    #git commit -m "app"
    #git push origin main
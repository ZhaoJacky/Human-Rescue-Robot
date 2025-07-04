from flask import Flask, request, render_template_string
#imports: Flask to create the app, request accesses incoming form data 
# (e.g. button presses), render_template string allows html strings in the 
# template (not separate files)

app = Flask(__name__)

# HTML template with buttons
HTML = """
<!DOCTYPE html>
<html>
<head><title>RC Car Controller</title></head>
<body>
  <h1>Control Your RC Car</h1>
  <form method="post" action="/control">
    <button name="command" value="forward">Forward</button>
    <button name="command" value="left">Left</button>
    <button name="command" value="right">Right</button>
    <button name="command" value="backward">Backward</button>
    <button name="command" value="stop">Stop</button>
  </form>
</body>
</html>
"""

# Main page
@app.route("/", methods=["GET"])
def home():
    return render_template_string(HTML)

# Handle button clicks
@app.route("/control", methods=["POST"])
def control():
    command = request.form["command"]
    print(f"Command received: {command}")  # Replace with code to control motors
    return f"<p>Command: {command}</p><p><a href='/'>Back to Control</a></p>"

if __name__ == "__main__":
    app.run(debug=True)


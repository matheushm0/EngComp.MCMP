from flask import Flask, render_template, request, redirect
import json

app = Flask(__name__)
vMax = 0

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/vmax')
def vmax():
    return jsonify(vmax)

if __name__ == '__main__':
    app.run(debug=True, port=8080, host='10.110.2.121')
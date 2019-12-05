from flask import Flask, render_template, request,jsonify, redirect
import json

app = Flask(__name__)
vmax = 0
frequencia = 0
vAtual = 0
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/setVmax', methods=['POST'])
def setVmax():
    global frequencia 
    global vmax	    
    frequencia = request.json['f']
    vmax = request.json['v']
    print(vmax,frequencia)    
    return "OK"

@app.route('/setVAtual', methods=['POST'])
def setVAtual():
    global vAtual
    vAtual = request.json['vAtual']
    return "OK"

@app.route('/getVAtual', methods=['GET'])
def getVAtual():
    global vAtual
    return jsonify(v=vAtual)


@app.route('/data',methods=['GET'])
def data():
    global vmax
    global frequencia
    print(vmax,frequencia)
    return jsonify(v=vmax,f=frequencia)


    
if __name__ == '__main__':
    app.run(debug=True, port=8080, host='10.110.2.121')

"""
Flask Documentation:     https://flask.palletsprojects.com/
Jinja2 Documentation:    https://jinja.palletsprojects.com/
Werkzeug Documentation:  https://werkzeug.palletsprojects.com/
This file creates your application.
"""

# from crypt import methods
import site 

from app import app, Config,  mongo, Mqtt
from flask import escape, render_template, request, jsonify, send_file, redirect, make_response, send_from_directory 
from json import dumps, loads 
from werkzeug.utils import secure_filename
from datetime import datetime,timedelta, timezone
from os import getcwd
from os.path import join, exists
from time import time, ctime
from math import floor
 



#####################################
#   Routing for your application    #
#####################################

@app.route('/api/WeatherStation/update/<start>/<end>', methods=['GET']) 
def update(start,end):   
    start = int(start)
    end = int(end)
    '''RETURNS ALL THE DATA FROM THE DATABASE THAT EXIST IN BETWEEN THE START AND END TIMESTAMPS'''
    print(f"Start Date: {start}")
    print(f"End Date: {end}")
    print(type(start))
    print(type(end))
    if request.method == "GET":
        '''Add your code here to complete this route'''
        try:
            range = mongo.insertData(start,end)
            data = list(range)
            if data:
                return jsonify({"status":"complete","data": data})
        except Exception as e:
            print(f"update error: f{str(e)}") 
    # FILE DATA NOT EXIST
    return jsonify({"status":"failed","data":0})

@app.route('/api/WeatherStation/get/<start>/<end>', methods=['GET']) 
def get_all(start,end):   
    start = int(start)
    end = int(end)
    '''RETURNS ALL THE DATA FROM THE DATABASE THAT EXIST IN BETWEEN THE START AND END TIMESTAMPS'''
    print(f"Start Date: {start}")
    print(f"End Date: {end}")
    print(type(start))
    print(type(end))
    if request.method == "GET":
        '''Add your code here to complete this route'''
        try:
            range = mongo.getAllInRange(start,end)
            data = list(range)
            if data:
                return jsonify({"status":"found","data": data})
        except Exception as e:
            print(f"get_all error: f{str(e)}") 
    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":[]})
   
@app.route('/api/mmar/dhttemperature/<start>/<end>', methods=['GET']) 
def get_temperature_DHT(start,end):
    start = int(start)
    end = int(end)   
    '''RETURNS MIN, MAX, AVG AND RANGE FOR DHT TEMPERATURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
    if request.method == "GET": 
        '''Add your code here to complete this route'''
        try:
            tempdht = mongo.temperatureDHT(start,end)
            data = list(tempdht)
            if data:
                return jsonify({"status":"found","data": data})
        except Exception as e:
            print(f"get_temperature_DHT error: f{str(e)}")
    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":0})

@app.route('/api/mmar/dhthumidity/<start>/<end>', methods=['GET']) 
def get_humidity_DHT(start,end):
    start = int(start)
    end = int(end)   
    '''RETURNS MIN, MAX, AVG AND RANGE FOR DHT HUMIDITY. THAT FALLS WITHIN THE START AND END DATE RANGE'''
    if request.method == "GET": 
        '''Add your code here to complete this route'''
        try:
            humiddht = mongo.humidityDHT(start,end)
            data = list(humiddht)
            if data:
                return jsonify({"status":"found","data": data})
        except Exception as e:
            print(f"get_humidity_DHT error: f{str(e)}") 
    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":0})

@app.route('/api/mmar/dhtheatindex/<start>/<end>', methods=['GET']) 
def get_heatindex_DHT(start,end):
    start = int(start)
    end = int(end)   
    '''RETURNS MIN, MAX, AVG AND RANGE FOR DHT HEAT INDEX. THAT FALLS WITHIN THE START AND END DATE RANGE'''
    if request.method == "GET": 
        '''Add your code here to complete this route'''
        try:
            hidht = mongo.heatindexDHT(start,end)
            data = list(hidht)
            if data:
                return jsonify({"status":"found","data": data})
        except Exception as e:
            print(f"get_heatindex_DHT error: f{str(e)}") 
    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":0})

@app.route('/api/mmar/bmptemperature/<start>/<end>', methods=['GET']) 
def get_temperature_BMP(start,end):
    start = int(start)
    end = int(end)   
    '''RETURNS MIN, MAX, AVG AND RANGE FOR BMP TEMPERATURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
    if request.method == "GET": 
        '''Add your code here to complete this route'''
        try:
            tempbmp = mongo.temperatureBMP(start,end)
            data = list(tempbmp)
            if data:
                return jsonify({"status":"found","data": data})
        except Exception as e:
            print(f"get_temperature_BMP error: f{str(e)}")
    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":0})

@app.route('/api/mmar/bmppressure/<start>/<end>', methods=['GET']) 
def get_pressure_BMP(start,end):
    start = int(start)
    end = int(end)   
    '''RETURNS MIN, MAX, AVG AND RANGE FOR BMP PRESSURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
    if request.method == "GET": 
        '''Add your code here to complete this route'''
        try:
            presbmp = mongo.pressureBMP(start,end)
            data = list(presbmp)
            if data:
                return jsonify({"status":"found","data": data})
        except Exception as e:
            print(f"get_pressure_BMP error: f{str(e)}")
    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":0})

@app.route('/api/mmar/bmpaltitude/<start>/<end>', methods=['GET']) 
def get_altitude_BMP(start,end):
    start = int(start)
    end = int(end)   
    '''RETURNS MIN, MAX, AVG AND RANGE FOR BMP ALTITUDE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
    if request.method == "GET": 
        '''Add your code here to complete this route'''
        try:
            presbmp = mongo.altitudeBMP(start,end)
            data = list(presbmp)
            if data:
                return jsonify({"status":"found","data": data})
        except Exception as e:
            print(f"get_altitude_BMP error: f{str(e)}")
    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":0})

@app.route('/api/mmar/soil/<start>/<end>', methods=['GET']) 
def get_soil_moisture(start,end):
    start = int(start)
    end = int(end)   
    '''RETURNS MIN, MAX, AVG AND RANGE FOR SOIL MOISTURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
    if request.method == "GET": 
        '''Add your code here to complete this route'''
        try:
            sm = mongo.soilmoisture(start,end)
            data = list(sm)
            if data:
                return jsonify({"status":"found","data": data})
        except Exception as e:
            print(f"get_soil_moisture error: f{str(e)}")
    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":0})

@app.route('/api/frequency/<variable>/<start>/<end>', methods=['GET']) 
def get_freq_distro(variable,start,end):
    start = int(start)
    end = int(end)   
    '''RETURNS FREQUENCY DISTRIBUTION FOR SPECIFIED VARIABLE'''
    if request.method == "GET": 
        '''Add your code here to complete this route'''    
        try:
            freq = mongo.frequencyDistro(variable,start,end)
            data = list(freq)
            if data:
                return jsonify({"status":"found","data": data})
        except Exception as e:
            print(f"get_freq_distro error: f{str(e)}")     
    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":[]})

@app.route('/api/file/get/<filename>', methods=['GET']) 
def get_images(filename):   
    '''RETURNS REQUESTED FILE FROM UPLOADS FOLDER'''
    if request.method == "GET":
        '''Add your code here to complete this route'''
        # FILE DOES NOT EXIST
        return jsonify({"status":"file not found"}), 404



@app.route('/api/file/upload',methods=["POST"])  
def upload():
    '''SAVES A FILE TO THE UPLOADS FOLDER'''
    
    if request.method == "POST": 
        file     = request.files['file']
        filename = secure_filename(file.filename)
        file.save(join(getcwd(),Config.UPLOADS_FOLDER , filename))
        return jsonify({"status":"File upload successful", "filename":f"{filename}" })

 


###############################################################
# The functions below should be applicable to all Flask apps. #
###############################################################


@app.route('/<file_name>.txt')
def send_text_file(file_name):
    """Send your static text file."""
    file_dot_text = file_name + '.txt'
    return app.send_static_file(file_dot_text)

@app.after_request
def add_header(response):
    """
    Add headers to both force latest IE rendering engine or Chrome Frame,
    and also tell the browser not to cache the rendered page. If we wanted
    to we could change max-age to 600 seconds which would be 10 minutes.
    """
    response.headers['X-UA-Compatible'] = 'IE=Edge,chrome=1'
    response.headers['Cache-Control'] = 'public, max-age=0'
    return response

@app.errorhandler(405)
def page_not_found(error):
    """Custom 404 page."""    
    return jsonify({"status": 404}), 404

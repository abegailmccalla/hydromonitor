 #!/usr/bin/python3


#################################################################################################################################################
#                                                    CLASSES CONTAINING ALL THE APP FUNCTIONS                                                                                                    #
#################################################################################################################################################


class DB:

    def __init__(self,Config):

        from math import floor
        from os import getcwd
        from os.path import join
        from json import loads, dumps, dump
        from datetime import timedelta, datetime, timezone 
        from pymongo import MongoClient , errors, ReturnDocument
        from urllib import parse
        from urllib.request import  urlopen 
        from bson.objectid import ObjectId  
       
      
        self.Config                         = Config
        self.getcwd                         = getcwd
        self.join                           = join 
        self.floor                      	= floor 
        self.loads                      	= loads
        self.dumps                      	= dumps
        self.dump                       	= dump  
        self.datetime                       = datetime
        self.ObjectId                       = ObjectId 
        self.server			                = Config.DB_SERVER
        self.port			                = Config.DB_PORT
        self.username                   	= parse.quote_plus(Config.DB_USERNAME)
        self.password                   	= parse.quote_plus(Config.DB_PASSWORD)
        self.remoteMongo                	= MongoClient
        self.ReturnDocument                 = ReturnDocument
        self.PyMongoError               	= errors.PyMongoError
        self.BulkWriteError             	= errors.BulkWriteError  
        self.tls                            = False # MUST SET TO TRUE IN PRODUCTION


    def __del__(self):
            # Delete class instance to free resources
            pass
 


    ####################
    # LAB 2 DATABASE UTIL FUNCTIONS  #
    ####################
    
    def insertData(self,data):
        '''ADD A NEW STORAGE LOCATION TO COLLECTION'''
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            remotedb.ELET2415.WeatherStation.insert_one(data)
        except Exception as e:
            msg = str(e)
            if "duplicate" not in msg:
                print("insertData error ",msg)
            return False
        else:                  
            return True
          
    def getAllInRange(self,start, end):
        '''RETURNS A LIST OF OBJECTS. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.WeatherStation.find({'$and':[{'Timestamp':{'$gte':start}}, {'Timestamp':{'$lte':end}}]}, {'_id': 0}).sort('Timestamp', 1)) #Add your query here        
            #result      = list(remotedb.ELET2415.climo.find({'timestamp': {'$gte': start, '$lte': end}},{'_id': 0}).sort('timestamp', 1))
        except Exception as e:
            msg = str(e)
            print("getAllInRange error ",msg)            
        else:                  
            return result
        
    def temperatureDHT(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR DHT TEMPERATURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.WeatherStation.aggregate([{'$match':{'$and':[{'Timestamp':{'$gte':start}}, {'Timestamp':{'$lte':end}}]}}, {'$group':{'_id': '$DHT_Temperature','DHT_Temperature': {'$push': '$$ROOT.DHT_Temperature'}}}, {'$project': {'_id': 0, 'max': {'$max': '$DHT_Temperature'}, 'min': {'$min': '$DHT_Temperature'}, 'avg': {'$avg': '$DHT_Temperature'}, 'range': {'$subtract': [{'$max': '$DHT_Temperature'}, {'$min': '$DHT_Temperature'}]}}}])) #Add your Aggregation pipeline here in this function                                                       #$cond:{ if:{$and:[{timestamp:{$gte:start}}, {timestamp:{$lte:end}}]}, then:{$max:temperature, $min:temperature, $avg:temperature , $range:[$max:temperature, $min:temperature]}})     
            #result = list(remotedb.ELET2415.climo.aggregate([{ '$match': { 'timestamp': { '$gte': start, '$lte': end } } }, { '$group': { '_id': 0, 'temperature': { '$push': '$$ROOT.temperature' } } }, { '$project': { 'max': { '$max': '$temperature' }, 'min': { '$min': '$temperature' }, 'avg': { '$avg': '$temperature' }, 'range': { '$subtract': [ { '$max': '$temperature' }, { '$min': '$temperature' } ] } } } ]))
        except Exception as e:
            msg = str(e)
            print("temperatureDHT error ",msg)            
        else:                  
            return result

    def humidityDHT(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR DHT HUMIDITY. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.WeatherStation.aggregate([{'$match':{'$and':[{'Timestamp':{'$gte':start}}, {'Timestamp':{'$lte':end}}]}}, {'$group':{'_id': '$DHT_Humidity','DHT_Humidity': {'$push': '$$ROOT.DHT_Humidity'}}}, {'$project': {'_id': 0, 'max': {'$max': '$DHT_Humidity'}, 'min': {'$min': '$DHT_Humidity'}, 'avg': {'$avg': '$DHT_Humidity'}, 'range': {'$subtract': [{'$max': '$DHT_Humidity'}, {'$min': '$DHT_Humidity'}]}}}])) #Add your Aggregation pipeline here in this function
        except Exception as e:
            msg = str(e)
            print("humidityDHT error ",msg)            
        else:                  
            return result  
    
    def heatindexDHT(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR DHT HEAT INDEX. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.WeatherStation.aggregate([{'$match':{'$and':[{'Timestamp':{'$gte':start}}, {'Timestamp':{'$lte':end}}]}}, {'$group':{'_id': '$DHT_HeatIndex','DHT_HeatIndex': {'$push': '$$ROOT.DHT_HeatIndex'}}}, {'$project': {'_id': 0, 'max': {'$max': '$DHT_HeatIndex'}, 'min': {'$min': '$DHT_HeatIndex'}, 'avg': {'$avg': '$DHT_HeatIndex'}, 'range': {'$subtract': [{'$max': '$DHT_HeatIndex'}, {'$min': '$DHT_HeatIndex'}]}}}])) #Add your Aggregation pipeline here in this function
        except Exception as e:
            msg = str(e)
            print("heatindexDHT error ",msg)            
        else:                  
            return result 
           
    def temperatureBMP(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR BMP TEMPERATURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.WeatherStation.aggregate([{'$match':{'$and':[{'Timestamp':{'$gte':start}}, {'Timestamp':{'$lte':end}}]}}, {'$group':{'_id': '$BMP_Temperature','BMP_Temperature': {'$push': '$$ROOT.BMP_Temperature'}}}, {'$project': {'_id': 0, 'max': {'$max': '$BMP_Temperature'}, 'min': {'$min': '$BMP_Temperature'}, 'avg': {'$avg': '$BMP_Temperature'}, 'range': {'$subtract': [{'$max': '$BMP_Temperature'}, {'$min': '$BMP_Temperature'}]}}}])) #Add your Aggregation pipeline here in this function                                                       #$cond:{ if:{$and:[{timestamp:{$gte:start}}, {timestamp:{$lte:end}}]}, then:{$max:temperature, $min:temperature, $avg:temperature , $range:[$max:temperature, $min:temperature]}})     
            #result = list(remotedb.ELET2415.climo.aggregate([{ '$match': { 'timestamp': { '$gte': start, '$lte': end } } }, { '$group': { '_id': 0, 'temperature': { '$push': '$$ROOT.temperature' } } }, { '$project': { 'max': { '$max': '$temperature' }, 'min': { '$min': '$temperature' }, 'avg': { '$avg': '$temperature' }, 'range': { '$subtract': [ { '$max': '$temperature' }, { '$min': '$temperature' } ] } } } ]))
        except Exception as e:
            msg = str(e)
            print("temperatureBMP error ",msg)            
        else:                  
            return result
    
    def pressureBMP(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR BMP PRESSURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.WeatherStation.aggregate([{'$match':{'$and':[{'Timestamp':{'$gte':start}}, {'Timestamp':{'$lte':end}}]}}, {'$group':{'_id': '$BMP_Pressure','BMP_Pressure': {'$push': '$$ROOT.BMP_Pressure'}}}, {'$project': {'_id': 0, 'max': {'$max': '$BMP_Pressure'}, 'min': {'$min': '$BMP_Pressure'}, 'avg': {'$avg': '$BMP_Pressure'}, 'range': {'$subtract': [{'$max': '$BMP_Pressure'}, {'$min': '$BMP_Pressure'}]}}}])) #Add your Aggregation pipeline here in this function                                                       #$cond:{ if:{$and:[{timestamp:{$gte:start}}, {timestamp:{$lte:end}}]}, then:{$max:temperature, $min:temperature, $avg:temperature , $range:[$max:temperature, $min:temperature]}})     
            #result = list(remotedb.ELET2415.climo.aggregate([{ '$match': { 'timestamp': { '$gte': start, '$lte': end } } }, { '$group': { '_id': 0, 'temperature': { '$push': '$$ROOT.temperature' } } }, { '$project': { 'max': { '$max': '$temperature' }, 'min': { '$min': '$temperature' }, 'avg': { '$avg': '$temperature' }, 'range': { '$subtract': [ { '$max': '$temperature' }, { '$min': '$temperature' } ] } } } ]))
        except Exception as e:
            msg = str(e)
            print("pressureBMP error ",msg)            
        else:                  
            return result
        
    def altitudeBMP(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR BMP ALTITUDE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.WeatherStation.aggregate([{'$match':{'$and':[{'Timestamp':{'$gte':start}}, {'Timestamp':{'$lte':end}}]}}, {'$group':{'_id': '$BMP_Altitude','BMP_Altitude': {'$push': '$$ROOT.BMP_Altitude'}}}, {'$project': {'_id': 0, 'max': {'$max': '$BMP_Altitude'}, 'min': {'$min': '$BMP_Altitude'}, 'avg': {'$avg': '$BMP_Altitude'}, 'range': {'$subtract': [{'$max': '$BMP_Altitude'}, {'$min': '$BMP_Altitude'}]}}}])) #Add your Aggregation pipeline here in this function                                                       #$cond:{ if:{$and:[{timestamp:{$gte:start}}, {timestamp:{$lte:end}}]}, then:{$max:temperature, $min:temperature, $avg:temperature , $range:[$max:temperature, $min:temperature]}})     
            #result = list(remotedb.ELET2415.climo.aggregate([{ '$match': { 'timestamp': { '$gte': start, '$lte': end } } }, { '$group': { '_id': 0, 'temperature': { '$push': '$$ROOT.temperature' } } }, { '$project': { 'max': { '$max': '$temperature' }, 'min': { '$min': '$temperature' }, 'avg': { '$avg': '$temperature' }, 'range': { '$subtract': [ { '$max': '$temperature' }, { '$min': '$temperature' } ] } } } ]))
        except Exception as e:
            msg = str(e)
            print("altitudeBMP error ",msg)            
        else:                  
            return result
    
    def soilmoisture(self,start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR SOIL MOISTURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        try:
            start = int(start)
            end = int(end)
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.WeatherStation.aggregate([{'$match':{'$and':[{'Timestamp':{'$gte':start}}, {'Timestamp':{'$lte':end}}]}}, {'$group':{'_id': '$Soil_Moisture','Soil_Moisture': {'$push': '$$ROOT.Soil_Moisture'}}}, {'$project': {'_id': 0, 'max': {'$max': '$Soil_Moisture'}, 'min': {'$min': '$Soil_Moisture'}, 'avg': {'$avg': '$Soil_Moisture'}, 'range': {'$subtract': [{'$max': '$Soil_Moisture'}, {'$min': '$Soil_Moisture'}]}}}])) #Add your Aggregation pipeline here in this function                                                       #$cond:{ if:{$and:[{timestamp:{$gte:start}}, {timestamp:{$lte:end}}]}, then:{$max:temperature, $min:temperature, $avg:temperature , $range:[$max:temperature, $min:temperature]}})     
            #result = list(remotedb.ELET2415.climo.aggregate([{ '$match': { 'timestamp': { '$gte': start, '$lte': end } } }, { '$group': { '_id': 0, 'temperature': { '$push': '$$ROOT.temperature' } } }, { '$project': { 'max': { '$max': '$temperature' }, 'min': { '$min': '$temperature' }, 'avg': { '$avg': '$temperature' }, 'range': { '$subtract': [ { '$max': '$temperature' }, { '$min': '$temperature' } ] } } } ]))
        except Exception as e:
            msg = str(e)
            print("soilmoisture error ",msg)            
        else:                  
            return result

    def frequencyDistro(self,variable,start, end):
        '''RETURNS THE FREQUENCY DISTROBUTION FOR A SPECIFIED VARIABLE WITHIN THE START AND END DATE RANGE'''
        start = int(start)
        end = int(end)
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.WeatherStation.aggregate([{'$match': {'Timestamp': { '$gte': start, '$lte': end}}}, {'$bucket': {'groupBy': f"${variable}",'boundaries': list(range(101)), 'default': 'outliers','output': {'count': { '$sum': 1 }}}}]))
                            # ([{'$match': {'timestamp': {'$gte': start,'$lte': end}}},{'$bucket': {'groupBy': variable, 'boundaries': [0, 100], 'default': 'outliers', 'output': {'count': {'$sum': 1}}}}]))
        except Exception as e:
            msg = str(e)
            print("frequencyDistro error ",msg)            
        else:                  
            return result
        
 

def main():
    from config import Config
    from time import time, ctime, sleep
    from math import floor
    from datetime import datetime, timedelta
    one = DB(Config)
 
 
    start = time() 
    end = time()
    print(f"completed in: {end - start} seconds")
    
if __name__ == '__main__':
    main()


    

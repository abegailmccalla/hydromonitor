import {defineStore} from 'pinia'
import {ref} from 'vue'
    
export const useAppStore =  defineStore('app', ()=>{

    /*  
    The composition API way of defining a Pinia store
    ref() s become state properties
    computed() s become getters
    function() s become actions  
    */ 
    const getAllInRange = async (start,end)=>{
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000); 
        const URL = `/api/WeatherStation/get/${start}/${end}`;
        try {
            const response = await fetch(URL,{ method: 'GET', signal: signal }); 
            if (response.ok){
                const data = await response.json();
                let keys = Object.keys(data);
                if(keys.includes("status")){
                    if(data["status"] == "found" ){
                        console.log(data["data"] ) 
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){ 
                        console.log("getAllInRange returned no data");
                    } 
                } 
            }
            else{ 
                const data = await response.text();
                console.log(data);
            }
        }
        catch(err){ 
            console.error('getAllInRange error: ', err.message); 
        } 
        return [] 
    }

    const getDHTTemperatureMMAR = async (start,end)=>{
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000); 
        const URL = `/api/mmar/dhttemperature/${start}/${end}`; 
        try {
            const response = await fetch(URL,{ method: 'GET', signal: signal }); 
            if (response.ok){
                const data = await response.json();
                let keys = Object.keys(data);
                if(keys.includes("status")){
                    if(data["status"] == "found" ){
                        console.log(data["data"] ) 
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){ 
                        console.log("getDHTTemperatureMMAR returned no data");
                    } 
                } 
            }
            else{ 
                const data = await response.text();
                console.log(data);
            }
        }
        catch(err){ 
            console.error('getDHTTemperatureMMAR error: ', err.message); 
        } 
        return [] 
    }

    const getDHTHumidityMMAR = async (start,end)=>{
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000); 
        const URL = `/api/mmar/dhthumidity/${start}/${end}`; 
        try {
            const response = await fetch(URL,{ method: 'GET', signal: signal }); 
            if (response.ok){
                const data = await response.json();
                let keys = Object.keys(data);
                if(keys.includes("status")){
                    if(data["status"] == "found" ){
                        console.log(data["data"] ) 
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){ 
                        console.log(" getDHTHumidityMMAR returned no data");
                    } 
                } 
            }
            else{ 
                const data = await response.text();
                console.log(data);
            }
        }
        catch(err){ 
            console.error(' getDHTHumidityMMAR error: ', err.message); 
        } 
        return [] 
    }

    const getDHTHeatIndexMMAR = async (start,end)=>{
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000); 
        const URL = `/api/mmar/dhtheatindex/${start}/${end}`; 
        try {
            const response = await fetch(URL,{ method: 'GET', signal: signal }); 
            if (response.ok){
                const data = await response.json();
                let keys = Object.keys(data);
                if(keys.includes("status")){
                    if(data["status"] == "found" ){
                        console.log(data["data"] ) 
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){ 
                        console.log(" getDHTHeatIndexMMAR returned no data");
                    } 
                } 
            }
            else{ 
                const data = await response.text();
                console.log(data);
            }
        }
        catch(err){ 
            console.error(' getDHTHeatIndexMMAR error: ', err.message); 
        } 
        return [] 
    }

    const getBMPTemperatureMMAR = async (start,end)=>{
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000); 
        const URL = `/api/mmar/bmptemperature/${start}/${end}`; 
        try {
            const response = await fetch(URL,{ method: 'GET', signal: signal }); 
            if (response.ok){
                const data = await response.json();
                let keys = Object.keys(data);
                if(keys.includes("status")){
                    if(data["status"] == "found" ){
                        console.log(data["data"] ) 
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){ 
                        console.log("getBMPTemperatureMMAR returned no data");
                    } 
                } 
            }
            else{ 
                const data = await response.text();
                console.log(data);
            }
        }
        catch(err){ 
            console.error('getBMPTemperatureMMAR error: ', err.message); 
        } 
        return [] 
    }

    const getBMPPressureMMAR = async (start,end)=>{
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000); 
        const URL = `/api/mmar/bmppressure/${start}/${end}`; 
        try {
            const response = await fetch(URL,{ method: 'GET', signal: signal }); 
            if (response.ok){
                const data = await response.json();
                let keys = Object.keys(data);
                if(keys.includes("status")){
                    if(data["status"] == "found" ){
                        console.log(data["data"] ) 
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){ 
                        console.log("getBMPPressureMMAR returned no data");
                    } 
                } 
            }
            else{ 
                const data = await response.text();
                console.log(data);
            }
        }
        catch(err){ 
            console.error('getBMPPressureMMAR error: ', err.message); 
        } 
        return [] 
    }

    const getBMPAltitudeMMAR = async (start,end)=>{
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000); 
        const URL = `/api/mmar/bmpaltitude/${start}/${end}`; 
        try {
            const response = await fetch(URL,{ method: 'GET', signal: signal }); 
            if (response.ok){
                const data = await response.json();
                let keys = Object.keys(data);
                if(keys.includes("status")){
                    if(data["status"] == "found" ){
                        console.log(data["data"] ) 
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){ 
                        console.log("getBMPAltitudeMMAR returned no data");
                    } 
                } 
            }
            else{ 
                const data = await response.text();
                console.log(data);
            }
        }
        catch(err){ 
            console.error('getBMPAltitudeMMAR error: ', err.message); 
        } 
        return [] 
    }

    const getSoilMoistureMMAR = async (start,end)=>{
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000); 
        const URL = `/api/mmar/soil/${start}/${end}`; 
        try {
            const response = await fetch(URL,{ method: 'GET', signal: signal }); 
            if (response.ok){
                const data = await response.json();
                let keys = Object.keys(data);
                if(keys.includes("status")){
                    if(data["status"] == "found" ){
                        console.log(data["data"] ) 
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){ 
                        console.log("getSoilMoistureMMAR returned no data");
                    } 
                } 
            }
            else{ 
                const data = await response.text();
                console.log(data);
            }
        }
        catch(err){ 
            console.error('getSoilMoistureMMAR error: ', err.message); 
        } 
        return [] 
    }
          
    const  getFreqDistro = async (variable, start, end)=>{
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000); 
        const URL = `/api/frequency/${variable}/${start}/${end}`;
        try {
            const response = await fetch(URL,{ method: 'GET', signal: signal }); 
            if (response.ok){
                const data = await response.json();
                let keys = Object.keys(data);
                if(keys.includes("status")){
                    if(data["status"] == "found" ){
                        console.log(data["data"] ) 
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){ 
                        console.log("getFreqDistro returned no data");
                    } 
                } 
            }
            else{ 
                const data = await response.text();
                console.log(data);
            }
        }
        catch(err){ 
            console.error('getFreqDistro error: ', err.message); 
        } 
        return [] 
    }

    // STATES 
  


    // ACTIONS
    

    return { 
        // EXPORTS	
        getAllInRange, getDHTTemperatureMMAR, getDHTHumidityMMAR, getDHTHeatIndexMMAR,  getBMPTemperatureMMAR, getBMPPressureMMAR, getBMPAltitudeMMAR, getSoilMoistureMMAR, getFreqDistro,
    }
},{ persist: true  });
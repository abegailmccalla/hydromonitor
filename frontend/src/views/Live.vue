<template>
    <v-container class="container" fluid background-colour="surface" justify = 'center' align = 'center'>
        <!-- ROW 1 -->
        <v-row class="row" max-width="1200px" justify = 'center'>
            <v-col class="col col1" cols="9" align = 'center'>
                <figure class="highcharts-figure">
                    <div id="container">
                    </div>
                </figure>
            </v-col>
            <v-col class="col col2" cols="3" align="center"> 
                <v-card margin-bottom="5" max-width="500" color="primaryContainer" subtitle="DHT Temperature">
                    <v-card-item>
                        <span class="text-h3  text-onPrimaryContainer">
                            {{ dht_temperature }}
                        </span>
                    </v-card-item>
                </v-card>
                <v-card margin-bottom="5" max-width="500" color="tertiaryContainer" subtitle="DHT Heat Index">
                    <v-card-item>
                        <span class="text-h3  text-onTertiaryContainer">
                            {{ dht_heatindex }}
                        </span>
                    </v-card-item>
                </v-card>
            </v-col>
        </v-row>
        <!-- ROW 2 -->
        <v-row class="row" max-width="1200px" justify = 'start'>
            <v-col class="col col3" cols="9" align = 'center'>
                <figure class="highcharts-figure">
                    <div id="container1">
                    </div>
                </figure>
            </v-col>
            <v-col class="col col2" cols="3" align="center">  
                <v-card margin-bottom="5" max-width="500" color="primaryContainer" subtitle="BMP Temperature">
                    <v-card-item>
                        <span class="text-h3  text-onPrimaryContainer">
                            {{ bmp_temperature }}
                        </span>
                    </v-card-item>
                </v-card>
            </v-col>
        </v-row>
        <!-- ROW 3 -->
        <v-row class="row" max-width="1200px" justify = 'start'>
            <v-col class="col" cols="9" align = 'center'>
                <figure class="highcharts-figure">
                    <div id="container2">
                    </div>
                </figure>
            </v-col>
            <v-col class="col col2" cols="3" align="center">  
                <v-card margin-bottom="5" max-width="500" color="secondaryContainer" subtitle="DHT Humidity">
                    <v-card-item>
                        <span class="text-h3  text-onSecondaryContainer">
                            {{ dht_humidity }}
                        </span>
                    </v-card-item>
                </v-card>
            </v-col>
        </v-row>
        <!-- ROW 4 -->
        <v-row class="row" max-width="1200px" justify = 'start'>
            <v-col class="col" cols="9" align = 'center'>
                <figure class="highcharts-figure">
                    <div id="container3">
                    </div>
                </figure>
            </v-col>
            <v-col class="col col2" cols="3" align="center">  
                <v-card margin-bottom="5" max-width="500" color="tertiaryContainer" subtitle="BMP Pressure">
                    <v-card-item>
                        <span class="text-h3  text-onTertiaryContainer">
                            {{ bmp_pressure }}
                        </span>
                    </v-card-item>
                </v-card>
            </v-col>
        </v-row>
        <!-- ROW 5 -->
        <v-row class="row" max-width="1200px" justify = 'start'>
            <v-col class="col" cols="9" align = 'center'>
                <figure class="highcharts-figure">
                    <div id="container4">
                    </div>
                </figure>
            </v-col>
            <v-col class="col col2" cols="3" align="center">  
                <v-card margin-bottom="5" max-width="500" color="tertiaryContainer" subtitle="BMP Altitude">
                    <v-card-item>
                        <span class="text-h3  text-onTertiaryContainer">
                            {{ bmp_altitude }}
                        </span>
                    </v-card-item>
                </v-card>
            </v-col>
        </v-row>
        <!-- ROW 6 -->
        <v-row class="row" max-width="1200px" justify = 'start'>
            <v-col class="col" cols="9" align = 'center'>
                <figure class="highcharts-figure">
                    <div id="container5">
                    </div>
                </figure>
            </v-col>
            <v-col class="col col2" cols="3" align="center">  
                <v-card margin-bottom="5" max-width="500" color="tertiaryContainer" subtitle="Soil Moisture">
                    <v-card-item>
                        <span class="text-h3  text-onTertiaryContainer">
                            {{ soil_moisture }}
                        </span>
                    </v-card-item>
                </v-card>
            </v-col>
        </v-row>
    </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
import { useMqttStore } from "@/store/mqttStore";
import { useAppStore } from "@/store/appStore";
import { storeToRefs } from "pinia";
// Highcharts, Load the exporting module and Initialize exporting module.
import Highcharts from 'highcharts';
import more from 'highcharts/highcharts-more';
import Exporting from 'highcharts/modules/exporting';
Exporting(Highcharts); 
more(Highcharts);

// VARIABLES
const Mqtt = useMqttStore();
const { payload, payloadTopic } = storeToRefs(Mqtt);
// VARIABLES
const router      = useRouter();
const route       = useRoute();  
// const led = reactive({"brightness":255,"nodes":1,"color":{ r: 255, g: 0, b: 255, a: 1 }});
let timer, ID = 1000;
const dhttempHiChart = ref(null); // Chart object
const bmptempHiChart = ref(null); // Chart object
const dhthumidHiChart = ref(null); // Chart object
const bmppressHiChart = ref(null); // Chart object
const bmpaltHiChart = ref(null); // Chart object
const smHiChart = ref(null); // Chart object
const points = ref(10); // Specify the quantity of points to be shown on the live graph simultaneously.
const shift = ref(false); // Delete a point from the left side and append a new point to the right side of the graph.
//const payload           = ref({"Type":"Sensor", "ID":620157646, "DHT_Temperature":0, "BMP_Temperature":0, "DHT_Humidity":0, "DHT_HeatIndex":0, "BMP_Pressure":0, "BMP_Altitude":0, "Soil_Moisture":0});

// FUNCTIONS
const CreateCharts = async () => {
    // DHT TEMPERATURE CHART
    dhttempHiChart.value = Highcharts.chart('container', {
    chart: { zoomType: 'x' },
    title: { text: 'DHT Air Temperature and Heat Index Analysis', align: 'left' },
    yAxis: { 
        title: { text: 'DHT Air Temperature & Heat Index' , style:{color:'#000000'}},
        labels: { format: '{value} °C' } 
    },
    xAxis: {
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} },
    },
    tooltip: { shared:true, },
    series: [
        {
            name: 'DHT Temperature',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[0]
        }, 
        {
            name: 'DHT Heat Index',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[1]
        }],
    });

    // BMP TEMPERATURE CHART
    bmptempHiChart.value = Highcharts.chart('container1', {
    chart: { zoomType: 'x' },
    title: { text: 'BMP Air Temperature Analysis', align: 'left' },
    yAxis: { 
        title: { text: 'BMP Air Temperature' , style:{color:'#000000'}},
        labels: { format: '{value} °C' } 
    },
    xAxis: {
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} },
    },
    tooltip: { shared:true, },
    series: [
        {
            name: 'BMP Temperature',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[2]
        }],
    });

    // DHT HUMIDITY CHART
    dhthumidHiChart.value = Highcharts.chart('container2', {
    chart: { zoomType: 'x' },
    title: { text: 'DHT Humidity Analysis', align: 'left' },
    yAxis: { 
        title: { text: 'DHT Humidity' , style:{color:'#000000'}},
        labels: { format: '{value} %' } 
    },
    xAxis: {
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} },
    },
    tooltip: { shared:true, },
    series: [
        {
            name: 'DHT Humidity',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[3]
        }],
    });

    // BMP PRESSURE CHART
    bmppressHiChart.value = Highcharts.chart('container3', {
    chart: { zoomType: 'x' },
    title: { text: 'BMP Pressure Analysis', align: 'left' },
    yAxis: { 
        title: { text: 'BMP Pressure' , style:{color:'#000000'}},
        labels: { format: '{value} Pa' } 
    },
    xAxis: {
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} },
    },
    tooltip: { shared:true, },
    series: [
        {
            name: 'BMP Pressure',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[4]
        }],
    });

    // BMP ALTITUDE CHART
    bmpaltHiChart.value = Highcharts.chart('container4', {
    chart: { zoomType: 'x' },
    title: { text: 'BMP Altitude Analysis', align: 'left' },
    yAxis: { 
        title: { text: 'BMP Altitude' , style:{color:'#000000'}},
        labels: { format: '{value} m' } 
    },
    xAxis: {
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} },
    },
    tooltip: { shared:true, },
    series: [
        {
            name: 'BMP Altitude',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[5]
        }],
    });

    // SOIL MOISTURE CHARTS
    smHiChart.value = Highcharts.chart('container5', {
    chart: { zoomType: 'x' },
    title: { text: 'Soil Moisture Level Analysis', align: 'left' },
    yAxis: { 
        title: { text: 'Soil Moisture Level' , style:{color:'#000000'}},
        labels: { format: '{value} %' } 
    },
    xAxis: {
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} },
    },
    tooltip: { shared:true, },
    series: [
        {
            name: 'Soil Moisture Level',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[6]
        }],
    });
};

onMounted(()=>{
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED 
    CreateCharts();
    Mqtt.connect(); // Connect to Broker located on the backend
    setTimeout( ()=>{
        // Subscribe to each topic
        Mqtt.subscribe("620157646"); 
        Mqtt.subscribe("620157646_pub");
    },3000);
});

onBeforeUnmount(()=>{
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
    Mqtt.unsubcribeAll();
});

// COMPUTED PROPERTIES
const dht_temperature = computed(()=>{
if(!!payload.value){
return `${payload.value.DHT_Temperature.toFixed(2)} °C`;
}
});

const bmp_temperature = computed(()=>{
if(!!payload.value){
return `${payload.value.BMP_Temperature.toFixed(2)} °C`;
}
});

const dht_humidity = computed(()=>{
if(!!payload.value){
return `${payload.value.DHT_Humidity.toFixed(2)} %`;
}
});

const dht_heatindex = computed(()=>{
if(!!payload.value){
return `${payload.value.DHT_HeatIndex.toFixed(2)} °C`;
}
});

const bmp_pressure = computed(()=>{
if(!!payload.value){
return `${payload.value.BMP_Pressure.toFixed(2)} Pa`;
}
});

const bmp_altitude = computed(()=>{
if(!!payload.value){
return `${payload.value.BMP_Altitude.toFixed(2)} m`;
}
});

const soil_moisture = computed(()=>{
if(!!payload.value){
return `${payload.value.Soil_Moisture.toFixed(2)} %`;
}
});

// WATCHERS
watch(payload,(data)=> { 
    if(points.value > 0){ points.value -- ; }
    else{ shift.value = true; }
    dhttempHiChart.value.series[0].addPoint({y:parseFloat(data.DHT_Temperature.toFixed(2)) ,x: data.Timestamp * 1000 }, true, shift.value);  
    bmptempHiChart.value.series[0].addPoint({y:parseFloat(data.BMP_Temperature.toFixed(2)) ,x: data.Timestamp * 1000 }, true, shift.value); 
    dhthumidHiChart.value.series[0].addPoint({y:parseFloat(data.DHT_Humidity.toFixed(2)) ,x: data.Timestamp * 1000 }, true, shift.value); 
    dhttempHiChart.value.series[1].addPoint({y:parseFloat(data.DHT_HeatIndex.toFixed(2)) ,x: data.Timestamp * 1000 }, true, shift.value);
    bmppressHiChart.value.series[0].addPoint({y:parseFloat(data.BMP_Pressure.toFixed(2)) ,x: data.Timestamp * 1000 }, true, shift.value);
    bmpaltHiChart.value.series[0].addPoint({y:parseFloat(data.BMP_Altitude.toFixed(2)) ,x: data.Timestamp * 1000 }, true, shift.value);
    smHiChart.value.series[0].addPoint({y:parseFloat(data.Soil_Moisture.toFixed(2)) ,x: data.Timestamp * 1000 }, true, shift.value);      
});
</script>

<style scoped>
/** CSS STYLE HERE */
Figure {
    border: 2px solid black;
}
</style>
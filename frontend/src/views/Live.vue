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
                <v-card margin-bottom="5" max-width="500" color="primaryContainer" subtitle="Temperature">
                    <v-card-item>
                        <span class="text-h3  text-onPrimaryContainer">
                            {{ temperature }}
                        </span>
                    </v-card-item>
                </v-card>
                <v-card margin-bottom="5" max-width="500" color="tertiaryContainer" subtitle="Heat Index">
                    <v-card-item>
                        <span class="text-h3  text-onTertiaryContainer">
                            {{ heatindex }}
                        </span>
                    </v-card-item>
                </v-card>
                <v-card margin-bottom="5" max-width="500" color="secondaryContainer" subtitle="Humidity">
                    <v-card-item>
                        <span class="text-h3  text-onSecondaryContainer">
                            {{ humidity }}
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
        </v-row>
    </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
import { useMqttStore } from "@/store/mqttStore";
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
const led = reactive({"brightness":255,"nodes":1,"color":{ r: 255, g: 0, b: 255, a: 1 }});
let timer, ID = 1000;
const tempHiChart = ref(null); // Chart object
const humidHiChart = ref(null); // Chart object
const points = ref(10); // Specify the quantity of points to be shown on the live graph simultaneously.
const shift = ref(false); // Delete a point from the left side and append a new point to the right side of the graph.

// FUNCTIONS
const CreateCharts = async () => {
    // TEMPERATURE CHART
    tempHiChart.value = Highcharts.chart('container', {
    chart: { zoomType: 'x' },
    title: { text: 'Air Temperature and Heat Index Analysis', align: 'left' },
    yAxis: { 
        title: { text: 'Air Temperature & Heat Index' , style:{color:'#000000'}},
        labels: { format: '{value} °C' } 
    },
    xAxis: {
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} },
    },
    tooltip: { shared:true, },
    series: [
        {
            name: 'Temperature',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[0]
        }, 
        {
            name: 'Heat Index',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[1]
        }],
});
    // HUMIDITY CHART
    humidHiChart.value = Highcharts.chart('container1', {
    chart: { zoomType: 'x' },
    title: { text: 'Humidity Analysis', align: 'left' },
    yAxis: { 
        title: { text: 'Humidity' , style:{color:'#000000'}},
        labels: { format: '{value} %' } 
    },
    xAxis: {
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} },
    },
    tooltip: { shared:true, },
    series: [
        {
            name: 'Humidity',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[0]
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
const indicatorColor = computed(()=>{
    return `rgba(${led.color.r},${led.color.g},${led.color.b},${led.color.a})`
})

const temperature = computed(()=>{
if(!!payload.value){
return `${payload.value.temperature.toFixed(2)} °C`;
}
});

const heatindex = computed(()=>{
if(!!payload.value){
return `${payload.value.heatindex.toFixed(2)} °C`;
}
});

const humidity = computed(()=>{
if(!!payload.value){
return `${payload.value.humidity.toFixed(2)} %`;
}
});

// WATCHERS
watch(payload,(data)=> { 
    if(points.value > 0){ points.value -- ; }
    else{ shift.value = true; }
    tempHiChart.value.series[0].addPoint({y:parseFloat(data.temperature.toFixed(2)) ,x: data.timestamp * 1000 }, true, shift.value); 
    tempHiChart.value.series[1].addPoint({y:parseFloat(data.heatindex.toFixed(2)) ,x: data.timestamp * 1000 }, true, shift.value); 
    humidHiChart.value.series[0].addPoint({y:parseFloat(data.humidity.toFixed(2)) ,x: data.timestamp * 1000 }, true, shift.value); 
})
</script>

<style scoped>
/** CSS STYLE HERE */
Figure {
    border: 2px solid black;
}
</style>
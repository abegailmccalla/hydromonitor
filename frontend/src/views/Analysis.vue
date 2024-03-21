<template>
  <v-container class="container" fluid>
    <!-- ROW 1 -->
    <v-row class="row1">
      <v-col>
        <v-sheet class="sheet">
          <p>Enter date range for Analysis</p>
          <v-divider></v-divider>
          <br />
            <v-text-field v-model="start" label="Start date" type="Date" dense solo-inverted class="mr-5" :style="{ maxWidth: '300px' }" flat></v-text-field>
            <v-text-field v-model="end" label="End date" type="Date" dense solo-inverted class="mr-5" :style="{ maxWidth: '300px' }" flat></v-text-field>
          <br />
          <v-btn text="Analyze" @click="updateLineCharts(); updateCards(); updateScatter();" color="primary" variant="tonal"></v-btn>
        </v-sheet>
      </v-col>
    </v-row>
    <!-- ROW 2 -->
    <v-row class="row2">
      <v-col cols="12">
        <figure class="highcharts-figure">
          <div id="container"></div>
        </figure>
      </v-col>
    </v-row>
    <!-- ROW 3 -->
    <v-row class="row3">
      <v-col cols="3" align="center">
        <v-card border title="DHT Temperature" width="250" variant="outlined" color="primary" density="compact" rounded="lg">
          <v-card-item class="mb-n5">
            <v-chip-group class="d-flex flex-row justify-center" color="primaryContainer" variant="flat">
              <v-tooltip text="Min" location="start">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhttemperature.min }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Range" location="top">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhttemperature.range }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Max" location="end">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhttemperature.max }}</v-chip>
                </template>
              </v-tooltip>
            </v-chip-group>
          </v-card-item>
          <v-card-item align="center">
            <span class="text-h1 text-primary font-weight-bold">
              {{ dhttemperature.avg }}
            </span>
          </v-card-item>
        </v-card>
      </v-col>
      <v-col cols="3" align="center">
        <v-card border title="DHT Heat Index" width="250" variant="outlined" color="primary" density="compact" rounded="lg">
          <v-card-item class="mb-n5">
            <v-chip-group class="d-flex flex-row justify-center" color="primaryContainer" variant="flat">
              <v-tooltip text="Min" location="start">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhtheatindex.min }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Range" location="top">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhtheatindex.range }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Max" location="end">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhtheatindex.max }}</v-chip>
                </template>
              </v-tooltip>
            </v-chip-group>
          </v-card-item>
          <v-card-item align="center">
            <span class="text-h1 text-primary font-weight-bold">
              {{ dhtheatindex.avg }}
            </span>
          </v-card-item>
        </v-card>
      </v-col>
    </v-row>
    <!-- ROW 4 -->
    <v-row class="row4">
      <v-col cols="12">
        <figure class="highcharts-figure">
          <div id="container0"></div>
        </figure>
      </v-col>
    </v-row>
    <!-- ROW 5 -->
    <v-row class="row5">
      <v-col cols="3" align="center">
        <v-card border title="DHT Temperature" width="250" variant="outlined" color="primary" density="compact" rounded="lg">
          <v-card-item class="mb-n5">
            <v-chip-group class="d-flex flex-row justify-center" color="primaryContainer" variant="flat">
              <v-tooltip text="Min" location="start">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhttemperature.min }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Range" location="top">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhttemperature.range }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Max" location="end">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhttemperature.max }}</v-chip>
                </template>
              </v-tooltip>
            </v-chip-group>
          </v-card-item>
          <v-card-item align="center">
            <span class="text-h1 text-primary font-weight-bold">
              {{ dhttemperature.avg }}
            </span>
          </v-card-item>
        </v-card>
      </v-col>
      <v-col cols="3" align="center">
        <v-card border title="BMP Temperature" width="250" variant="outlined" color="primary" density="compact" rounded="lg">
          <v-card-item class="mb-n5">
            <v-chip-group class="d-flex flex-row justify-center" color="primaryContainer" variant="flat">
              <v-tooltip text="Min" location="start">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ bmptemperature.min }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Range" location="top">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ bmptemperature.range }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Max" location="end">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ bmptemperature.max }}</v-chip>
                </template>
              </v-tooltip>
            </v-chip-group>
          </v-card-item>
          <v-card-item align="center">
            <span class="text-h1 text-primary font-weight-bold">
              {{ bmptemperature.avg }}
            </span>
          </v-card-item>
        </v-card>
      </v-col>
    </v-row>
    <!-- ROW 6 -->
    <v-row class="row6">
      <v-col cols="12">
        <figure class="highcharts-figure">
          <div id="container1"></div>
        </figure>
      </v-col>
    </v-row>
    <!-- ROW 7 -->
    <v-row class="row7">
      <v-col cols="3" align="center">
        <v-card border title="DHT Humidity" width="250" variant="outlined" color="primary" density="compact" rounded="lg">
          <v-card-item class="mb-n5">
            <v-chip-group class="d-flex flex-row justify-center" color="primaryContainer" variant="flat">
              <v-tooltip text="Min" location="start">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhthumidity.min }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Range" location="top">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhthumidity.range }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Max" location="end">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ dhthumidity.max }}</v-chip>
                </template>
              </v-tooltip>
            </v-chip-group>
          </v-card-item>
          <v-card-item align="center">
            <span class="text-h1 text-primary font-weight-bold">
              {{ dhthumidity.avg }}
            </span>
          </v-card-item>
        </v-card>
      </v-col>
    </v-row>
    <!-- ROW 8 -->
    <v-row class="row8">
      <v-col cols="12">
        <figure class="highcharts-figure">
          <div id="container2"></div>
        </figure>
      </v-col>
    </v-row>
    <!-- ROW 9 -->
    <v-row class="row9">
      <v-col cols="5" align="center">
        <v-card border title="BMP Pressure" width="500" variant="outlined" color="primary" density="compact" rounded="lg">
          <v-card-item class="mb-n5">
            <v-chip-group class="d-flex flex-row justify-center" color="primaryContainer" variant="flat">
              <v-tooltip text="Min" location="start">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ bmppressure.min }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Range" location="top">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ bmppressure.range }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Max" location="end">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ bmppressure.max }}</v-chip>
                </template>
              </v-tooltip>
            </v-chip-group>
          </v-card-item>
          <v-card-item align="center">
            <span class="text-h1 text-primary font-weight-bold">
              {{ bmppressure.avg }}
            </span>
          </v-card-item>
        </v-card>
      </v-col>
      <v-col cols="3" >
        <v-card border title="BMP Altitude" width="250" variant="outlined" color="primary" density="compact" rounded="lg">
          <v-card-item class="mb-n5">
            <v-chip-group class="d-flex flex-row justify-center" color="primaryContainer" variant="flat">
              <v-tooltip text="Min" location="start">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ bmpaltitude.min }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Range" location="top">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ bmpaltitude.range }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Max" location="end">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ bmpaltitude.max }}</v-chip>
                </template>
              </v-tooltip>
            </v-chip-group>
          </v-card-item>
          <v-card-item align="center">
            <span class="text-h1 text-primary font-weight-bold">
              {{ bmpaltitude.avg }}
            </span>
          </v-card-item>
        </v-card>
      </v-col>
    </v-row>
    <!-- ROW 10 -->
    <v-row class="row10">
      <v-col cols="12">
        <figure class="highcharts-figure">
          <div id="container3"></div>
        </figure>
      </v-col>
    </v-row>
    <!-- ROW 11 -->
    <v-row class="row11">
      <v-col cols="3" align="center">
        <v-card border title="Soil Moisture" width="250" variant="outlined" color="primary" density="compact" rounded="lg">
          <v-card-item class="mb-n5">
            <v-chip-group class="d-flex flex-row justify-center" color="primaryContainer" variant="flat">
              <v-tooltip text="Min" location="start">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ sm.min }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Range" location="top">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ sm.range }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Max" location="end">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ sm.max }}</v-chip>
                </template>
              </v-tooltip>
            </v-chip-group>
          </v-card-item>
          <v-card-item align="center">
            <span class="text-h1 text-primary font-weight-bold">
              {{ sm.avg }}
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
import {ref,reactive,watch,onMounted,onBeforeUnmount,computed,} from "vue";
import { useRoute, useRouter } from "vue-router";
import { useMqttStore } from "@/store/mqttStore";
import { useAppStore } from "@/store/appStore";
import { storeToRefs } from "pinia";
// Highcharts, Load the exporting module and Initialize exporting module.
import Highcharts from "highcharts";
import more from "highcharts/highcharts-more";
import Exporting from "highcharts/modules/exporting";
import { withDirectives } from "vue";
Exporting(Highcharts);
more(Highcharts);

// VARIABLES
const Mqtt = useMqttStore();
const { payload, payloadTopic } = storeToRefs(Mqtt);
// VARIABLES
const router = useRouter();
const route = useRoute();
let timer,
ID = 1000;
const points = ref(10); // Specify the quantity of points to be shown on the live graph simultaneously.
const shift = ref(false); // Delete a point from the left side and append a new point to the right side of the graph.
const start = ref(null); // Chart object
const end = ref(null); // Chart object
const dhttemperature = reactive({ min: 0, max: 0, avg: 0, range: 0 });
const dhtheatindex = reactive({ min: 0, max: 0, avg: 0, range: 0 });
const bmptemperature = reactive({ min: 0, max: 0, avg: 0, range: 0 });
const dhthumidity = reactive({ min: 0, max: 0, avg: 0, range: 0 });
const bmppressure = reactive({ min: 0, max: 0, avg: 0, range: 0 });
const bmpaltitude = reactive({ min: 0, max: 0, avg: 0, range: 0 });
const sm = reactive({ min: 0, max: 0, avg: 0, range: 0 });
const dhttempandhiHiChart = ref(null); // Chart object
const dhtandbmptemphiHiChart = ref(null); // Chart object
const dhthumidHiChart = ref(null); // Chart object
const scatbmppresandaltHiChart = ref(null); // Chart object
const smHiChart = ref(null); // Chart object
const AppStore = useAppStore();
//const payload           = ref({"Type":"Sensor", "ID":620157646, "DHT_Temperature":0, "BMP_Temperature":0, "DHT_Humidity":0, "DHT_HeatIndex":0, "BMP_Pressure":0, "BMP_Altitude":0, "Soil_Moisture":0});


// FUNCTIONS
const CreateCharts = async () => {
// DHT TEMPERATURE AND HEAT INDEX CHART
dhttempandhiHiChart.value = Highcharts.chart("container", {
  chart: { zoomType: "x" },
  title: { text: "DHT Air Temperature and Heat Index Analysis", align: "left" },
  subtitle: {
    text: 'The heat index, also known as the "apparent temperature," is a measure that combines air temperature and relative humidity to assess how hot it feels to the human body. The relationship between heat index and air temperature is influenced by humidity levels. As humidity increases, the heat index also rises, making the perceived temperature higher than the actual air temperature.',
  },
  yAxis: {
    title: {
      text: "DHT Air Temperature & Heat Index",
      style: { color: "#000000" },
    },
    labels: { format: "{value} °C" },
  },
  xAxis: {
    type: "datetime",
    title: { text: "Time", style: { color: "#000000" } },
  },
  tooltip: {
    pointFormat: "DHT Heat Index: {point.x} % <br/> DHT Temperature: {point.y} °C",
    shared: true,
  },
  series: [
    {
      name: "DHT Temperature",
      type: "spline",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[0],
    },
    {
      name: "DHT Heat Index",
      type: "spline",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[1],
    },
  ],
});
// DHT AND BMP TEMPERATURE CHART
dhtandbmptemphiHiChart.value = Highcharts.chart("container0", {
  chart: { zoomType: "x" },
  title: { text: "DHT and BMP Air Temperature Analysis", align: "left" },
  yAxis: {
    title: {
      text: "DHT and BMP Air Temperature",
      style: { color: "#000000" },
    },
    labels: { format: "{value} °C" },
  },
  xAxis: {
    type: "datetime",
    title: { text: "Time", style: { color: "#000000" } },
  },
  tooltip: {
    pointFormat: "DHT Temperature: {point.x} % <br/> BMP Temperature: {point.y} °C",
    shared: true,
  },
  series: [
    {
      name: "DHT Temperature",
      type: "spline",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[2],
    },
    {
      name: "BMP Temperature",
      type: "spline",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[3],
    },
  ],
});
// DHT HUMIDITY CHART
dhthumidHiChart.value = Highcharts.chart("container1", {
  chart: { zoomType: "x" },
  title: { text: "DHT Humidity Analysis", align: "left" },
  yAxis: {
    title: { text: "DHT Humidity", style: { color: "#000000" } },
    labels: { format: "{value} %" },
  },
  xAxis: {
    type: "datetime",
    title: { text: "Time", style: { color: "#000000" } },
  },
  tooltip: {
    shared: true,
  },
  series: [
    {
      name: "DHT Humidity",
      type: "spline",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[4],
    },
  ],
});
// BMP Pressure and Altitude CHART
scatbmppresandaltHiChart.value = Highcharts.chart("container2", {
  chart: { zoomType: "x" },
  title: {
    text: "BMP Pressure & Altitude Correlation Analysis",
    align: "left",
  },
  subtitle: {
    text: "Visualize the relationship between Pressure and Altitude as well as revealing patterns or trends in the data",
  },
  yAxis: {
    title: {
      text: "Pressure",
      style: { color: "#000000" },
    },
    labels: { format: "{value} Pa" },
  },
  xAxis: {
    title: { text: "Altitude", style: { color: "#000000" } },
    labels: { format: "{value} m" },
  },
  tooltip: {
    shared: true,
    pointFormat: "Altitude: {point.x} m <br/> Pressure: {point.y} Pa",
  },
  series: [
    { 
      name: "Analysis",
      type: "scatter",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[5],
    },
  ],
});
// SOIL MOISTURE LEVEL
smHiChart.value = Highcharts.chart("container3", {
  chart: { zoomType: "x" },
  title: { text: "Soil Moisture Level Analysis", align: "left" },
  yAxis: {
    title: { text: "Soil Moisture Level", style: { color: "#000000" } },
    labels: { format: "{value} %" },
  },
  xAxis: {
    type: "datetime",
    title: { text: "Time", style: { color: "#000000" } },
  },
  tooltip: {
    shared: true,
  },
  series: [
    {
      name: "Soil Moisture Level",
      type: "spline",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[6],
    },
  ],
});
};

const updateCards = async () => {
  // Retrieve Min, Max, Avg, Spread/Range
  if (!!start.value && !!end.value) {
    // 1. Convert start and end dates collected fron TextFields to 10 digit timestamps
    let startDate = new Date(start.value).getTime() / 1000;
    let endDate = new Date(end.value).getTime() / 1000;
    // 2. Fetch data from backend by calling the API functions
    const dhttemp = await AppStore.getDHTTemperatureMMAR(startDate, endDate);
    const dhthumid = await AppStore.getDHTHumidityMMAR(startDate, endDate);
    const dhthi = await AppStore.getDHTHeatIndexMMAR(startDate, endDate);
    const bmptemp = await AppStore.getBMPTemperatureMMAR(startDate, endDate);
    const bmppres = await AppStore.getBMPPressureMMAR(startDate, endDate);
    const bmpalt = await AppStore.getBMPAltitudeMMAR(startDate, endDate);
    const soilm = await AppStore.getSoilMoistureMMAR(startDate, endDate);
    //console.log(temp);
    dhttemperature.max = dhttemp[0].max.toFixed(1);
    dhttemperature.min = dhttemp[0].min.toFixed(1);
    dhttemperature.avg = dhttemp[0].avg.toFixed(1);
    dhttemperature.range = dhttemp[0].range.toFixed(1);
    dhthumidity.max = dhthumid[0].max.toFixed(1);
    dhthumidity.min = dhthumid[0].min.toFixed(1);
    dhthumidity.avg = dhthumid[0].avg.toFixed(1);
    dhthumidity.range = dhthumid[0].range.toFixed(1);
    dhtheatindex.max = dhthi[0].max.toFixed(1);
    dhtheatindex.min = dhthi[0].min.toFixed(1);
    dhtheatindex.avg = dhthi[0].avg.toFixed(1);
    dhtheatindex.range = dhthi[0].range.toFixed(1);
    bmptemperature.max = bmptemp[0].max.toFixed(1);
    bmptemperature.min = bmptemp[0].min.toFixed(1);
    bmptemperature.avg = bmptemp[0].avg.toFixed(1);
    bmptemperature.range = bmptemp[0].range.toFixed(1);
    bmppressure.max = bmppres[0].max.toFixed(1);
    bmppressure.min = bmppres[0].min.toFixed(1);
    bmppressure.avg = bmppres[0].avg.toFixed(1);
    bmppressure.range = bmppres[0].range.toFixed(1);
    bmpaltitude.max = bmpalt[0].max.toFixed(1);
    bmpaltitude.min = bmpalt[0].min.toFixed(1);
    bmpaltitude.avg = bmpalt[0].avg.toFixed(1);
    bmpaltitude.range = bmpalt[0].range.toFixed(1);
    sm.max = soilm[0].max.toFixed(1);
    sm.min = soilm[0].min.toFixed(1);
    sm.avg = soilm[0].avg.toFixed(1);
    sm.range = soilm[0].range.toFixed(1);
  }
};

const updateLineCharts = async () => {
if (!!start.value && !!end.value) {
  // Convert output from Textfield components to 10 digit timestamps
  let startDate = new Date(start.value).getTime() / 1000;
  let endDate = new Date(end.value).getTime() / 1000;
  // Fetch data from backend
  const data = await AppStore.getAllInRange(startDate, endDate);
  // Create arrays for each plot
  let dhttemperature = [];
  let dhtheatindex = [];
  let bmptemperature = [];
  let dhthumidity = [];
  let sm = [];
  // Iterate through data variable and transform object to format recognized by highcharts
  data.forEach((row) => {
    dhttemperature.push({
      x: row.Timestamp * 1000,
      y: parseFloat(row.DHT_Temperature.toFixed(2)),
    });
    dhtheatindex.push({
      x: row.Timestamp * 1000,
      y: parseFloat(row.DHT_HeatIndex.toFixed(2)),
    });
    bmptemperature.push({
      x: row.Timestamp * 1000,
      y: parseFloat(row.BMP_Temperature.toFixed(2)),
    });
    dhthumidity.push({
      x: row.Timestamp * 1000,
      y: parseFloat(row.DHT_Humidity.toFixed(2)),
    });
    sm.push({
      x: row.Timestamp * 1000,
      y: parseFloat(row.Soil_Moisture.toFixed(2)),
    });
  });
  // Add data to Temperature and Heat Index chart
  dhttempandhiHiChart.value.series[0].setData(dhttemperature);
  dhttempandhiHiChart.value.series[1].setData(dhtheatindex);
  dhtandbmptemphiHiChart.value.series[0].setData(dhttemperature);
  dhtandbmptemphiHiChart.value.series[1].setData(bmptemperature);
  dhthumidHiChart.value.series[0].setData(dhthumidity);
  smHiChart.value.series[0].setData(sm);
}
};

const updateScatter = async () => {
if (!!start.value && !!end.value) {
  // Convert output from Textfield components to 10 digit timestamps
  let startDate = new Date(start.value).getTime() / 1000;
  let endDate = new Date(end.value).getTime() / 1000;
  // Fetch data from backend
  const data = await AppStore.getAllInRange(startDate, endDate);
  // Create arrays for each plot
  let scatterPoints = [];
  // Iterate through data variable and transform object to format recognized by highcharts
  data.forEach((row) => {
    scatterPoints.push({
      x: parseFloat(row.BMP_Altitude.toFixed(2)),
      y: parseFloat(row.BMP_Pressure.toFixed(2)),
    });
  });
  // Add data to Temperature and Heat Index chart
  scatbmppresandaltHiChart.value.series[0].setData(scatterPoints);
}
};

onMounted(() => {
// THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
CreateCharts();
Mqtt.connect(); // Connect to Broker located on the backend
setTimeout(() => {
  // Subscribe to each topic
  Mqtt.subscribe("620157646");
  Mqtt.subscribe("620157646_pub");
}, 3000);
});

onBeforeUnmount(() => {
// THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
Mqtt.unsubcribeAll();
});
</script>

<style scoped>
/** CSS STYLE HERE */
.row {
max-width: 1200px;
}

.row1 {
max-width: 1200px;
padding: 1;
}

.col1 {
border: black;
}

.sheet {
padding: 2;
height: 250;
}

Figure {
border: 2px solid black;
}
</style>
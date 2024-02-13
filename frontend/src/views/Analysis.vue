<template>
  <v-container class="container" fluid>
    <v-row class="row1">
      <v-col>
        <v-sheet class="sheet">
          <p>Enter date range for Analysis</p>
          <v-divider></v-divider>
          <br />
          <v-text-field
              v-model="start"
              label="Start date"
              type="Date"
              dense
              solo-inverted
              class="mr-5"
              :style="{ maxWidth: '300px' }"
              flat
          ></v-text-field>
          <v-text-field
              v-model="end"
              label="End date"
              type="Date"
              dense
              solo-inverted
              class="mr-5"
              :style="{ maxWidth: '300px' }"
              flat
          ></v-text-field>
          <br />
          <v-btn
              text="Analyze"
              @click="
                  updateLineCharts();
                  updateCards();
                  updateHistogramCharts();
                  updateScatter();
              "
              color="primary"
              variant="tonal"
          ></v-btn>
      </v-sheet>
      </v-col>
      <v-col cols="3" align="center">
        <v-card
          border
          title="Temperature"
          width="250"
          variant="outlined"
          color="primary"
          density="compact"
          rounded="lg"
        >
          <v-card-item class="mb-n5">
            <v-chip-group
              class="d-flex flex-row justify-center"
              color="primaryContainer"
              variant="flat"
            >
              <v-tooltip text="Min" location="start">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ temperature.min }}</v-chip>
                </template>
              </v-tooltip>

              <v-tooltip text="Range" location="top">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ temperature.range }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Max" location="end">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ temperature.max }}</v-chip>
                </template>
              </v-tooltip>
            </v-chip-group>
          </v-card-item>
          <v-card-item align="center">
            <span class="text-h1 text-primary font-weight-bold">
              {{ temperature.avg }}
            </span>
          </v-card-item>
        </v-card>
      </v-col>
      <v-col cols="3" align="center">
        <v-card
          border
          title="Humidity"
          width="250"
          variant="outlined"
          color="primary"
          density="compact"
          rounded="lg"
        >
          <v-card-item class="mb-n5">
            <v-chip-group
              class="d-flex flex-row justify-center"
              color="primaryContainer"
              variant="flat"
            >
              <v-tooltip text="Min" location="start">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ humidity.min }}</v-chip>
                </template>
              </v-tooltip>

              <v-tooltip text="Range" location="top">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ humidity.range }}</v-chip>
                </template>
              </v-tooltip>
              <v-tooltip text="Max" location="end">
                <template v-slot:activator="{ props }">
                  <v-chip v-bind="props">{{ humidity.max }}</v-chip>
                </template>
              </v-tooltip>
            </v-chip-group>
          </v-card-item>
          <v-card-item align="center">
            <span class="text-h1 text-primary font-weight-bold">
              {{ humidity.avg }}
            </span>
          </v-card-item>
        </v-card>
      </v-col>
    </v-row>
    <v-row class="row">
      <v-col cols="12">
        <figure class="highcharts-figure">
          <div id="container"></div>
        </figure>
      </v-col>
      <v-col cols="12">
        <figure class="highcharts-figure">
          <div id="container0"></div>
        </figure>
      </v-col>
    </v-row>
    <v-row class="row">
      <v-col class="col1" cols="12">
        <figure class="highcharts-figure">
          <div id="container1"></div>
        </figure>
      </v-col>
      <v-col cols="12">
        <figure class="highcharts-figure">
          <div id="container2"></div>
        </figure>
      </v-col>
      <v-col cols="12">
        <figure class="highcharts-figure">
          <div id="container3"></div>
        </figure>
      </v-col>
    </v-row>
  </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
import {
ref,
reactive,
watch,
onMounted,
onBeforeUnmount,
computed,
} from "vue";
import { useRoute, useRouter } from "vue-router";
import { useMqttStore } from "@/store/mqttStore";
import { storeToRefs } from "pinia";
// Highcharts, Load the exporting module and Initialize exporting module.
import Highcharts from "highcharts";
import more from "highcharts/highcharts-more";
import Exporting from "highcharts/modules/exporting";
Exporting(Highcharts);
more(Highcharts);
import { useAppStore } from "@/store/appStore";

// VARIABLES
const Mqtt = useMqttStore();
const { payload, payloadTopic } = storeToRefs(Mqtt);
// VARIABLES
const router = useRouter();
const route = useRoute();
const led = reactive({
brightness: 255,
nodes: 1,
color: { r: 255, g: 0, b: 255, a: 1 },
});
let timer,
ID = 1000;
const points = ref(10); // Specify the quantity of points to be shown on the live graph simultaneously.
const shift = ref(false); // Delete a point from the left side and append a new point to the right side of the graph.
const start = ref(null); // Chart object
const end = ref(null); // Chart object
const temperature = reactive({ min: 0, max: 0, avg: 0, range: 0 });
const humidity = reactive({ min: 0, max: 0, avg: 0, range: 0 });
const tempHiChart = ref(null); // Chart object
const humidHiChart = ref(null); // Chart object
const histoHiChart = ref(null); // Chart object
const scattempHiChart = ref(null); // Chart object
const scathumidHiChart = ref(null); // Chart object
const AppStore = useAppStore();

// FUNCTIONS
const CreateCharts = async () => {
// TEMPERATURE CHART
tempHiChart.value = Highcharts.chart("container", {
  chart: { zoomType: "x" },
  title: { text: "Air Temperature and Heat Index Analysis", align: "left" },
  subtitle: {
    text: 'The heat index, also known as the "apparent temperature," is a measure that combines air temperature and relative humidity to assess how hot it feels to the human body. The relationship between heat index and air temperature is influenced by humidity levels. As humidity increases, the heat index also rises, making the perceived temperature higher than the actual air temperature.',
  },
  yAxis: {
    title: {
      text: "Air Temperature & Heat Index",
      style: { color: "#000000" },
    },
    labels: { format: "{value} °C" },
  },
  xAxis: {
    type: "datetime",
    title: { text: "Time", style: { color: "#000000" } },
  },
  tooltip: {
    pointFormat: "Humidity: {point.x} % <br/> Temperature: {point.y} °C",
    shared: true,
  },
  series: [
    {
      name: "Temperature",
      type: "spline",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[0],
    },
    {
      name: "Heat Index",
      type: "spline",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[1],
    },
  ],
});
// HUMIDITY CHART
humidHiChart.value = Highcharts.chart("container0", {
  chart: { zoomType: "x" },
  title: { text: "Humidity Analysis", align: "left" },
  yAxis: {
    title: { text: "Humidity", style: { color: "#000000" } },
    labels: { format: "{value} %" },
  },
  xAxis: {
    type: "datetime",
    title: { text: "Time", style: { color: "#000000" } },
  },
  tooltip: {
    pointFormat: "Humidity: {point.x} % <br/> Temperature: {point.y} °C",
    shared: true,
  },
  series: [
    {
      name: "Humidity",
      type: "spline",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[0],
    },
  ],
});

histoHiChart.value = Highcharts.chart("container1", {
  chart: { zoomType: "x" },
  title: { text: "Frequency Distribution Analysis", align: "left" },
  yAxis: {
    title: {
      text: "Frequency",
      style: { color: "#000000" },
    },
    labels: { format: "{value}" },
  },
  xAxis: {
    title: { text: "ID", style: { color: "#000000" } },
  },
  tooltip: { shared: true },
  series: [
    {
      name: "Temperature",
      type: "bar",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[0],
    },
    {
      name: "Humidity",
      type: "bar",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[1],
    },
    {
      name: "Heat Index",
      type: "bar",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[3],
    },
  ],
});

scattempHiChart.value = Highcharts.chart("container2", {
  chart: { zoomType: "x" },
  title: {
    text: "Temperature & Heat Index Correlation Analysis",
    align: "left",
  },
  subtitle: {
    text: "Visualize the relationship between Temperature and Heat Index as well as revealing patterns or trends in the data",
  },
  yAxis: {
    title: {
      text: "Heat Index",
      style: { color: "#000000" },
    },
    labels: { format: "{value} °C" },
  },

  xAxis: {
    title: { text: "Temperature", style: { color: "#000000" } },
    labels: { format: "{value} °C" },
  },
  tooltip: {
    shared: true,
    pointFormat: "Temperature: {point.x} °C <br/> Heat Index: {point.y} °C",
  },
  series: [
    {
      name: "Analysis",
      type: "scatter",
      data: [],
      turboThreshold: 0,
      color: Highcharts.getOptions().colors[0],
    },
  ],
});

scathumidHiChart.value = Highcharts.chart("container3", {
  chart: { zoomType: "x" },
  title: {
    text: "Humidity & Heat Index Correlation Analysis",
    align: "left",
  },
  subtitle: {
    text: "Visualize the relationship between Humidity and Heat Index as well as revealing patterns or trends in the data",
  },
  yAxis: {
    title: {
      text: "Heat Index",
      style: { color: "#000000" },
    },
    labels: { format: "{value} °C" },
  },

  xAxis: {
    title: { text: "Humidity", style: { color: "#000000" } },
    labels: { format: "{value} %" },
  },
  tooltip: {
    shared: true,
    pointFormat: "Humidity: {point.x} °C <br/> Heat Index: {point.y} °C",
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
};

const updateLineCharts = async () => {
if (!!start.value && !!end.value) {
  // Convert output from Textfield components to 10 digit timestamps
  let startDate = new Date(start.value).getTime() / 1000;
  let endDate = new Date(end.value).getTime() / 1000;
  // Fetch data from backend
  const data = await AppStore.getAllInRange(startDate, endDate);
  // Create arrays for each plot
  let temperature = [];
  let heatindex = [];
  let humidity = [];
  // Iterate through data variable and transform object to format recognized by highcharts
  data.forEach((row) => {
    temperature.push({
      x: row.timestamp * 1000,
      y: parseFloat(row.temperature.toFixed(2)),
    });
    heatindex.push({
      x: row.timestamp * 1000,
      y: parseFloat(row.heatindex.toFixed(2)),
    });
    humidity.push({
      x: row.timestamp * 1000,
      y: parseFloat(row.humidity.toFixed(2)),
    });
  });
  // Add data to Temperature and Heat Index chart
  tempHiChart.value.series[0].setData(temperature);
  tempHiChart.value.series[1].setData(heatindex);
  humidHiChart.value.series[0].setData(humidity);
}
};

const updateCards = async () => {
  // Retrieve Min, Max, Avg, Spread/Range
  if (!!start.value && !!end.value) {
    // 1. Convert start and end dates collected fron TextFields to 10 digit timestamps
    let startDate = new Date(start.value).getTime() / 1000;
    let endDate = new Date(end.value).getTime() / 1000;
    // 2. Fetch data from backend by calling the API functions
    const temp = await AppStore.getTemperatureMMAR(startDate, endDate);
    const humid = await AppStore.getHumidityMMAR(startDate, endDate);
    //console.log(temp);
    temperature.max = temp[0].max.toFixed(1);
    temperature.min = temp[0].min.toFixed(1);
    temperature.avg = temp[0].avg.toFixed(1);
    temperature.range = temp[0].range.toFixed(1);
    humidity.max = humid[0].max.toFixed(1);
    humidity.min = humid[0].min.toFixed(1);
    humidity.avg = humid[0].avg.toFixed(1);
    humidity.range = humid[0].range.toFixed(1);
  }
};

const updateHistogramCharts = async () => {
// Retrieve Min, Max, Avg, Spread/Range for Column graph
if (!!start.value && !!end.value) {
  // 1. Convert start and end dates collected fron TextFields to 10 digit timestamps
  // Subsequently, create startDate and endDate variables and then save the respective timestamps in these variables
  let startDate = new Date(start.value).getTime() / 1000;
  let endDate = new Date(end.value).getTime() / 1000;
  // 2. Fetch data(temp, humid and hi) from backend by calling the getFreqDistro API functions for each
  const temp = await AppStore.getFreqDistro(
    "temperature",
    startDate,
    endDate
  );
  const humid = await AppStore.getFreqDistro("humidity", startDate, endDate);
  const hi = await AppStore.getFreqDistro("heatindex", startDate, endDate);
  // 3. create an empty array for each variable (temperature, humidity and heatindex)
  // see example below
  let temperature = [];
  let heatindex = [];
  let humidity = [];
  // 4. Iterate through the temp variable, which contains temperature data fetched from the backend
  // transform the data to {"x": x_value,"y": y_value} format and then push it to the temperature array created previously
  // see example below
  temp.forEach((row) => {
    temperature.push({ x: row["_id"], y: row["count"] });
  });
  // 5. Iterate through the humid variable, which contains humidity data fetched from the backend
  // transform the data to {"x": x_value,"y": y_value} format and then push it to the humidity array created previously
  humid.forEach((row) => {
    humidity.push({ x: row["_id"], y: row["count"] });
  });
  // 6. Iterate through the humid variable, which contains heat index data fetched from the backend
  // transform the data to {"x": x_value,"y": y_value} format and then push it to the heatindex array created previously
  hi.forEach((row) => {
    heatindex.push({ x: row["_id"], y: row["count"] });
  });
  // 7. update series[0] for the histogram/Column chart with temperature data
  // see example below
  histoHiChart.value.series[0].setData(temperature);
  // 8. update series[1] for the histogram/Column chart with humidity data
  histoHiChart.value.series[1].setData(humidity);
  // 9. update series[2] for the histogram/Column chart with heat index data
  histoHiChart.value.series[2].setData(heatindex);
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
  let scatterPoints1 = [];
  let scatterPoints2 = [];
  // Iterate through data variable and transform object to format recognized by highcharts
  data.forEach((row) => {
    scatterPoints1.push({
      x: parseFloat(row.temperature.toFixed(2)),
      y: parseFloat(row.heatindex.toFixed(2)),
    });
  });

  data.forEach((row) => {
    scatterPoints2.push({
      x: parseFloat(row.humidity.toFixed(2)),
      y: parseFloat(row.heatindex.toFixed(2)),
    });
  });
  // Add data to Temperature and Heat Index chart
  scattempHiChart.value.series[0].setData(scatterPoints1);
  scathumidHiChart.value.series[0].setData(scatterPoints2);
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
<template>
    <v-container class="container" fluid align="center" justify="center">
        <v-row class="row bg-surfaceVariant" justify="center">
            <v-col class="col col1" align="center"> 
                <v-sheet class="rounded-t-lg mb-1" color="surface" elevation="0" max-width="800" width="100%">
                    <v-card title="LED Controls" color="surface" subtitle="Recent settings" variant="tonal" flat>
                    </v-card>
                </v-sheet>
                <v-sheet clas="mb-1" color="surface" elevation="0" max-width="800" width="100%">
                    <v-card class="pt-5" color="surface" variant="tonal">
                        <v-slider class="pt-2 bg-surface" append-icon="mdi:mdi-car-light-high" density="compact" thumb-size="16" color="secondary" label="Brightness" direction="horizontal"  min="0" max="250" step="10" show-ticks thumb-label="always">
                        </v-slider>
                    </v-card>
                </v-sheet>
                <v-sheet class="mb-1" justify="center" align="center" color="surface" elevation="0" max-width="800" width="100%">
                    <v-card class="pt-5" color="surface" variant="tonal" flat>
                        <v-slider class="pt-2 bg-surface" append-icon="mdi:mdi-led-on" density="compact" thumb-size="16" color="secondary" label="LED Nodes" direction="horizontal"  min="1" max="7" step="1" show-ticks thumb-label="always">
                        </v-slider>
                    </v-card>
                </v-sheet>
                <v-sheet class="mb-1 pa-2 border" justify="center" align="center" color="surface" elevation="0" max-width="800" width="100%">
                    <v-progress-circular rotate="0" size="200" width="15">
                    </v-progress-circular>
                </v-sheet>
            </v-col>
            <v-col class="col col2" align="center"> 
                <v-colorpicker>
                </v-colorpicker>
            </v-col>
        </v-row>
    </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
 
 
// VARIABLES
const router      = useRouter();
const route       = useRoute();  


// FUNCTIONS
onMounted(()=>{
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
});


onBeforeUnmount(()=>{
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
});


</script>


<style scoped>
/** CSS STYLE HERE */
@import url('https://fonts.googleapis.com/css2?family=Noto+Sans+Symbols+2&display=swap');
.digit {
    font-family: 'Noto Sans Symbols 2'; 
    font-size: 250px;
}
.container{
    height: 100%; 
    /** border: 1px solid blue; */
}
.row{
    max-width: 1200px; 
    margin: 10px 0px;
    padding: 10px; 
    /** border: 1px solid purple; */
}
</style>
  
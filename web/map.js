// Initialise Map
var map = L.map('map').setView([49.553, -72.342316], 7);

var lightningStrikes = new Map();

// Lighning Icon
var lightning = L.icon({
    iconUrl:
     'https://cdn.pixabay.com/photo/2012/04/11/12/37/lightning-28003_1280.png',
    iconSize:     [23, 45], 
    iconAnchor:   [0, 45],
    popupAnchor:  [12, -36] 
});

var station = L.icon({
    iconUrl: 'https://www.clipartmax.com/png/middle/169-1690558_radio-antenna-free-icon-satellite-dish-icon-png.png',
    iconSize:     [50, 50], 
    iconAnchor:   [25, 25],
    popupAnchor:  [-3, -50] 
});

// Set tiles with google maps
L.tileLayer('http://mts1.google.com/vt/lyrs=m&x={x}&y={y}&z={z}', {
    maxZoom: 19,
    minZoom: 5,
}).addTo(map);

function drawLightning(lat, long, id, timestamp, websocket) {
    marker = new L.marker([lat,long],{icon: lightning});
    map.addLayer(marker);
    let actualTime = new Date(timestamp * 1000);

    marker.bindPopup(`Time: ${actualTime.toLocaleTimeString("en-GB")} <br/> ID: ${id} <br/> Checked: <button onclick="sendCancel(${id}, ${websocket})"> 
    <svg xmlns="http://www.w3.org/2000/svg" height="1em" viewBox="0 0 448 512"><!--! Font Awesome Free 6.4.0 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license (Commercial License) Copyright 2023 Fonticons, Inc. -->
    <path d="M438.6 105.4c12.5 12.5 12.5 32.8 0 45.3l-256 256c-12.5 12.5-32.8 12.5-45.3 0l-128-128c-12.5-12.5-12.5-32.8 0-45.3s32.8-12.5 45.3 0L160 338.7 393.4 105.4c12.5-12.5 32.8-12.5 45.3 0z"/></svg></button>`);

    // Adds a value to the hashmap storing the marker
    lightningStrikes.set(id, marker);
}

// Takes JSON data and plots points on the map
function setStrikes(json, websocket) {
    var strike_data = JSON.parse(json);
    switch (strike_data.command) {
        case "nodes":
            
            for (let i = 0; i < strike_data.nodes.length; i++) {
                console.log(strike_data.nodes[i].lat);
                drawBaseStation(strike_data.nodes[i].lat, strike_data.nodes[i].lon, 0);
            }
            break;
        case "prev-strikes":
            for (let i = 0; i < strike_data.strikes.length; i++) {
                console.log(strike_data.strikes[i].lat);
                drawLightning(strike_data.strikes[i].lat, strike_data.strikes[i].lon, strike_data.strikes[i].strike_id, strike_data.strikes[i].timestamp, websocket);
                drawThreeStations(strike_data.strikes[i]);
            }
            break;
        case "cancel": 
            removelightning(strike_data.strike_id);
            break;
        case "add":
            drawLightning(strike_data.strike.lat, strike_data.strike.lon, strike_data.strike.id, strike_data.strike.timestamp, websocket);
            drawThreeStations(strike_data.strike);
            break;
        default: 
            console.log("Unsupported JSON instruction recieved");
            break;
    }
}


function drawThreeStations(strike) {
    drawBaseStation(strike.centre_lat_1, strike.centre_lon_1, strike.radius_1);
    drawBaseStation(strike.centre_lat_2, strike.centre_lon_2, strike.radius_2);
    drawBaseStation(strike.centre_lat_3, strike.centre_lon_3, strike.radius_3);
}

// DO NOT call this function directly it should
// only be called by the setStrikes function
// so all users get the updated strikes
function removeLightning (id) {
    marker = lightningStrikes.get(id);
    map.removeLayer(marker);
}

function drawBaseStation(lat, lon, r) {
    console.log(r);
    console.log(lat);
    console.log(lon);
    // Base Station
    L.circle([lat,lon], {
        color: 'black',
        fillColor: 'black',
        fillOpacity: 1,
        radius: 25,
    }).addTo(map);

    // Distance from Base Radius
    L.circle([lat, lon], {
        color: "#3d3d3c",
        fillOpacity: 0,
        // fillColor: "#8a8987",
        // fillOpacity: 0.5,
        radius: r,
    }).addTo(map);
}

// Updates map when sent new data
window.addEventListener("DOMContentLoaded", 
() => {
    // Creates websocket
    const websocket = new WebSocket("ws://192.168.34.222:5678");

    // Recieving Data
    websocket.onmessage = (event) => {
        console.log(event);
        setStrikes(event.data, websocket);
    }
});

// Send cancel message through websocket
function sendCancel(id, websocket) {
    console.log("Inside Listener");
    websocket.send(JSON.stringify({id}));
}

// // Tests
// drawLightning(49.55, -72.34, 10000, "23:00", "");
// drawBaseStation(49.555, -72.345, 5000);

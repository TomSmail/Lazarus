// Initialise Map
var map = L.map('map').setView([51.49843, -0.17693], 14);

var lightningStrikes = new Map();

// Lighning Icon
var lightning = L.icon({
    iconUrl:
     './img/lightning.png',
    iconSize:     [23, 45], 
    iconAnchor:   [0, 45],
    popupAnchor:  [12, -36] 
});

var station = L.icon({
    iconUrl: './img/basestation.png',
    iconSize:     [64, 40], 
    iconAnchor:   [25, 25],
    popupAnchor:  [-3, -50] 
});


// Set tiles with JAWG
L.tileLayer('https://{s}.tile.jawg.io/jawg-light/{z}/{x}/{y}{r}.png?access-token={accessToken}', {
    attribution: '<a href="http://jawg.io" title="Tiles Courtesy of Jawg Maps" target="_blank">&copy; <b>Jawg</b>Maps</a> &copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors',
    maxZoom: 19,
    minZoom: 5,
    subdomains: 'abcd',
    accessToken: 'PJ7QkPxNaAADH2vKKJQO4J9zYicPbQjWt7PkGSQs9WWxMr4ea3y7uRmxoJoDsXKV',
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
    drawBaseStation(strike.centre_lat_1, strike.centre_lon_1);
    drawBaseStation(strike.centre_lat_2, strike.centre_lon_2);
    drawBaseStation(strike.centre_lat_3, strike.centre_lon_3);
}

// DO NOT call this function directly it should
// only be called by the setStrikes function
// so all users get the updated strikes
function removeLightning (id) {
    marker = lightningStrikes.get(id);
    map.removeLayer(marker);
}

function drawBaseStation(lat, lon) {
    // Base Station
    marker = new L.marker([lat,lon],{icon: station});
    map.addLayer(marker);
}

// Updates map when sent new data
window.addEventListener("DOMContentLoaded", 
() => {
    // Creates websocket
    const websocket = new WebSocket("ws://192.168.24.222:5678");

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

// Tests
// drawLightning(51.508963, -0.166851, 10000, "23:00", "");
// drawBaseStation(51.499952, -0.177476);

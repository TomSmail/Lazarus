import json

nodeList = []

nodes_json = {
    "command": "nodes",
    "nodes": nodeList
}

START_LAT = 47.50000
DELTA_LAT = 0.2
START_LON = -75.60000
DELTA_LON = 0.2

NUM_ROWS = 10
NUM_COLS = 10

id = 0
curr_lat = START_LAT
for row in range(NUM_ROWS):
    curr_lat += DELTA_LAT
    curr_lon = START_LON + (row % 2) * DELTA_LON 
    for col in range(NUM_COLS):
        curr_lon += DELTA_LON
        nodeList.append({"lat" : curr_lat, "lon" : curr_lon, "id" : id})
        id += 1
        

print(json.dumps(nodes_json, indent = 2))

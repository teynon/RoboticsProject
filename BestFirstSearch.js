var grid = [];
var open = [];
var current = {};
var start =  {};
var goal = {};
var openCells = 0;
var path = [];
var pathIndex = 0;

// init grid
for (var x = 0; x < 10; x++) {
    grid[x] = [];
    open[x] = [];
    for (var y = 0; y < 10; y++) {
        grid[x][y] = 0;
        open[x][y] = true;
        openCells++;
    }
}

open[5][4] = false;

open[0][0] = false;
openCells--;

start.x = 0;
start.y = 0;

goal.x = 5;
goal.y = 5;

function genericDistance(x1, y1, x2, y2) {
    return Math.abs(x2 - x1) + Math.abs(y2 - y1);
}

current = start;

path[pathIndex++] = current;

while (openCells > 0) {
    open[current.x][current.y] = false;
    
    if (current.x == goal.x && current.y == goal.y) {
        break;
    }
    
    var nodeDistances = { 
        node1: 1000,
        node2 : 1000,
        node3 : 1000,
        node4 : 1000
    };
    
    // Get the best node.
    // Search adjacent tiles.
    var distance = 1000;
    
    var node = {};
    node.x = -1;
    node.y = -1;
    if (current.x - 1 > 0 && open[current.x - 1][current.y]) {
        node.x = current.x - 1;
        node.y = current.y;
        
        distance = genericDistance(current.x - 1, current.y, goal.x, goal.y);
    }
    
    if (current.y - 1 > 0 && open[current.x][current.y - 1] && (genericDistance(current.x, current.y - 1, goal.x, goal.y) < distance)) {
        node.x = current.x;
        node.y = current.y - 1;
        distance = genericDistance(current.x, current.y - 1, goal.x, goal.y);
    }
    
    if (current.x + 1 < 10 && open[current.x + 1][current.y] && genericDistance(current.x + 1, current.y, goal.x, goal.y) < distance) {
        node.x = current.x + 1;
        node.y = current.y;
        distance = genericDistance(current.x + 1, current.y, goal.x, goal.y);
    }
    
    if (current.y + 1 < 10 && open[current.x][current.y + 1] && genericDistance(current.x, current.y + 1, goal.x, goal.y) < distance) {
        node.x = current.x;
        node.y = current.y + 1;
        distance = genericDistance(current.x, current.y + 1, goal.x, goal.y);
    }
    
    if (node.x == -1 || node.y == -1) {
        // We need to step backwards until a node is available.
        pathIndex -= 2;
        current = path[pathIndex];
        continue;
    }
    current = node;
    path[pathIndex++] = current;
}

console.log(path);

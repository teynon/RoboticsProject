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

open[3][5] = false;
open[2][5] = false;
open[2][4] = false;
open[2][3] = false;
open[8][6] = false;
open[8][7] = false;
open[8][5] = false;
open[8][4] = false;
open[8][3] = false;
open[8][2] = false;
open[8][1] = false;
open[6][1] = false;
open[6][2] = false;
open[6][3] = false;
open[4][5] = false;
open[4][6] = false;
open[5][4] = false;
open[6][4] = false;
open[7][4] = false;
open[4][7] = false;
open[4][8] = false;
open[4][9] = false;

open[0][0] = false;
openCells--;

start.x = 0;
start.y = 0;

goal.x = 5;
goal.y = 5;
var pathFound = false;

function genericDistance(x1, y1, x2, y2) {
    return Math.abs(x2 - x1) + Math.abs(y2 - y1);
}


    
    // Draw grid / path
    var table = $("<table></table>").appendTo("body");
    var tr = [];
    var td = [];
    for (var x = 0; x < 10; x++) {
        tr[x] = $("<tr></tr>").appendTo(table);
        td[x] = [];
        for (var y = 0; y < 10; y++) {
            td[x][y] = $("<td></td>").appendTo(tr[x]).css({
                width : "50px",
                height : "50px",
                border : "1px solid #000000"
            });
        }
    }
    
    for (var x = 0; x < 10; x++) {
        for (var y = 0; y < 10; y++) {
            var t = '#FF0000';
            if (open[x][y]) t = '#00FF00';
            td[x][y].css({ "background-color": t });
        }
    }

current = start;

path[pathIndex++] = current;

while (openCells > 0) {
    if (current == undefined) break;
    open[current.x][current.y] = false;
    
    if (current.x == goal.x && current.y == goal.y) {
        pathFound = true;
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
        //path[pathIndex] = null;
        path[pathIndex - 1] = null;
        pathIndex -= 1;
        current = path[pathIndex - 1];
        continue;
    }
    current = node;
    path[pathIndex++] = current;
}


if (pathFound) {
    // Clean up path.
    // Work from the goal back to the start position, see if we can shortcut any neighboring nodes.
    var pathCleanedUp = true;
    
    while (pathCleanedUp) {
        pathCleanedUp = false;
        for (var x = pathIndex - 1; x > 0; x--) {
            if (pathCleanedUp) break;
            for (var i = 0; i < pathIndex && i < x - 1; i++) {
                if (pathCleanedUp) break;
                // Node comes before x and is a neighbor of x
                if (genericDistance(path[i].x, path[i].y, path[x].x, path[x].y) == 1) {
                    pathCleanedUp = true;
                    // Trim the inbetween.
                    var newPath = [];
                    for (var z = 0; z <= i; z++) {
                        newPath[z] = { x : path[z].x, y : path[z].y };
                    }
                    var newPathIndex = z;
                    for (var z = x; z < pathIndex; z++) {
                        newPath[newPathIndex++] = { x : path[z].x, y : path[z].y };
                    }
                    path = newPath;
                    pathIndex = newPathIndex;
                }
                else if ((path[i].x != path[x].x && path[i].y == path[x].y) || (path[i].x == path[x].x && path[i].y != path[x].y)) {
                    console.log("Connection:");
                    console.log(path[i]);
                    console.log(path[x]);
                    var connected = true;
                    // If nodes are within line of site and no used nodes are in between.
                    if (path[i].x == path[x].x) {
                        if (path[x].x == 1 && path[x].y == 2) console.log("X shared");
                        // Walk the Y value over until the values are the same or the node is closed.
                        if (path[i].y < path[x].y) {
                            for (var ty = path[i].y + 1; ty < path[x].y; ty++) {
                                console.log("x: " + path[i].x + " y: " + ty);
                                if (!open[path[i].x][ty]) {
                                    connected = false;
                                    break;
                                }
                            }
                        }
                        else {
                            for (var ty = path[i].y - 1; ty > path[x].y; ty--) {
                                if (!open[path[i].x][ty]) {
                                    connected = false;
                                    break;
                                }
                            }
                        }
                    }
                    else if (path[i].y == path[i].y) {
                        // Walk the X value over until the values are the same or the node is closed.
                        if (path[i].x < path[x].x) {
                            for (var ty = path[i].x + 1; ty < path[x].x; ty++) {
                                if (!open[ty][path[i].y]) {
                                    connected = false;
                                    break;
                                }
                            }
                        }
                        else {
                            for (var ty = path[i].x - 1; ty > path[x].x; ty--) {
                                if (!open[ty][path[i].y]) {
                                    connected = false;
                                    break;
                                }
                            }
                        }
                    }
                    
                    if (connected) {
                        console.log("Connected.");
                        var dist = 0;
                        
                        var newPath = [];
                        for (var z = 0; z <= i; z++) {
                            newPath[z] = { x : path[z].x, y : path[z].y };
                        }
                        
                        // Connect the two paths cutting out the inbetween steps.
                        if (path[i].x < path[x].x) {
                            // Increase x until we hit path[x].x.
                            for (var ix = i + 1; path[i].x + dist != path[x].x; ix++) {
                                dist++;
                                newPath[ix] = { x : path[i].x + dist, y : path[i].y };
                            }
                        }
                        else if (path[i].x > path[x].x) {
                            // Increase x until we hit path[x].x.
                            for (var ix = i + 1; path[i].x + dist != path[x].x; ix++) {
                                dist--;
                                newPath[ix] = { x : path[i].x + dist, y : path[i].y };
                            }
                        }
                        else if (path[i].y < path[x].y) {
                            // Increase x until we hit path[x].x.
                            for (var ix = i + 1; path[i].y + dist != path[x].y; ix++) {
                                dist++;
                                newPath[ix] = { x : path[i].x, y : path[i].y + dist };
                            }                            
                        }
                        else if (path[i].y > path[x].y) {
                            // Increase x until we hit path[x].x.
                            for (var ix = i + 1; path[i].y + dist != path[x].y; ix++) {
                                dist--;
                                newPath[ix] = { x : path[i].x, y : path[i].y + dist };
                            }
                        }
                        
                        for (var iz = x; iz < pathIndex; iz++) {
                            newPath[ix++] = path[iz];
                        }
                        path = newPath;
                        pathIndex = ix;
                    }
                }
            }
        }
    }
    
    for (var x = 0; x < pathIndex; x++) {
        td[path[x].x][path[x].y].html("x");
    }
}
else {
    console.log("No path found.");
}

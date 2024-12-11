#include "highways.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define HIGHWAY_POINTS_COUNT 3
#define HIGHWAY_LINES_COUNT 2

static highway_point points[] = {
    {-1000, 0},   //0   
    {0,0},       //1   
    {1000, 0}     //2    
};
static highway_line lines[] = {
    {0,1},
    {1,2}
};

// Graph structure for search algorithm
static int graph[HIGHWAY_POINTS_COUNT][HIGHWAY_POINTS_COUNT]; // indexes of highways
static int highway_cost[HIGHWAY_LINES_COUNT];


//These are all of the objects
static highway_obstruction_object obs_obj_stocks[]{
    {{-500, 0}, true, 100, 100, highway_obstruction_object_type::Rectangle}
};
static highway_obstruction_object obs_obj_opponent = 
    {{500, 0}, false, 200, 0, highway_obstruction_object_type::Circle};


//Internal Prototypes
double distance(highway_point a, highway_point b);
double point_to_segment_distance(highway_point p, highway_point v, highway_point w);
bool is_point_in_rectangle(highway_point p, highway_point rect[4]);
bool does_circle_touch_rectangle(highway_obstruction_object circle, highway_point rect[4]);
void get_available_highways(bool av_highways_arr[]);
bool any_obstacle_on_highway(highway_point highway_rect[]);
void calculate_highway_rect(highway_line * line, highway_point highway_rect[]);
bool obstacle_on_highway(highway_obstruction_object* obs, highway_point highway_rect[]);



// Function to calculate the angle ABC in degrees
double calculate_angle(highway_point A, highway_point B, highway_point C) {
    // Calculate vectors AB and BC
    double ABx = B.x - A.x;
    double ABy = B.y - A.y;
    double BCx = C.x - B.x;
    double BCy = C.y - B.y;

    // Calculate dot product and magnitudes of AB and BC
    double dot_product = ABx * BCx + ABy * BCy;
    double magnitude_AB = sqrt(ABx * ABx + ABy * ABy);
    double magnitude_BC = sqrt(BCx * BCx + BCy * BCy);

    // Calculate the cosine of the angle
    double cos_theta = dot_product / (magnitude_AB * magnitude_BC);

    // Clamp the value of cos_theta to avoid floating-point issues
    if (cos_theta > 1.0) cos_theta = 1.0;
    if (cos_theta < -1.0) cos_theta = -1.0;

    // Calculate the angle in radians and convert to degrees
    double angle_rad = acos(cos_theta);
    double angle_deg = angle_rad * RAD_TO_DEG;

    return angle_deg;
}

//takes in point indexes
int additionnal_turn_cost(int start, int middle, int end){
    //Lets say that turning costs 500mm + 1mm/deg
    double angle = calculate_angle(points[start], points[middle], points[end]);
    if (angle < 5f)
        return 0
    return ((int)(angle) + 500);
}

// Function to find the shortest path using Dijkstra's algorithm
// Returns the number of points in result[]. If error, returns 0
bool dijkstra(int source, int destination, bool available_highways[], highway_point result[]) {
    int dist[HIGHWAY_POINTS_COUNT], visited[HIGHWAY_POINTS_COUNT];
    int prev[HIGHWAY_POINTS_COUNT];

    for (int i = 0; i < HIGHWAY_POINTS_COUNT; i++) {
        dist[i] = INF;
        visited[i] = 0;
        prev[i] = -1;
    }

    dist[source] = 0;

    for (int i = 0; i < HIGHWAY_POINTS_COUNT - 1; i++) {
        int min_dist = INF, u = -1;

        for (int v = 0; v < HIGHWAY_POINTS_COUNT; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                u = v;
            }
        }

        if (u == -1 || u == destination) break;

        visited[u] = 1;

        for (int v = 0; v < HIGHWAY_POINTS_COUNT; v++) {
            if (!visited[v] && graph[u][v] != -1 && prev[u] != v && available_highways[graph[u][v]]) {
                int cost = highway_cost[graph[u][v]];
                int additional_cost = (prev[u] != -1) ? additionnal_turn_cost(prev[u], u, v) : 0; // Add turn cost if applicable
                if (dist[u] + cost + additional_cost < dist[v]) {
                    dist[v] = dist[u] + cost + additional_cost;
                    prev[v] = u;
                }
            }
        }
    }

    if (dist[destination] == INF) {
        printf("No path exists between the points.\n");
        return 0;
    }

    printf("Shortest path time: %d\n", dist[destination]);
    printf("Path: ");

    int path[HIGHWAY_POINTS_COUNT], path_length = 0;
    for (int at = destination; at != -1; at = prev[at]) {
        path[path_length++] = at;
    }

    for (int i = path_length - 1; i >= 0; i--) {
        printf("%d%s", path[i], (i > 0) ? " -> " : "\n");
        result[i] = path[ path_length - 1 - i ];
    }
    return path_length;
}


void init_highways(){
    for (int i = 0; i < HIGHWAY_POINTS_COUNT; i++) {
        for (int j = 0; j < HIGHWAY_POINTS_COUNT; j++) {
            graph[i][j] = -1;
        }
    }
    for (int i = 0; i < HIGHWAY_LINES_COUNT; i++) {
        highway_point * A = points + lines[i].a;
        highway_point * B = points + lines[i].b;
        double travel_time = distance(*A, *B);
        graph[lines[i].a][lines[i].b] = i;
        graph[lines[i].b][lines[i].a] = i; // Highways are bidirectional
        highway_cost[i] = (int)travel_time;
    }
}
// Returns the number of points in result[]. If error, returns 0
// also adds target point at the end of result[]
int find_fastest_path(highway_point start, highway_point target, highway_point result[]){
    bool av_highways_arr[HIGHWAY_LINES_COUNT];
    get_available_highways(av_highways_arr);

    //TODO change this so it goes to the closes highway, not point, using the min distance to an highway
    int A = 0, B = 0;
    int min_start = INF, min_target = INF;
    for(int i = 0; i < HIGHWAY_POINTS_COUNT; i++){
        int dist_start = distance(start, points[i]);
        int dist_target = disttance(target, points[i]);
        if (dist_start < min_start){
            min_start = dist_start;
            A = i;
        }
        if (dist_target < min_target){
            min_target = dist_target;
            B = i;
        }
    }
    int res = dijkstra(A, B, av_highways_arr, result);
    if (res == 0)
        return 0;
    result[res] = target;
    return res+1;
}


bool unit_tests(){
    highway_point result[HIGHWAY_POINTS_COUNT+1];
    int res = find_fastest_path({-1100, 0}, {1100, 0}, result);
    if (res == 0){
        // No path found, obstacle was blocking
        printf("test success");
    }
    else{
        printf("test fail");
        return false;
    }

    obs_obj_opponent.pos = {-500, 200+150+50};
    res = find_fastest_path({1100, 0}, {-1100, 0}, result);
    if (res != 0){
        // path found
        printf("test success");
    }
    else{
        printf("test fail");
        return false;
    }
    return true;
}


// Fills the bool array, true if highway is available
void get_available_highways(bool av_highways_arr[]){
    for(int line_i = 0; line_i < HIGHWAY_LINES_COUNT; line_i++){
        highway_point highway_rect[4];
        calculate_highway_rect(lines + line_i, highway_rect);
        av_highways_arr[line_i] = !any_obstacle_on_highway(highway_rect);
    }
}

bool any_obstacle_on_highway(highway_point highway_rect[]){
    bool on_highway;
    for(int stock_i = 0; stock_i < 1; stock_i++){
        on_highway = obstacle_on_highway(obs_obj_stocks + stock_i, highway_rect);
        if (on_highway){
            return true;
        }
    }
    on_highway = obstacle_on_highway(&obs_obj_opponent, highway_rect);
    if (on_highway){
        return true;
    }
}

// Calculates a rectangle from a pointer to a line
void calculate_highway_rect(highway_line * line, highway_point highway_rect[]){
    highway_point A = points[line->a];
    highway_point B = points[line->b];

    // Calculate rectangle corners based on line and width
    double half_width = ROBOT_WIDTH / 2.0;

    highway_point rect[4];

    double dx = B.y - A.y;
    double dy = A.x - B.x;
    double length = sqrt(dx * dx + dy * dy);
    dx = (dx / length) * half_width;
    dy = (dy / length) * half_width;

    highway_rect[0] = (highway_point){A.x + dx, A.y + dy};
    highway_rect[1] = (highway_point){A.x - dx, A.y - dy};
    highway_rect[2] = (highway_point){B.x - dx, B.y - dy};
    highway_rect[3] = (highway_point){B.x + dx, B.y + dy};
}

//takes a pointer to an obstruction object
// TODO : Do the opposite, calculate the circle as a bigger radius and compare that to the highway
bool obstacle_on_highway(highway_obstruction_object* obs, highway_point highway_rect[]){
    if (!obs->present)
        return false;
    switch (obs->type)
    {
    case Circle
        // Check if the circle is inside the rectangle
        if (does_circle_touch_rectangle(obs, highway_rect)) {
            printf("The circle is inside the rectangle.\n");
            return true;
        } else {
            printf("The circle is NOT inside the rectangle.\n");
            return false;
        }
        break;
    
    default:
        printf("Not yet implemented obstacle type");
        return false;
        break;
    }
}




// Function to calculate the distance between two points
double distance(highway_point a, highway_point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Function to calculate the shortest distance from a point to a line segment
double point_to_segment_distance(highway_point p, highway_point v, highway_point w) {
    double l2 = distance(v, w) * distance(v, w);
    if (l2 == 0.0) return distance(p, v);

    double t = ((p.x - v.x) * (w.x - v.x) + (p.y - v.y) * (w.y - v.y)) / l2;
    t = fmax(0, fmin(1, t));

    highway_point projection = {
        .x = v.x + t * (w.x - v.x),
        .y = v.y + t * (w.y - v.y)
    };

    return distance(p, projection);
}

// Function to check if a point is inside a rectangle
bool is_point_in_rectangle(highway_point p, highway_point rect[4]) {
    double cross1 = (rect[1].x - rect[0].x) * (p.y - rect[0].y) - (rect[1].y - rect[0].y) * (p.x - rect[0].x);
    double cross2 = (rect[2].x - rect[1].x) * (p.y - rect[1].y) - (rect[2].y - rect[1].y) * (p.x - rect[1].x);
    double cross3 = (rect[3].x - rect[2].x) * (p.y - rect[2].y) - (rect[3].y - rect[2].y) * (p.x - rect[2].x);
    double cross4 = (rect[0].x - rect[3].x) * (p.y - rect[3].y) - (rect[0].y - rect[3].y) * (p.x - rect[3].x);

    return (cross1 <= 0 && cross2 <= 0 && cross3 <= 0 && cross4 <= 0) ||
           (cross1 >= 0 && cross2 >= 0 && cross3 >= 0 && cross4 >= 0);
}

// Function to check if a circular highway_obstruction_object touches or overlaps a rectangle
bool does_circle_touch_rectangle(highway_obstruction_object circle, highway_point rect[4]) {
    // Check if the circle's center is inside the rectangle
    if (is_point_in_rectangle(circle.pos, rect)) {
        return true;
    }

    // Check if the circle touches any of the rectangle's edges
    for (int i = 0; i < 4; i++) {
        highway_point v = rect[i];
        highway_point w = rect[(i + 1) % 4];

        if (point_to_segment_distance(circle.pos, v, w) <= circle.size) {
            return true;
        }
    }

    return false; // The circle does not touch or overlap the rectangle
}
#include "lidarAnalize.h"
#include <math.h>


void convertAngularToAxial(lidarAnalize_t* data, int count, position_t *position,int narrow){
    for(int i = 0; i< count; i++){
        data[i].x = data[i].dist * cos(double((data[i].angle - position->theta) * DEG_TO_RAD)) + position->x;
        data[i].y = -data[i].dist * sin(double((data[i].angle - position->theta) * DEG_TO_RAD)) + position->y;
        //if (i%2 && data[i].angle < 90) {LOG_INFO("X = ", data[i].x, "/ Y =",data[i].y, "dist = ", data[i].dist, "/ angle=",data[i].angle, "/ angle=",position->theta);}
            
        //get table valid
        //TODO : Maybe replace 1600 with 1500 and add narrow_x, narrow_y
        if(data[i].x<1000-narrow && data[i].x>-1000+narrow && data[i].y<1600-narrow && data[i].y>-1600+narrow){
            //LOG_INFO("X = ", data[i].x, "/ Y =",data[i].y, "dist = ", data[i].dist, "/ angle=",data[i].angle);
            //printf("\nx = %i / y = %i / in ? = %i",data[i].x, data[i].y, data[i].x<1100 && data[i].x>-1100 && data[i].y<1700 && data[i].y>-1700);
            data[i].onTable = true;}
        else{data[i].onTable = false;}
    }
}
bool position_opponent(lidarAnalize_t* data, int count, position_t robot_pos, position_t *opponent_pos){
    double som_dist=0,som_angle=0;
    int nb = 0, next_valid;
    for(int i = 0; i < count; i++){        
        if(data[i].onTable){
            som_angle += data[i].angle;
            som_dist += data[i].dist;
            nb ++;
            next_valid = 1;
            while ((!data[i+next_valid].onTable) && ((i+next_valid) <count-1)) {next_valid++;}
            if (fabs(data[i].dist - data[i+next_valid].dist) > 50 || fabs(data[i].angle- data[i+next_valid].angle) > 1){ 
                if (nb < 2) return false;
                som_dist += 30*nb;
                opponent_pos->x = robot_pos.x + som_dist/nb*cos((360 - (int)(som_angle/nb + robot_pos.theta)%360 ) *DEG_TO_RAD) + 55*cos(robot_pos.theta*DEG_TO_RAD);
                opponent_pos->y = robot_pos.y + som_dist/nb*sin((360 - (int)(som_angle/nb + robot_pos.theta)%360) *DEG_TO_RAD) - 55*sin(robot_pos.theta*DEG_TO_RAD);
                //LOG_GREEN_INFO("nb ", nb);
                
                return true;
            }
        }
    }
    opponent_pos->x = robot_pos.x + som_dist/nb*cos((360 - (int)(som_angle/nb + robot_pos.theta)%360 ) *DEG_TO_RAD) + 55*cos(robot_pos.theta*DEG_TO_RAD);
    opponent_pos->y = robot_pos.y + som_dist/nb*sin((360 - (int)(som_angle/nb + robot_pos.theta)%360) *DEG_TO_RAD) - 55*sin(robot_pos.theta*DEG_TO_RAD);
    return true; //TODO : Not quite sure of this one
}

#define BLOBS_COUNT 200 // Define the maximum number of blobs

typedef struct {
    int index_start;
    int index_stop;
    int count;
} lidar_blob_detection;

// Returns the number of blobs found
int find_blobs(lidarAnalize_t* data, int count, lidar_blob_detection* blobs, int min_points, int max_distance){
    LOG_DEBUG("Total lidar point count is ", count, ", minium to be a blob is ", min_points);
    
    int blob_idx = 0; // Count of detected blobs

    // Initialize blobs
    for (int b = 0; b < BLOBS_COUNT; b++) {
        blobs[b].index_start = 0;
        blobs[b].index_stop = 0;
        blobs[b].count = 0;
    }

    // Iterate through points and create blobs
    for (int i = 0; i < count; i++) {
        if (!data[i].onTable){
            if (blobs[blob_idx].count > min_points){
                blob_idx++;
                if (blob_idx == BLOBS_COUNT){
                    LOG_WARNING("position_opponentV2 - No more place for more blobs!");
                    return -1;
                }
            }
            else if (blobs[blob_idx].count > 0){
                blobs[blob_idx].count = 0;
            }
            continue;
        }

        if (blobs[blob_idx].count == 0){
            blobs[blob_idx].index_start = i;
            blobs[blob_idx].index_stop = i;
            blobs[blob_idx].count = 1;
        }
        else if (fabs(data[blobs[blob_idx].index_stop].dist - data[i].dist) > max_distance || data[i].angle - data[blobs[blob_idx].index_stop].angle > 1.0){ //If two points are further than max_distance then create a new blob
            if (blobs[blob_idx].count >= min_points)
                blob_idx++;
            if (blob_idx == BLOBS_COUNT){
                LOG_WARNING("position_opponentV2 - No more place for more blobs!");
                return -1;
            }
            blobs[blob_idx].index_start = i;
            blobs[blob_idx].index_stop = i;
            blobs[blob_idx].count = 1;
        }
        else{ //Add the point to the current blob
            blobs[blob_idx].index_stop = i;
            blobs[blob_idx].count ++;
        }
    }
    //Check for the edge case where opponent is right in front
    if (data[0].onTable && data[count - 1].onTable && fabs(data[0].dist - data[count - 1].dist) < max_distance){
        //Merge the last into the first
        blobs[0].index_start = blobs[blob_idx].index_start;
        blob_idx--;
    }
    return blob_idx;
}

// Dans le pire des cas, on a 0.3% des points qui sont l'ennemi
bool position_opponentV2(lidarAnalize_t* data, int count, position_t robot_pos, position_t *opponent_pos){
    lidar_blob_detection blobs[BLOBS_COUNT]; // Array to hold detected blobs
    int min_points = 4 ; //4 points to be an opponent
    int max_distance = 50; //50mm to be a different blob
    int blob_count = find_blobs(data, count, blobs, min_points, max_distance);


    if (blob_count < 1){
        LOG_DEBUG("Did not find any opponent blob");
        return false;
    }

    int opponent_idx = -1;

    // determination of the linearity of the blobs
    for (int j = 0; j < blob_count; j++) {
        lidar_blob_detection blob = blobs[j];

        // determination of the blob size
        double diam, dist, angle;
        blob_delimiter(data, count, blob, &diam, &dist, &angle);
        if (diam > 100 || diam < 50) continue;
        LOG_DEBUG("Found a blob with diam=", diam, "mm, with ", blob.count, " points at ", data[blob.index_start].angle, " degrees ", dist, "mm far");

        if (opponent_idx != -1){
            LOG_WARNING("Too many opponents blobs were found, stopping");
            return false;
        }
        // Blob considered as opponent 
        opponent_idx = j;
    }

    if (opponent_idx == -1){
        return false;
    }

    // determination of the opponent could be using the bounding box of the blob
    lidar_blob_detection opponent_blob = blobs[opponent_idx];
    // Calculate the centroid of the largest blob
    int pos_sum_x = 0;
    int pos_sum_y = 0;
    if (opponent_blob.index_stop >= opponent_blob.index_start){
        for (int i = opponent_blob.index_start; i <= opponent_blob.index_stop; i++){
            pos_sum_x += data[i].x;
            pos_sum_y += data[i].y;
        }
    }
    else{
        // edgecase
        for (int i = opponent_blob.index_start; i < count; i++){
            pos_sum_x += data[i].x;
            pos_sum_y += data[i].y;
        }
        for (int i = 0; i <= opponent_blob.index_stop; i++){
            pos_sum_x += data[i].x;
            pos_sum_y += data[i].y;
        }
    }
    opponent_pos->x = pos_sum_x / opponent_blob.count;
    opponent_pos->y = pos_sum_y / opponent_blob.count;

    double angle_robot_opponent = atan2(opponent_pos->y - robot_pos.y, opponent_pos->x - robot_pos.x);
    opponent_pos->x += 15*cos(angle_robot_opponent); //Offset the position my 15mm
    opponent_pos->y += 15*sin(angle_robot_opponent);

    return true;
}

//TODO : These can go
void printLidarAxial(lidarAnalize_t* data, int count){
    for(int i = 0; i< count; i++){
        const char* charMessage = "          ";
        if(data[i].onTable){
            charMessage = "non Table ";
        }
        printf("%s theta : %lf \t x : %d \ty : %d\n",charMessage,data[i].angle,data[i].x,data[i].y);
    }
}

void printAngular(lidarAnalize_t* data, int count){
    for(int i = 0; i< count; i++){
        printf("theta: %03.2f \tDist: %08.2f\n",data[i].angle,data[i].dist);
    }
}

void maxDistance(lidarAnalize_t* data, int count,int& maxX, int maxY){
    maxX = 0;
    maxY = 0;
    for(int i = 0; i< count; i++){
        if(abs(data[i].x) > maxX)
            maxX = abs(data[i].x);
        if(abs(data[i].y) > maxY)
            maxY = abs(data[i].y);
    }
}

bool collideFordward(lidarAnalize_t* data, int count){
    for(int i = 0; i <count; i++){
        bool willCollide = data[i].onTable &&
                        (data[i].angle <30 || data[i].angle>(360-30)) &&
                        (data[i].dist < 500);
        if (willCollide) 
            return true;
    }
    return false;
}

bool collideBackward(lidarAnalize_t* data, int count){
    for(int i = 0; i <count; i++){
        bool willCollide = data[i].onTable &&
                        (data[i].angle<(180+30) && data[i].angle>(180-30)) &&
                        (data[i].dist < 500);
        if (willCollide)
            return true;
    }
    return false;
}

int collide(lidarAnalize_t* data, int count ,int distanceStop){
    static int PreviousDistanceStop  = 0;

    //keep direction
    if(distanceStop!=0){
        if(distanceStop >= 0){
            PreviousDistanceStop = 1;
        }
        else{
            PreviousDistanceStop = -1 ;
        }
    }
    else{
        distanceStop = PreviousDistanceStop;
    }

    int iRet = 12000; //maximum capation distance for lidar
    for(int i = 0; i <count; i++){
        if (!data[i].onTable) continue;
        if(distanceStop >= 0){
            if(data[i].angle <45 || data[i].angle>(360-45)){
                if(data[i].dist-distanceStop < iRet){
                    iRet = data[i].dist-distanceStop;
                }
            }
        }
        else{
            if(data[i].angle<(180+45) && data[i].angle>(180-45)){
                if(data[i].dist+distanceStop < iRet){
                    iRet = data[i].dist+distanceStop;
                }
            }
        }
    }
    //LOG_DEBUG("Distance Stop : ",distanceStop);
    //LOG_DEBUG("Collide : ",iRet);
    // LOG_DEBUG("Prec : ",iRetPre);
    return iRet;
}

void supprimerElement(element_decord**& array, int& rows, int index) {
    if (index < 0 || index >= rows) {
        printf("\n ERREUR INDEX SupprimerElement");
        return;} // Assurez-vous que l'index est valide

    element_decord** newArray = new element_decord*[rows - 1];
    
        // Copier les éléments (sauf celui à supprimer) dans le nouveau tableau
        int newIdx = 0;
        for (int i = 0; i < rows; ++i) {
            if (i != index) {
                newArray[newIdx++] = array[i];
            } else {
                delete array[i]; // Libérer la mémoire de l'élément à supprimer
            }
        }

        // Libérer la mémoire de l'ancien tableau
        delete[] array;

        // Réassigner le pointeur et la taille
        array = newArray;
        rows--;
        
}


double distance_2_pts(double d1,double deg1, double d2, double deg2){
    double alpha;
    double d3;
    alpha = fabs((deg1-deg2)*M_PI/180);
    d3 = sqrt(pow(d1,2) + pow(d2,2) -2*d1*d2*cos(alpha));
    return d3;
}


double angle(position_float_t A, position_float_t B, position_float_t C) {
    double a2 = (B.x - C.x) * (B.x - C.x) + (B.y - C.y) * (B.y - C.y);
    double b2 = (A.x - C.x) * (A.x - C.x) + (A.y - C.y) * (A.y - C.y);
    double c2 = (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
    return acos((b2 + c2 - a2) / (2 * sqrt(b2) * sqrt(c2)));
}
int in_table(position_float_t M){
    if (M.x > 1000 || M.x < -1000 || M.y < -1500 || M.y > 1500) return 0;
    else return 1;
}
position_float_t sol_eq_2cercle(position_float_t A, double RA, position_float_t B, double RB){

    // Calcul des distances entre les centres
    double dx = B.x - A.x;
    double dy = B.y - A.y;
    double d = sqrt(dx * dx + dy * dy);

    // Calcul des coordonnées du point d'intersection P2
    double a = (RA * RA - RB * RB + d * d) / (2 * d);
    double h = sqrt(RA * RA - a * a);
    double x2 = A.x + a * (B.x - A.x) / d;
    double y2 = A.y + a * (B.y - A.y) / d;

    // Points d'intersection
    position_float_t P1 = {x2 + h * (B.y - A.y) / d, y2 - h * (B.x - A.x) / d};
    position_float_t P2 = {x2 - h * (B.y - A.y) / d, y2 + h * (B.x - A.x) / d};

    if (in_table(P2)) return P2;
    if (in_table(P1)) return P1;
    //else {printf("\nles deux sont sur la table");}
}

position_float_t position_estime(double alpha12, double alpha23, double alpha31, double angle_poto2, int d01, int d02,int d03) {       
    position_float_t bal_1 = {-950, -1594};
    position_float_t bal_2 = {950, -1594};
    position_float_t bal_3 = {0, 1594};
    double D1_2 = sqrt(pow(bal_1.x - bal_2.x,2)+pow(bal_1.y - bal_2.y,2));
    double D2_3 = sqrt(pow(bal_3.x - bal_2.x,2)+pow(bal_3.y - bal_2.y,2));
    double D1_3 = sqrt(pow(bal_1.x - bal_3.x,2)+pow(bal_1.y - bal_3.y,2));
    double R1,X1,Y1,R2,X2,Y2,R3;

    R1 = D1_2/(2*sin(alpha12*M_PI/180));
    R2 = D2_3/(2*sin(alpha23*M_PI/180));
    R3 = D1_3/(2*sin(alpha31*M_PI/180));

    position_float_t M = {(bal_1.x + bal_2.x) / 2, (bal_1.y + bal_2.y) / 2}; // Calcul du milieu du segment AB
    double d = sqrt((bal_2.x - bal_1.x) * (bal_2.x - bal_1.x) + (bal_2.y - bal_1.y) * (bal_2.y - bal_1.y)); // Calcul de la distance entre A et B
    double h = sqrt(R1 * R1 - (d / 2) * (d / 2)); // Calcul de la distance du milieu au centre
    position_float_t C = {M.x - h * (bal_2.y - bal_1.y) / d, M.y + h * (bal_2.x - bal_1.x) / d}; // Calcul des centres possibles
    position_float_t C11 = {M.x + h * (bal_2.y - bal_1.y) / d, M.y - h * (bal_2.x - bal_1.x) / d}; // Calcul des centres possibles

    position_float_t M2 = {(bal_3.x + bal_2.x) / 2, (bal_3.y + bal_2.y) / 2}; // Calcul du milieu du segment AB
    double d2 = sqrt((bal_2.x - bal_3.x) * (bal_2.x - bal_3.x) + (bal_2.y - bal_3.y) * (bal_2.y - bal_3.y)); // Calcul de la distance entre A et B
    double h2 = sqrt(R2 * R2 - (d2 / 2) * (d2 / 2)); // Calcul de la distance du milieu au centre
    position_float_t C2 = {M2.x - h2 * (bal_2.y - bal_3.y) / d2, M2.y + h2* (bal_2.x - bal_3.x) / d2}; // Calcul des centres possibles
    position_float_t C22 = {M2.x + h2 * (bal_2.y - bal_3.y) / d2, M2.y - h2* (bal_2.x - bal_3.x) / d2}; // Calcul des centres possibles

    position_float_t robot[4];
    robot[0] = sol_eq_2cercle(C,R1,C2,R2);
    robot[1] = sol_eq_2cercle(C11,R1,C2,R2);
    robot[2] = sol_eq_2cercle(C,R1,C22,R2);
    robot[3] = sol_eq_2cercle(C11,R1,C22,R2);

    double min = 10000,distance;
    int index_best=0;
    for (int i = 0; i < 4; i++){
        distance = fabs(d01-sqrt(pow(robot[i].x-bal_1.x,2)+pow(robot[i].y - bal_1.y,2))) + fabs(d02-sqrt(pow(robot[i].x-bal_2.x,2)+pow(robot[i].y - bal_2.y,2))) + fabs(d03-sqrt(pow(robot[i].x-bal_3.x,2)+pow(robot[i].y - bal_3.y,2)));
        //printf("\n distance = %f / x = %f / y = %f", distance, robot[i].x, robot[i].y);
        if (distance < min){
            min = distance;
            index_best = i;
        }
    }
    distance = fabs(alpha12 - angle(robot[index_best],bal_1,bal_2));
    //printf("\n robot : X = %f / Y = %f distance = %f", robot[index_best].x,robot[index_best].y ,distance);  
    position_float_t proj_robot = {bal_2.x,robot[index_best].y};
    double angle_robot = angle(robot[index_best],bal_2,proj_robot)*180/M_PI-angle_poto2;
    if (angle_robot < 0) angle_robot+=360;
    robot[index_best].angle = angle_robot;
    //printf("angle robot = %f",angle_robot); 
    
    return robot[index_best];
}

position_float_t position_estime2(double alpha12, double alpha23, double alpha31){
    position_float_t bal_1 = {-950, -1594};
    position_float_t bal_2 = {950, -1594};
    position_float_t bal_3 = {0, 1594};
    double D1_2 = sqrt(pow(bal_1.x - bal_2.x,2)+pow(bal_1.y - bal_2.y,2));
    double R1 = D1_2/(2*sin(alpha12));
    position_float_t C1, M = {(bal_1.x + bal_2.x) / 2, (bal_1.y + bal_2.y) / 2}; // Calcul du milieu du segment AB
    double d = sqrt((bal_2.x - bal_1.x) * (bal_2.x - bal_1.x) + (bal_2.y - bal_1.y) * (bal_2.y - bal_1.y)); // Calcul de la distance entre A et B
    double h = sqrt(R1 * R1 - (d / 2) * (d / 2)); // Calcul de la distance du milieu au centre
    if (alpha12<M_PI/2) {
        C1.x = M.x - h * (bal_2.y - bal_1.y) / d;
        C1.y = M.y + h * (bal_2.x - bal_1.x) / d;} // Calcul des centres possibles
    else {
        C1.x = M.x + h * (bal_2.y - bal_1.y) / d;
        C1.y =  M.y - h * (bal_2.x - bal_1.x) / d;} // Calcul des centres possibles

    printf("\n pos C_x1 = %f, C_y1 = %f", C1.x, C1.y);
    double  alpha_x,theta = 0,min = 10000,diff; 
    position_float_t robot_finale;
    double a2,a3,somme,angle_step = 2 * M_PI / 100;  // Incrément angulaire

    for (int i = 0; i < 1000; ++i) {
        if (fabs(theta*180/M_PI) > 360) {theta = random() % 360;}
        position_float_t estim_robot = {C1.x + R1 * cos(theta), C1.y + R1 * sin(theta)};
        a2 = angle(estim_robot,bal_3,bal_2);
        a3 = angle(estim_robot,bal_1,bal_3);
        somme = alpha12*180/M_PI+ a2*180/M_PI+a3*180/M_PI;
        if (fabs(somme-360)> 0.5) {
            a2 = 2*M_PI - a2;
            somme = alpha12*180/M_PI+ a2*180/M_PI+a3*180/M_PI;
        }
        if (fabs(somme-360)> 0.5) {
            a3 = 2*M_PI - a3;
            a2 = 2*M_PI - a2;
            somme = alpha12*180/M_PI+ a2*180/M_PI+a3*180/M_PI;
        }

        diff = a2*180/M_PI - alpha23*180/M_PI;
        if (fabs(diff) < min) {min = 0;
            min = fabs(diff);
            robot_finale = estim_robot;
        }
        if (fabs(diff) < 0.001) {break;}
        theta += diff/200;

    }
    return robot_finale;
}


void init_position_balise(lidarAnalize_t* data, int count, position_t *position){
    double deg1,deg2,deg3,distance,somme_angle=0,somme_dist = 0,nb =0;
    int next_valid,ligne =0,rows = 200;//nb de lignes
 
    // Allocation dynamique du tableau
    element_decord** array = new element_decord*[rows];
    for (int i = 0; i < rows; ++i) {
        array[i] = new element_decord; // Allocation de mémoire pour chaque élément
        array[i]->moy_angle = 0; array[i]->nb = 0;array[i]->moy_dist = 0;array[i]->nb = 0; array[i]->i = 0; //initialisation de tt
    }

    //fragmente le décord en plusieurs éléments proches
    for(int i = 0; i <count; i++){
        distance = data[i].dist/1.008;
        if(data[i].onTable){
            somme_angle += data[i].angle;
            somme_dist += distance; nb ++;
            next_valid = 1;
            while ((!data[i+next_valid].onTable) && ((i+next_valid) <count-1)) {next_valid++;}
            if (fabs(distance- data[i+next_valid].dist) > 50 || fabs(data[i].angle- data[i+next_valid].angle) > 1){   // changement d'élément de décord, séparation si écart > 5cm
                array[ligne]->cm = 2*distance_2_pts(distance, data[i].angle, somme_dist/nb,somme_angle/nb);
                array[ligne]->moy_angle = somme_angle/nb; array[ligne]->nb = nb;
                array[ligne]->moy_dist = somme_dist/nb; array[ligne]->i = i;
                somme_angle = 0; somme_dist = 0; nb = 0; ligne ++;
            }
        }
    }
    //supprime le fin du tableau
    while (array[rows-1]->moy_dist == 0){supprimerElement(array, rows, rows -1);}
    
    
    // suppression si nb < 3 ou cm < 3cm ou cm > 20cm
    for (int i= 1; i< rows; i++){
        while (array[rows -1 - i]->nb < 3 || array[rows -1 - i]->cm <30 || array[rows -1 - i]->cm >200){
            if (array[rows -1 - i]->moy_angle > 350 || array[rows -1 - i]->moy_angle < 10) {break;}
            supprimerElement(array, rows, rows -1 -i); 
            if (rows -1 -i < 0){break;}
        }
    }
    if (array[0]->nb < 3 || array[0]->cm <30 || array[0]->cm >200){supprimerElement(array, rows, 0); }
    
    /*
    // Affichage pour vérifier la valeur
    printf("\n");
    for (int l = 0; l < rows; ++l) {
        printf("\n Rows = %i /Angle = %f /Dist = %f /n = %i /i = %i /mm = %lf",l, array[l]->moy_angle,array[l]->moy_dist, array[l]->nb, array[l]->i, array[l]->cm); 
    }
    */

    // donne poto 1 et 2
    double poto_1_2, poto_2_3, poto_3_1, d_tot = 10000, d1,d2,d3, x1,y1, x2,y2,x3,y3;
    int index_poto1, index_poto2, index_poto3; //poto 1 = gauche haut, poto 2 = gauche bas, poto 3 = droite
    poto_1_2 = 1900.0;poto_2_3 = 3326.5; poto_3_1 = 3326.5;
    int L = 3000;

    if (rows >= 3) {
        for (int i= 0; i< rows; i++){
            for (int j= 0; j< rows; j++){
                for (int k=0; k<rows; k++){
                    if (i!=j && i!=k && j!= k){ 
                        double d_1_2 = distance_2_pts(array[i]->moy_dist, array[i]->moy_angle, array[j]->moy_dist, array[j]->moy_angle);
                        double d_2_3 = distance_2_pts(array[j]->moy_dist, array[j]->moy_angle, array[k]->moy_dist, array[k]->moy_angle);
                        double d_3_1 = distance_2_pts(array[k]->moy_dist, array[k]->moy_angle, array[i]->moy_dist, array[i]->moy_angle);
                        distance = fabs(d_1_2-poto_1_2) + fabs(d_2_3-poto_2_3) + fabs(d_3_1-poto_3_1);
                        //printf("\n d_1_2 = %f / d_2_3 =  %f / d_3_1 = %f / distance = %f", d_1_2 , d_2_3 , d_3_1, distance);
                        if (distance< d_tot) {
                            d_tot = distance;
                            index_poto1 = i; index_poto2 = j; index_poto3 = k;
                            d1 = array[i]->moy_dist;
                            d2 = array[j]->moy_dist;
                            d3 = array[k]->moy_dist;
                        }
                    }
                }
            }
        }
        
        double d_1_2 = distance_2_pts(array[index_poto1]->moy_dist, array[index_poto1]->moy_angle, array[index_poto2]->moy_dist, array[index_poto2]->moy_angle);
        double d_2_3 = distance_2_pts(array[index_poto2]->moy_dist, array[index_poto2]->moy_angle, array[index_poto3]->moy_dist, array[index_poto3]->moy_angle);
        double d_3_1 = distance_2_pts(array[index_poto3]->moy_dist, array[index_poto3]->moy_angle, array[index_poto1]->moy_dist, array[index_poto1]->moy_angle);
        distance = fabs(d_1_2-poto_1_2) + fabs(d_2_3-poto_2_3) + fabs(d_3_1-poto_3_1);
        
        deg1 = array[index_poto1]->moy_angle;deg2 = array[index_poto2]->moy_angle;deg3 = array[index_poto3]->moy_angle;
        if ((deg1 < deg3 && deg3< deg2) or (deg3 < deg2 && deg2 < deg1)or (deg2<deg1&& deg1<deg3));
        else {int temp = index_poto1;index_poto1 = index_poto2;index_poto2 = temp;}
        //printf("\n D1 = %f / D2 = %f / D3 = %f / d_1_2 = %f / d_2_3 = %f / d_3_1 = %f / distance = %f",d1,d2,d3, d_1_2, d_2_3, d_3_1,distance);
        
        if (distance > 100) {return;}
        double angle1,angle2, angle3;
        angle1 = array[index_poto1]->moy_angle - array[index_poto2]->moy_angle;
        angle2 = array[index_poto2]->moy_angle - array[index_poto3]->moy_angle;
        angle3 = array[index_poto3]->moy_angle - array[index_poto1]->moy_angle;
        if (angle1 <0) angle1 += 360;if (angle2 < 0) angle2 += 360; if (angle3 < 0) angle3 += 360;

        
        printf("\n angle = %f / %f / %f / somme = %f", angle1, angle3, angle2, angle1 + angle2 + angle3 - 360);
        //position_float_t pos_robot = position_estime(angle1,angle2,angle3,array[index_poto2]->moy_angle,array[index_poto1]->moy_dist,array[index_poto2]->moy_dist,array[index_poto3]->moy_dist);
        position_float_t pos_robot = position_estime2(angle1*M_PI/180,angle3*M_PI/180,angle2*M_PI/180);
        
        printf("\n X = %f / y = %f / angle = %f\n\n", pos_robot.x,pos_robot.y, pos_robot.angle);

    
    }
    // Libération de la mémoire
    for (int i = 0; i < rows; ++i) {
        delete array[i];}
    delete[] array;
}














// Ludovic Bouchard - Beacons V2


int delta_angle(int angle1, int angle2) {
    int delta = angle2 - angle1;

    // Normalize to [-180, 180]
    while (delta > 180) {
        delta -= 360;
    }
    while (delta < -180) {
        delta += 360;
    }

    return delta;
}
double delta_angle_double(double angle1, double angle2) {
    double delta = angle2 - angle1;

    // Normalize to [-180, 180]
    while (delta > 180.0) {
        delta -= 360.0;
    }
    while (delta < -180.0) {
        delta += 360.0;
    }

    return delta;
}
double average_angles(double angles[], int count) {
    double x_sum = 0.0, y_sum = 0.0;
    
    for (int i = 0; i < count; i++) {
        // Convert angle to unit vector
        x_sum += cos(angles[i] * DEG_TO_RAD);
        y_sum += sin(angles[i] * DEG_TO_RAD);
    }

    // Calculate the average vector
    double avg_x = x_sum / count;
    double avg_y = y_sum / count;

    // Convert back to angle
    double avg_angle = atan2(avg_y, avg_x) * RAD_TO_DEG;

    // Normalize angle to [0, 360) if needed
    if (avg_angle < 0) {
        avg_angle += 360.0;
    }

    return avg_angle;
}

bool double_in_table(double x, double y, double robot_padding){
    return (x < 1000.0 - robot_padding &&
        x > -1000.0 + robot_padding &&
        y > -1500.0 + robot_padding &&
        y < 1500.0 - robot_padding);
}

// Calculate the local points to lidar. Inverses angle from clockwise to trig
void local_lidar_point_position(lidarAnalize_t* data, int idx, double* x, double* y){
    *x = data[idx].dist * cos((double)(-data[idx].angle * DEG_TO_RAD));
    *y = data[idx].dist * sin((double)(-data[idx].angle * DEG_TO_RAD));
}

//Calculate the average diameter of a blob, also returns the dist 
void blob_delimiter(lidarAnalize_t* data, int count, lidar_blob_detection blob, double* diam, double* dist, double* angle){
    //Using the min and max angles seen by the lidar of the blob, you can find a circle the circles the blob

    double sum_r = 0;
    if (blob.index_stop >= blob.index_start){
        for (int i = blob.index_start; i <= blob.index_stop; i++) sum_r += data[i].dist;
    }
    else{
        // edgecase where blob right in front
        for (int i = blob.index_start; i < count; i++) sum_r += data[i].dist;
        for (int i = 0; i <= blob.index_stop; i++) sum_r += data[i].dist;
    }

    *dist = sum_r / (double)blob.count;
    double angles[] = {data[blob.index_start].angle, data[blob.index_stop]. angle};
    *angle = average_angles(angles, 2);

    double distance1 = distance_2_pts(*dist, *angle, data[blob.index_start].dist, angles[0]);
    double distance2 = distance_2_pts(*dist, *angle, data[blob.index_stop].dist, angles[1]);

    *diam = 2 * (distance1 > distance2 ? distance1 : distance2);    
}

// Function to calculate the orthogonal distance from a point to the line
double orthogonal_distance(double x, double y, double m, double b) {
    // Line equation: y = mx + b
    double y_line = m * x + b; // Calculate the y value on the line
    return fabs(y - y_line) / sqrt(1 + m * m); // Orthogonal distance formula
}

// The result is local to lidar coordinates.
double linear_regression(lidarAnalize_t* data, int count, lidar_blob_detection blob, double blob_size, double* angle, double* bx, double* by) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
    double x, y;

    if (blob.index_stop >= blob.index_start){
        for (int i = blob.index_start; i <= blob.index_stop; i++){
            local_lidar_point_position(data, i, &x, &y);
            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_x2 += x * x;
        }
    }
    else{
        // edgecase where blob right in front
        for (int i = blob.index_start; i < count; i++){
            local_lidar_point_position(data, i, &x, &y);
            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_x2 += x * x;
        }
        for (int i = 0; i <= blob.index_stop; i++){
            local_lidar_point_position(data, i, &x, &y);
            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_x2 += x * x;
        }
    }

    double denominator = blob.count * sum_x2 - sum_x * sum_x;

    if (denominator == 0) {
        return 0;
    }

    double m = (blob.count * sum_xy - sum_x * sum_y) / denominator;
    double b = (sum_y * sum_x2 - sum_x * sum_xy) / denominator;

    *angle = atan(m) * RAD_TO_DEG;  // Angle of the line in degrees, -90 to 90

    *bx = sum_x / (double)(blob.count);  // Center of the lines
    *by = sum_y / (double)(blob.count);

    // Calculate orthogonal distances and find the mean
    // aka Total least squares
    double sum_distances = 0.0;
    double sum_squared_distances = 0.0;

    if (blob.index_stop >= blob.index_start) {
        for (int i = blob.index_start; i <= blob.index_stop; i++) {
            local_lidar_point_position(data, i, &x, &y);
            double dista = orthogonal_distance(x, y, m, b);
            sum_distances += dista;
            sum_squared_distances += dista * dista;
        }
    }
    else{
        // edgecase where blob right in front
        for (int i = blob.index_start; i < count; i++){
            local_lidar_point_position(data, i, &x, &y);
            double dista = orthogonal_distance(x, y, m, b);
            sum_distances += dista;
            sum_squared_distances += dista * dista;
        }
        for (int i = 0; i <= blob.index_stop; i++){
            local_lidar_point_position(data, i, &x, &y);
            double dista = orthogonal_distance(x, y, m, b);
            sum_distances += dista;
            sum_squared_distances += dista * dista;
        }
    }
    double mean_distance = sum_distances / blob.count;
    double mean_squared_distance = sum_squared_distances / blob.count;

    // Calculate variance and standard deviation
    // double variance = (sum_squared_distances / blob.count) - (mean_distance * mean_distance);
    // double stddev = std::sqrt(variance);

    // Normalize by standard deviation
    // double normalized_score = 1 - ((stddev != 0) ? (mean_distance / stddev) : 0);

    return 100.0 * mean_squared_distance / (blob_size * blob_size);
}

// transforme le vecteur à 0,0 avec 0deg vers l'espace absolue en se basant sur le vecteur 1 vers 1_prime
// returns true if inside the table
bool transform_coordinates(double x1, double y1, double theta1, double x1_prime, double y1_prime, double theta1_prime, double* x, double* y, double* theta){
    // Convert angles from degrees
    *theta = (theta1_prime - theta1);
    double theta_rad = *theta * DEG_TO_RAD;

    // Calculate the absolute coordinates
    *x = - x1 * cos(theta_rad) + y1 * sin(theta_rad) + x1_prime;
    *y = - x1 * sin(theta_rad) - y1 * cos(theta_rad) + y1_prime;

    // Padding in mm (Bit less than half of the robot size)
    if (double_in_table(*x,*y, 100.0)) return true;

    //Theres two possibility of position, so rotate the vector by 180 degs
    *theta -= 180;
    if (*theta < 0) {
        *theta += 360.0;
    }
    theta_rad = *theta * DEG_TO_RAD;
    
    // Calculate the absolute coordinates
    *x = - x1 * cos(theta_rad) + y1 * sin(theta_rad) + x1_prime;
    *y = - x1 * sin(theta_rad) - y1 * cos(theta_rad) + y1_prime;

    if (double_in_table(*x,*y, 100.0)) return true;

    // Robot is not inside the table
    return false;
}

bool find_beacons_best_fit(position_double_t* beacons_real_pos, position_double_t* beacons_blob_pos, position_t *position){
    double robot_pos_x[BEACONS_COUNT*BEACONS_COUNT];
    double robot_pos_y[BEACONS_COUNT*BEACONS_COUNT];
    double robot_angles[BEACONS_COUNT*BEACONS_COUNT];
    bool robot_ins_table[BEACONS_COUNT*BEACONS_COUNT];
    // We calculate what would be our position for each of the beacons and check the best combo of 3 positions
    for (int i = 0; i < BEACONS_COUNT; i++) { // Iterate blobs
        for (int j = 0; j < BEACONS_COUNT; j++) { //Iterate real beacons
            int idx = i * BEACONS_COUNT + j;
            robot_ins_table[idx] = transform_coordinates(beacons_blob_pos[i].x, beacons_blob_pos[i].y, beacons_blob_pos[i].angle, 
                                beacons_real_pos[j].x, beacons_real_pos[j].y, beacons_real_pos[j].angle, 
                                &robot_pos_x[idx], &robot_pos_y[idx], &robot_angles[idx]);
            if (robot_ins_table[idx])
                LOG_DEBUG("Beacon ", i, " relative to beacon ", j, " could be at\tx=", robot_pos_x[idx], "\ty=", robot_pos_y[idx], "\ttheta=", robot_angles[idx], " degs");
        }
    }

    // We iterate through checking the best combo of beacons
    for (int i = 0; i < BEACONS_COUNT; i++) { //  i=0=> B0=b0 B1=b1 B2=b2    i=1=> B0=b1 B1=b2 B2=b0  ....
        double sum_x = 0, sum_y = 0;
        double angles[BEACONS_COUNT];
        for (int j = 0; j < BEACONS_COUNT; j++) { // Blobs
            int idx = j * BEACONS_COUNT + (j+i) % BEACONS_COUNT;
            sum_x += robot_pos_x[idx];
            sum_y += robot_pos_y[idx];
            angles[j] = robot_angles[idx];
        }
        double avg_x = sum_x / (double)(BEACONS_COUNT);
        double avg_y = sum_y / (double)(BEACONS_COUNT);
        double avg_theta = average_angles(angles, BEACONS_COUNT);

        bool is_solution = true;
        //If any of the points calculated dont fit with the average, go to next combo
        for (int j = 0; j < BEACONS_COUNT; j++) { // Blobs
            int idx = j * BEACONS_COUNT + (j+i) % BEACONS_COUNT;
            double x_error = fabs(avg_x - robot_pos_x[idx]);
            double y_error = fabs(avg_y - robot_pos_y[idx]);
            double theta_error = fabs(delta_angle_double(avg_theta, robot_angles[idx]));
            if (x_error > 100.0 || y_error > 100.0 || theta_error > 10.0){ //mm and degs
                is_solution = false;
                break;
            }
        }
        // Maybe use a weighted average using the linearity coefficient
        if (is_solution){
            LOG_GREEN_INFO("Found robot position using beacons, x = ", avg_x, ", y = ", avg_y, " theta = ", avg_theta);
            position->x = round(avg_x);
            position->y = round(avg_y);
            position->theta = round(avg_theta);
            return true;
        }
    }
    return false;
}

beacon_detection_t beacon_detection = {
    .valid = false,
    .beacon_count = BEACONS_COUNT,
};

bool position_robot_beacons(lidarAnalize_t* data, int count, position_t *position, colorTeam_t team_color, colorTeam_t* out_team_color){
    beacon_detection.valid = false;

    // Propagate exterior points
    // 120mm safe zone around beacons
    for (int i = 0; i < count; i++) data[i].onTable = true;
    for (int i = 0; i < count; i++){
        if (data[i].onTable && data[i].dist > 3600){
            data[i].onTable = false;
            int backtrack = i - 1;
            //Backtrack until i = 0 or that the seperation is too big
            while(backtrack >= 0 && data[backtrack].onTable){
                if (fabs(data[backtrack].dist - data[backtrack + 1].dist) > 120.0 || data[backtrack + 1].angle - data[backtrack].angle > 1.0){
                    break;
                }
                data[backtrack].onTable = false;
                backtrack--;
            }
            //Forward until i = count or that the seperation is too big
            int forwtrack = i + 1;
            while(forwtrack < count && data[forwtrack].onTable){
                if (fabs(data[forwtrack].dist - data[forwtrack - 1].dist) > 120.0 || data[forwtrack].angle - data[forwtrack - 1].angle > 1.0){
                    break;
                }
                data[forwtrack].onTable = false;
                forwtrack++;
            }
        }
    }

    lidar_blob_detection blobs[BLOBS_COUNT]; // Array to hold detected blobs
    int min_points = 6 ; //6 points min to be a beacon
    int max_distance = 20; //20mm to be a different blob
    int blob_count = find_blobs(data, count, blobs, min_points, max_distance);


    if (blob_count < BEACONS_COUNT){
        LOG_DEBUG("Found ", blob_count, " blobs");
        return false;
    }

    int beacon_idx = 0;

    // determination of the linearity of the blobs
    for (int j = 0; j < blob_count; j++) {
        lidar_blob_detection blob = blobs[j];

        // determination of the blob size
        double diam, dist, angle;
        blob_delimiter(data, count, blob, &diam, &dist, &angle);
        if (diam > 130 || diam < 92) continue; //TODO Might need to be tweaked
        LOG_DEBUG("Found a blob with diam=", diam, "mm, with ", blob.count, " points at ", data[blob.index_start].angle, " degrees ", dist, "mm far");

        // determination of the linearity of the blobs
        double pangle, px, py;
        double mean_deriv = linear_regression(data, count, blob, diam, &pangle, &px, &py);
        LOG_DEBUG("Found a blob with a mean_deriv = ", mean_deriv);

        if (mean_deriv < 0.1){ //TODO Might need to be tweaked
            if (beacon_idx == BEACONS_COUNT){
                LOG_WARNING("Too many beacons were found, stopping");
                return false;
            }
            beacon_detection.beacons_rel_pos[beacon_idx] = {px, py, pangle};
            beacon_detection.diameters[beacon_idx] = diam;

            LOG_DEBUG("Beacon ", beacon_idx, " is relative to lidar at\tx=", px, "\ty=", py, "\ttheta=", pangle, " degs");
            // Blob considered as beacon 
            beacon_idx++;
            if (beacon_idx == BEACONS_COUNT)
                LOG_DEBUG("search for beacons : ", BEACONS_COUNT, " are found");
        }
        LOG_DEBUG("");
    }
    if (beacon_idx != BEACONS_COUNT){
        LOG_DEBUG("Did not find the ", BEACONS_COUNT, " beacons");
        return false;
    }
    beacon_detection.valid = true;

    // Definition of blue beacons
    // 0:Right, 1:BottomLeft 2:TopLeft   - when looking at public perspective   3:Top
    // The order is important, use the same as the spinning of the lidar
    // Those are the positions for blue
    position_double_t beacons_real_pos[BEACONS_COUNT];
    beacons_real_pos[0] = {0, 1594, 0}; 
    beacons_real_pos[1] = {950, -1594, 30}; //TODO : Have the real angle of the beacon
    beacons_real_pos[2] = {-950, -1594, -30};

    // if team color is NONE, its going to find the best fit for both color, starting with blue, only works if 4 beacons (or asymetric disposition)

    if (team_color != YELLOW && find_beacons_best_fit(beacons_real_pos, beacon_detection.beacons_rel_pos, position)){
        if (team_color == NONE)
            *out_team_color = BLUE;
        return true;
    }
    else if (team_color != BLUE){
        // Switch team color to YELLOW, inverse beacons position while keeping the lidar spinning order
        position_double_t inv_beacons_real_pos[BEACONS_COUNT];
        for (int i = 0; i < BEACONS_COUNT; i++){
            inv_beacons_real_pos[BEACONS_COUNT - i - 1].x = beacons_real_pos[i].x;
            inv_beacons_real_pos[BEACONS_COUNT - i - 1].y = -beacons_real_pos[i].y;
            inv_beacons_real_pos[BEACONS_COUNT - i - 1].angle = -beacons_real_pos[i].angle;
        }
        if (find_beacons_best_fit(inv_beacons_real_pos, beacon_detection.beacons_rel_pos, position)){
            if (team_color == NONE)
                *out_team_color = YELLOW;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

// Serialization for beacon_detection_t
void to_json(json& j, const beacon_detection_t& bd) {
    if (!bd.valid){
        j = json{
            {"valid", false}
        };
        return;
    }

    std::vector<json> beacons;
    std::vector<double> diameters;
    for (int i = 0; i < bd.beacon_count; ++i) {
        beacons.push_back(bd.beacons_rel_pos[i]);
        diameters.push_back(bd.diameters[i]);
    }

    // Create the final JSON object
    j = json{
        {"valid", bd.valid},
        {"beacon_count", bd.beacon_count},
        {"beacons_rel_pos", beacons},
        {"diameters", diameters}
    };
}
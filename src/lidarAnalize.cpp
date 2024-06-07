#include "lidarAnalize.h"
#include <math.h>

void convertAngularToAxial(lidarAnalize_t* data, int count, position_t *position,int narrow){
    for(int i = 0; i< count; i++){
        if(data[i].valid){
            data[i].x = data[i].dist*cos(( data[i].angle + position->teta)*DEG_TO_RAD) + position->x ;
            data[i].y = - data[i].dist*sin((data[i].angle+position->teta)*DEG_TO_RAD) + position->y;
            //if (i%2 && data[i].angle < 90) {LOG_INFO("X = ", data[i].x, "/ Y =",data[i].y, "dist = ", data[i].dist, "/ angle=",data[i].angle, "/ angle=",position->teta);}
                
            //get table valid
            if(data[i].x<1200-narrow && data[i].x>-1200+narrow && data[i].y<1800-narrow && data[i].y>-1800+narrow){
                //LOG_INFO("X = ", data[i].x, "/ Y =",data[i].y, "dist = ", data[i].dist, "/ angle=",data[i].angle);
                //printf("\nx = %i / y = %i / in ? = %i",data[i].x, data[i].y, data[i].x<1100 && data[i].x>-1100 && data[i].y<1700 && data[i].y>-1700);
                data[i].onTable = true;}
            else{data[i].onTable = false;}
            
        }
    }
}

void printLidarAxial(lidarAnalize_t* data, int count){
    for(int i = 0; i< count; i++){
        const char* charMessage = "          ";
        if(!data[i].valid){
            charMessage = "non Valide";
        }
        if(data[i].onTable){
            charMessage = "non Table ";
        }
        printf("%s teta : %lf \t x : %d \ty : %d\n",charMessage,data[i].angle,data[i].x,data[i].y);
    }
}

void printAngular(lidarAnalize_t* data, int count){
    for(int i = 0; i< count; i++){
        if(data[i].valid){
            printf("theta: %03.2f \tDist: %08.2f\n",data[i].angle,data[i].dist);
        }
        else{
            printf("non valid\n");
        }
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
        if(data[i].valid && data[i].onTable)
            if(data[i].angle <45 || data[i].angle>(360-45))
                if(data[i].dist < 500){
                    return true;
                }
    }
    return false;
}

bool collideBackward(lidarAnalize_t* data, int count){
    for(int i = 0; i <count; i++){
        if(data[i].valid && data[i].onTable)
            if(data[i].angle<(180+45) && data[i].angle>(180-45))
                if(data[i].dist < 500){
                    return true;
                }
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
        if(data[i].valid && data[i].onTable){    
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
    }
    //LOG_DEBUG("Distance Stop : ",distanceStop);
    //LOG_DEBUG("Collide : ",iRet);
    // LOG_DEBUG("Prec : ",iRetPre);
    return iRet;
}

void pixelArtPrint(lidarAnalize_t* data, int count,int sizeX,int sizeY,int scale,position_t position){
    char* matriceAffichage;
    matriceAffichage = (char*)malloc(sizeX * sizeY * sizeof(char));
    
    //initMatrice
    for(int i = 0; i<sizeX * sizeY; i++){
        matriceAffichage[i] = ' ';
    }

    int posix, posiy;
    for(int i = 0; i<count; i++){
        if(data[i].valid == true){
            posix = data[i].x/scale + sizeX/2;
            posiy = data[i].y/scale + sizeY/2;
            if(posix<sizeX && posix>=0 && posiy<sizeY && posiy>=0){
                if(data[i].onTable)
                    matriceAffichage[posix + sizeX * posiy] = '*';
                else
                    matriceAffichage[posix + sizeX * posiy] = 'X';
            }                
            else{
                if(posix>=sizeX)
                    posix = sizeX-1;
                if(posix<0)
                    posix = 0;
                if(posiy>=sizeY)
                    posiy = sizeY-1;
                if(posiy<0)
                    posiy = 0;
                matriceAffichage[posix + sizeX * posiy] = 'W';
            }
        }
    }
    //fill

    int positionRoboty = position.x/scale + sizeX/2;
    int positionRobotx = position.y/scale + sizeY/2;

    for(int i = 0; i<sizeX; i++){
        char chartype = ' ';
        for(int j = positionRoboty; j<sizeY; j++){
            int posX = MAP(j,positionRoboty,sizeY,positionRobotx,i);
            if(matriceAffichage[posX + sizeX * j] != ' '){
                chartype = matriceAffichage[posX + sizeX * j];
            }                
            if(chartype != ' '){
                matriceAffichage[posX + sizeX * j] = chartype;
            }
        }
    }

    for(int i = 0; i<sizeX; i++){
        char chartype = ' ';
        for(int j = positionRoboty; j>=0; j--){
            int posX = MAP(j,positionRoboty,0,positionRobotx,i);
            if(matriceAffichage[posX + sizeX * j] != ' '){
                chartype = matriceAffichage[posX + sizeX * j];
            }                
            if(chartype != ' '){
                matriceAffichage[posX + sizeX * j] = chartype;
            }
        }
    }


    for(int j = 0; j<sizeY; j++){
        char chartype = ' ';
        for(int i = positionRobotx; i<sizeX; i++){
            int posY = MAP(i,positionRobotx,sizeX,positionRoboty,j);
            if(matriceAffichage[i + sizeX * posY] != ' '){
                chartype = matriceAffichage[i + sizeX * posY];
            }                
            if(chartype != ' '){
                matriceAffichage[i + sizeX * posY] = chartype;
            }
        }
    }

    for(int j = 0; j<sizeY; j++){
        char chartype = ' ';
        for(int i = positionRobotx; i>=0; i--){
            int posY = MAP(i,positionRobotx,0,positionRoboty,j);
            if(matriceAffichage[i + sizeX * posY] != ' '){
                chartype = matriceAffichage[i + sizeX * posY];
            }                
            if(chartype != ' '){
                matriceAffichage[i + sizeX * posY] = chartype;
            }
        }
    }
    
    //add border
    int posiyPos, posiyNeg;
    for(int i = 0; i<sizeX; i+=(sizeX/scale)+1){
        posix = i;
        posiyPos = 1600/scale + sizeY/2;
        posiyNeg = (-1600)/scale + sizeY/2;
        if(posix < sizeX && posix >= 0 && posiyPos < sizeY && posiyPos >= 0 && posiyNeg < sizeY && posiyNeg >= 0){
            matriceAffichage[posix + sizeX * posiyPos] = 'Z';
            matriceAffichage[posix + sizeX * posiyNeg] = 'Z';
        }
    }
    int posixPos, posixNeg;
    for(int i = 0; i<sizeY; i+=(sizeY/scale)+1){
        posixNeg = (-1000)/scale + sizeX/2;
        posixPos = 1000/scale + sizeX/2;
        posiy = i;
        if(posixPos < sizeX && posixPos >= 0 && posixNeg < sizeX && posixNeg >= 0  && posiyNeg < sizeY && posiyNeg >= 0){
            matriceAffichage[posixPos + sizeX * posiy] = 'Z';
            matriceAffichage[posixNeg + sizeX * posiy] = 'Z';
        }
    }

    posix = position.x/scale + sizeX/2;
    posiy = position.y/scale + sizeY/2;
    if(posix<sizeX && posix>=0 && posiy<sizeY && posiy>=0)
        matriceAffichage[posix + sizeX * posiy] = 'O';

    //print
    for(int x = sizeX-1; x>=0; x--){
        for(int y = sizeY-1; y>=0; y--){
            printf("%c%c",matriceAffichage[x + y * sizeX],matriceAffichage[x + y * sizeX]);
        }
        printf("\n");
    }

    free(matriceAffichage);

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

void position_facile(lidarAnalize_t* data,int count, double *X, double* Y, double X_prec, double Y_prec){
    

    double X1,X2,Y1,Y2,a;
    double sommeX1=0, sommeX2=0, sommeY1=0, sommeY2=0;
    int i_max=0;
    float angle_index;
    int l1,l2,L1,L2,l = 2000,L = 3000,nb = 0,m;
    double alpha1, alpha2;
    int offset= 0;

    for(int i = 0; i <count; i++){
        i_max ++;
    }

    angle_index = float(i_max)/4;
   
    for (offset = 0; offset< angle_index; offset += 1){

        //printf("\n %i dist  = %f / %f, objectif = %f", data[2*angle_index  + offset].angle,data[2*angle_index  + offset +1].angle, float(offset)*90/angle_index);
        
        l1 = data[int(2*angle_index)  + offset].dist; 
        l2 = data[int(0*angle_index)  + offset ].dist;
        L1 = data[int(3*angle_index)  + offset].dist; 
        L2 = data[int(1*angle_index)  + offset].dist; 

        if (l1+l2 > L1+L2){m = l1;l1 = L1;L1 = m;
            m = l2;l2 = L2;L2 = m;}

        alpha1 = double(l)/(l1+l2);
        alpha2 = double(L)/(L1+L2);
        //printf("\n\n alpha 1 = %f / alpha 2 = %f / diff = %f / l1 = %i / l2 = %i / L1 = %i / L2 = %i / ",acos(alpha1)*180/M_PI-offset*90/angle_index,acos(alpha2)*180/M_PI -offset*90/angle_index,fabs(alpha1 - alpha2), l1, l2, L1, L2);
            
        if (fabs(alpha1 - alpha2) < 0.005 && l1 != 0 && l2!= 0 && L1!=0 && L2!=0 && ( l - l1-l2 < 10) && (L-L1-L2 < 100)){

            //printf("\n alpha 1 = %f / alpha 2 = %f / diff = %f / l1 = %i / l2 = %i / L1 = %i / L2 = %i / ",acos(alpha1)*180/M_PI-offset*90/angle_index,acos(alpha2)*180/M_PI -offset*90/angle_index,fabs(alpha1 - alpha2), l1, l2, L1, L2);
            a = (alpha1+alpha2)/2;
            if (L1 < L2) {X1 = L1*a;X2 = L2*a;}
            else {X2 = L1*a;X1 = L2*a;}
            if (l1<l2) {Y1 = l1*a;Y2 = l2*a;}
            else {Y2 = l1*a;Y1 = l2*a;}
         
            
            sommeX1+=X1; sommeX2+=X2;
            sommeY1+=Y1; sommeY2+=Y2;
            
            nb++;
            //printf("\n x1 = %f /x2 = %f / y1 = %f / y2 = %f / diff = %f et %f / nb = %i",X1,X2,Y1,Y2, L- fabs(X1 + X2),l - fabs(Y1 + Y2),nb );
        }
        
    
    }
    X1 = sommeX1/nb;
    X2 = sommeX2/nb;
    Y1 = sommeY1/nb;
    Y2 = sommeY2/nb;
    printf("\n somme : x1 = %f /x2 = %f / y1 = %f / y2 = %f / diff = %f et %f \n",sommeX1/nb,sommeX2/nb,sommeY1/nb,sommeY2/nb, l - fabs(sommeY1/nb + sommeY2/nb), L- fabs(sommeX1/nb + sommeX2/nb));
    *X = X1; *Y = Y1;
}


double distance_2_pts(double d1,double deg1, double d2, double deg2){
    double alpha;
    double d3;
    alpha = fabs((deg1-deg2)*M_PI/180);
    d3 = sqrt(pow(d1,2) + pow(d2,2) -2*d1*d2*cos(alpha));
    return d3;
}

void sol_eq_2cercle(double xA,double  yA,double AM,double xB,double yB,double BM,double xC, double yC, double CM, int *xM, int *yM){
    double C,D,E,F,cst;

    //cercle A-C
    E = (yA-yC)/(xC-xA);
    F = (xC*xC + yC*yC + AM*AM - CM*CM - xA*xA - yA*yA)/(2*(xC-xA));
    //cercle A-B
    if (xB-xA == 0){
        cst = (xB*xB + yB*yB + AM*AM - BM*BM - xA*xA - yA*yA)/(2*(yB-yA));
        *yM = int(cst);
        *xM = int(cst*E + F);}
    else{
        C = (yA-yB)/(xB-xA);
        D = (xB*xB + yB*yB + AM*AM - BM*BM - xA*xA - yA*yA)/(2*(xB-xA)); 
        cst = (F-D)/(C-E);
        *yM = int(cst);
        *xM = int(cst*C + D);
    }
    //printf("\nxM = %f / yM = %f / dist = %f\n",xM,yM, sqrt(xM*xM + yM*yM));
}

void init_position_balise(lidarAnalize_t* data, int count, position_t *position){
    double d1,d2,deg1,deg2,deg3,distance,somme_angle=0,somme_dist = 0,nb =0;
    int next_valid,ligne =0,rows = 200;//nb de lignes
 
    // Allocation dynamique du tableau
    element_decord** array = new element_decord*[rows];
    for (int i = 0; i < rows; ++i) {
        array[i] = new element_decord; // Allocation de mémoire pour chaque élément
        array[i]->moy_angle = 0; array[i]->nb = 0;array[i]->moy_dist = 0;array[i]->nb = 0; array[i]->i = 0; //initialisation de tt
    }

    //fragmente le décord en plusieurs éléments proches
    for(int i = 0; i <count; i++){
        distance = data[i].dist;
        if(data[i].onTable){
            somme_angle += data[i].angle;
            somme_dist += distance; nb ++;

            next_valid = 1;
            while ((!data[i+next_valid].onTable) && ((i+next_valid) <count)) {next_valid++;}
            if (fabs(distance- data[i+next_valid].dist) > 50 || fabs(data[i].angle- data[i+next_valid].angle) > 1){   // changement d'élément de décord, séparation si écart > 5cm
                d1 = data[i].dist;d2 = somme_dist/nb;
                deg1 = data[i].angle; deg2 = somme_angle/nb;
                array[ligne]->cm = 2*distance_2_pts(d1, deg1, d2,deg2);
                array[ligne]->moy_angle = somme_angle/nb; array[ligne]->nb = nb;
                array[ligne]->moy_dist = somme_dist/nb; array[ligne]->i = i;
                somme_angle = 0; somme_dist = 0; nb = 0; ligne ++;
            }
        }
    }
    //supprime le fin du tableau
    while (array[rows-1]->moy_dist == 0){supprimerElement(array, rows, rows -1);}
    
    
    // suppression si nb < 3 ou cm < 3cm ou cm > 20cm
    for (int i= 0; i< rows; i++){
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
        printf("\n Rows = %i /Angle = %f /Dist = %f /n = %i /i = %i /mm = %f /x = %f/y = %f",l, array[l]->moy_angle,array[l]->moy_dist, array[l]->nb, array[l]->i, array[l]->cm, - array[l]->moy_dist*cos((90 +array[l]->moy_angle+ position->teta)*DEG_TO_RAD) + position->x,array[l]->moy_dist*sin((90 + array[l]->moy_angle+position->teta)*DEG_TO_RAD))+position->y; 
    }
    */

    // donne poto 1 et 2
    double poto_1_2, poto_2_3, poto_3_1, d_tot = 10000;
    int index_poto1, index_poto2, index_poto3; //poto 1 = gauche haut, poto 2 = gauche bas, poto 3 = droite
    poto_1_2 = 1900.0;poto_2_3 = 3300.0; poto_3_1 = 3300.0;
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
                        }
                    }
                }
            }
        }
        
        double d_1_2 = distance_2_pts(array[index_poto1]->moy_dist, array[index_poto1]->moy_angle, array[index_poto2]->moy_dist, array[index_poto2]->moy_angle);
        double d_2_3 = distance_2_pts(array[index_poto2]->moy_dist, array[index_poto2]->moy_angle, array[index_poto3]->moy_dist, array[index_poto3]->moy_angle);
        double d_3_1 = distance_2_pts(array[index_poto3]->moy_dist, array[index_poto3]->moy_angle, array[index_poto1]->moy_dist, array[index_poto1]->moy_angle);
        distance = fabs(d_1_2-poto_1_2) + fabs(d_2_3-poto_2_3) + fabs(d_3_1-poto_3_1);
        //printf("\n d_1_2 = %f / d_2_3 =  %f / d_3_1 = %f / distance = %f", d_1_2 , d_2_3 , d_3_1, distance);
        // vérification bon arrangement poteaux
        deg1 = array[index_poto1]->moy_angle;deg2 = array[index_poto2]->moy_angle;deg3 = array[index_poto3]->moy_angle;
        if ((deg1 < deg3 && deg3< deg2) or (deg3 < deg2 && deg2 < deg1)or (deg2<deg1&& deg1<deg3));
        else {int temp = index_poto1;index_poto1 = index_poto2;index_poto2 = temp;}
        //printf(" /  distance = ",distance);
        if (distance > 100) {return;}
        //determination centre 3 cercles
        //printf("\nD_2_M = %f / poto 1 = %i / poto 2 = %i / poto 3 = %i / d_1_2 = %f / d_2_3 = %f / d_3_1 = %f / distance = %f", array[index_poto2]->moy_dist, index_poto1, index_poto2, index_poto3, d_1_2, d_2_3, d_3_1,distance);
        sol_eq_2cercle(0,d_1_2,array[index_poto1]->moy_dist, 0,0, array[index_poto2]->moy_dist, sqrt(d_2_3*d_2_3 - d_1_2*d_1_2/4), d_1_2/2, array[index_poto3]->moy_dist, &position->y,&position->x);
        position->dist = array[index_poto2]->moy_dist;
        position->teta = 270 - atan(position->y/position->x)*180/M_PI - array[index_poto2]->moy_angle;
        if (position->teta < 0){ position->teta += 360;}
        position->y =- L/2 - 95 + position->y; position->x = -position->x + poto_1_2/2;
        //printf("\n x = %i / y = %i / erreur  = %f", position->x ,position->y, distance);
        /*
        //Index ennemie
        for (int i = 0; i < rows; i++){
            if (i!= index_poto1 && i!= index_poto2 &&i!= index_poto3){index_ennemie = i;}
        }

        if (index_ennemie == -1) {LOG_ERROR("INDEX ENNEMIE PAS TROUVE");return;}
        position_ennemie->dist = array[index_ennemie]->moy_dist; position_ennemie->teta = array[index_ennemie]->moy_angle;
        angle_ennemie = 90- position->teta - array[index_ennemie]->moy_angle;
        position_ennemie->y =array[index_ennemie]->moy_dist*cos(angle_ennemie*DEG_TO_RAD);
        position_ennemie->x = array[index_ennemie]->moy_dist*sin(angle_ennemie*DEG_TO_RAD);
        //printf("\n ennemie : x = %i / y = %i / erreur  = %f", position_ennemie->x ,position_ennemie->y, distance);
        */
    }
    // Libération de la mémoire
    for (int i = 0; i < rows; ++i) {
        delete array[i];}
    delete[] array;
}
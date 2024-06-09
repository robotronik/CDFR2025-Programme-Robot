
int desc_gradient(lidarAnalize_t* data, int count, position_t position){
    double X = 0, Y=0, teta = 0,gradX, gradY, gradTeta;
    int LEN = 128, TPS = 20000, LEARNING_RATE = 1.0, LEARNING_RATE_DECAY_STEP = 50, LEARNING_RATE_DECAY_FACTOR = 4.0, EPSILON = 1;


    
    for (int i = 0; i < TPS; i++){  
        gradX = (erreur(data,count, X+EPSILON,Y)-erreur(data,count, X,Y))/EPSILON;
        gradY = (erreur(data,count, X,Y+EPSILON)-erreur(data,count, X,Y))/EPSILON;
        //printf("\n ereur = %f / erreur y = %f", (erreur(data,count, X,Y)), erreur(data,count, X+EPSILON,Y));
        X-= gradX*LEARNING_RATE;
        Y-= gradY*LEARNING_RATE;
    }
    
    printf("\n X = %f / Y = %f / distance = %f",X,Y,erreur(data,count, X,Y));
    


}
void regression_lineaire(lidarAnalize_t* data,int count, double X, double Y){
    //double somme_xy = 0, somme_x = 0, somme_y = 0, somme_xx, a,b;
    int NB = 0, ligne = 0,  pos = 0, neg = 0, prev = 0;
    double somme_angle = 0, somme_dist = 0, nb=0, distance;
    int rows = 100; // Nombre de lignes
    int next_valid; //permet de trouver le prochain élément valide
    double d1,d2, deg1,deg2,deg3;
    

     // Allocation dynamique du tableau
    element_decord** array = new element_decord*[rows];
    for (int i = 0; i < rows; ++i) {
        array[i] = new element_decord; // Allocation de mémoire pour chaque élément
        array[i]->moy_angle = 0; // Initialisation de chaque élément
        array[i]->nb = 0;array[i]->moy_dist = 0;
        array[i]->nb = 0; array[i]->i = 0;  
    }

    //fragmente le décord en plusieurs éléments proches
    for(int i = 0; i <count; i++){
        distance = data[i].dist;

        //if (position->dist < 100) {convertAngularToAxial(data, count, position);}
        if(data[i].valid && distance < 4000){
            somme_angle += data[i].angle;
            somme_dist += distance; nb ++;
            
            next_valid = 1;
            while ((!data[i+next_valid].valid) && ((i+next_valid) <count)) {next_valid++;}
            
            double ecart = data[i].dist - data[i+next_valid].dist;
            if (ecart >=0 && prev >= 0) {pos += 1;prev = 1;}
            if (ecart <=0 && prev <= 0) {neg += 1;prev = -1;}
            if (ecart >=0 && prev < 0) {pos =1;prev = 1;}
            if (ecart <=0 && prev > 0) {neg = 1;prev = -1;}
            
            if (fabs(ecart) > 200 || (pos >= 3 && neg >=3)){   // changement d'élément de décord, séparation si écart > 20cm
                if (pos >= 3 && neg >=3) {printf("\n angle coin = %f", data[i].angle);}
                pos = 0,neg = 0;
                d1 = data[i].dist;
                d2 = somme_dist/nb;
                deg1 = data[i].angle;
                deg2 = somme_angle/nb;
                array[ligne]->cm = 2*distance_2_pts(d1, deg1, d2,deg2);
                array[ligne]->moy_angle = somme_angle/nb; array[ligne]->nb = nb;
                array[ligne]->moy_dist = somme_dist/nb; array[ligne]->i = i;
                somme_angle = 0; somme_dist = 0; nb = 0; ligne ++;
            }
        }
    }
    //supprime le fin du tableau
    while (array[rows-1]->moy_dist == 0){
        supprimerElement(array, rows, rows -1);}

    // Affichage pour vérifier la valeur
    for (int l = 0; l < rows; ++l) {
        printf("\n Rows = %i / Angle = %f / Dist = %f / n = %i / i = %i / mm = %f ",l, array[l]->moy_angle,array[l]->moy_dist, array[l]->nb, array[l]->i, array[l]->cm); 
    }
    /*
    for (int i = 0; i < count; i++){
        if (data[i].valid && data[i].angle < 180 && data[i].angle > 90){
            somme_xy += data[i].x*data[i].y;
            somme_x += data[i].x;
            somme_y += data[i].y;
            somme_xx += data[i].x*data[i].x;
            NB++;
        }}
    b = (NB*somme_xy - somme_x*somme_y)/(NB*somme_xx - somme_x*somme_x);
    a = somme_y/NB -b*somme_x/NB;
    printf("\na = %f / b = %f", a,b);
    */
}


double erreur(lidarAnalize_t* data,int count, double X, double Y){
    int nb=0;
    double l = 2000, L = 3000;
    double alpha, beta ;
    double distance=0;
    alpha = atan((L-2*X)/(l-2*Y))*180/M_PI;
    beta = atan((l-2*Y)/(L-2*X))*180/M_PI;

    for (int i = 0; i < count; i++){
        if (data[i].valid && (data[i].angle < alpha || data[i].angle > 360 - alpha)){
            distance += pow(data[i].y - (l/2 -Y),2)/1000;
            nb ++;}
        if (data[i].valid && (data[i].angle < alpha + 2*beta || data[i].angle > alpha)){
            distance += pow(data[i].x - (L/2 - X),2)/1000;
            nb ++;}
        if (data[i].valid && (data[i].angle < 3*alpha + 2*beta || data[i].angle > alpha + 2*beta)){
            distance += pow(data[i].y - (l/2 +Y),2)/1000;
            nb ++;}
        if (data[i].valid && (data[i].angle < 360 - alpha || data[i].angle > 3*alpha + 2*beta)){
            distance += pow(data[i].x - (L/2 + X),2)/1000;
            nb ++;}
    }
    distance = distance /nb;
    printf("\ndistance = %f", distance);
    return distance;
}


void init_position_sol(lidarAnalize_t* data, int count, position_t *position){
    double distance;
    double d1,d2, deg1,deg2,deg3;
    int rows = 100; // Nombre de lignes
    int next_valid; //permet de trouver le prochain élément valide
    int ligne =0;
    double somme_angle = 0, somme_dist = 0, nb=0;
    
    // Allocation dynamique du tableau
    element_decord** array = new element_decord*[rows];
    for (int i = 0; i < rows; ++i) {
        array[i] = new element_decord; // Allocation de mémoire pour chaque élément
        array[i]->moy_angle = 0; // Initialisation de chaque élément
        array[i]->nb = 0;array[i]->moy_dist = 0;
        array[i]->nb = 0; array[i]->i = 0;  
    }

    //fragmente le décord en plusieurs éléments proches
    
    for(int i = 0; i <count; i++){
        //reboucle à l'angle 0 pour pas couper les balises
        distance = data[i].dist;

        //if (position->dist < 100) {convertAngularToAxial(data, count, position);}
        if(data[i].valid && distance < 4000){
            somme_dist += distance; nb ++;
            somme_angle += data[i].angle;

            next_valid = 1;

            while ((!data[i+next_valid].valid) && ((i+next_valid) <count)) {next_valid++;}
            printf("\n distance = %f / angle = %f", data[i].dist - data[i + next_valid].dist, data[i].angle);
            if (fabs(distance- data[i+next_valid].dist) > 50){   // changement d'élément de décord, séparation si écart > 20cm
                d1 = data[i].dist;
                d2 = somme_dist/nb;
                deg1 = data[i].angle;
                deg2 = somme_angle/nb;
                array[ligne]->cm = 2*distance_2_pts(d1, deg1, d2,deg2);
                array[ligne]->moy_angle = somme_angle/nb; array[ligne]->nb = nb;
                array[ligne]->moy_dist = somme_dist/nb; array[ligne]->i = i;
                somme_angle = 0; somme_dist = 0; nb = 0; ligne ++;
            }
        }
    }

    //supprime le fin du tableau
    while (array[rows-1]->moy_dist == 0){
        supprimerElement(array, rows, rows -1);}
    
    // suppression si nb <  et largeur < 20cm b
    for (int i= 0; i< rows; i++){
        
        while (array[rows -1    - i]->nb < 5 || array[rows -1 - i]->cm <200){
            supprimerElement(array, rows, rows -1 -i); 
            if (rows -1 -i < 0){break;}
        }}


    // Affichage pour vérifier la valeur
    for (int l = 0; l < rows; ++l) {
        printf("\n Rows = %i / Angle = %f / Dist = %f / n = %i / i = %i / mm = %f ",l, array[l]->moy_angle,array[l]->moy_dist, array[l]->nb, array[l]->i, array[l]->cm); 
    }
}

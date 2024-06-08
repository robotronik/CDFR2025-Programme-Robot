
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
#include "fonction.h"

int initPositon2(tableState* itable, Asser* iAsser,int x, int y,int teta){
    LOG_SCOPE("initPositon2");
    int ireturn = 0;
    static bool initStat = true;
    static fsminitPos_t currentState = SETPOS_INIT;
    fsminitPos_t nextState = currentState;
    int xSave,ySave,tetaSave;
    static unsigned long startTime;
    //static int step = -1;

    int TetaStart = 90;
    int TetaSecond = -180;
    int xSecond = 300;
    int xStart = 1000 - ROBOT_Y_OFFSET;
    int yStart = 1500 - ROBOT_Y_OFFSET;
    int yBack = 400;
    if(y<0){
        TetaStart = -90;
    }
     if(y<0){
        yBack = -yBack;
    }
    if(y<0){
        yStart = -yStart;
    }
    if(x<0){
        TetaSecond = 0;
        xSecond = -xSecond;
        xStart = -xStart;
    }

    
    switch (currentState)
    {
    case SETPOS_INIT :
        if(initStat) LOG_STATE("SETPOS_INIT");
        iAsser->getCoords(xSave,ySave,tetaSave);
        iAsser->setLinearMaxSpeed(200);
        iAsser->setMaxTorque(20);
        startTime = millis()+100;
        nextState = SETPOS_FIRSTFORWARD;
        break;

    case SETPOS_WAITINIT :
        if(initStat) LOG_STATE("SETPOS_WAITINIT");
        if(startTime < millis()){
            nextState = SETPOS_WAITINIT;
        }
        break;

    case SETPOS_FIRSTFORWARD :
        if(initStat) LOG_STATE("SETPOS_FIRSTFORWARD");
        if(deplacementLinearPoint(itable->robot.collide,iAsser,xSave,ySave+yBack)>0){
            nextState = SETPOS_FIRSTBACKWARD;
            iAsser->setCoords(xSave,yStart,TetaStart);
        }
        break;

    case SETPOS_FIRSTBACKWARD :
        if(initStat) LOG_STATE("SETPOS_FIRSTBACKWARD");    
        if(deplacementgoToPoint(itable->robot.collide,iAsser,xSave,y,TetaSecond)>0){
            nextState = SETPOS_SECONDBACKWARD;
        }
        break;

    case SETPOS_SECONDBACKWARD :
        if(initStat) LOG_STATE("SETPOS_SECONDBACKWARD");
        if(deplacementLinearPoint(itable->robot.collide,iAsser,xSave+xSecond,y)>0){
            iAsser->setCoords(xStart, y,TetaSecond);
            nextState = SETPOS_SECONDFORWARD;
        }
        break;

    case SETPOS_SECONDFORWARD :
        if(initStat) LOG_STATE("SETPOS_SECONDFORWARD");
        if(deplacementgoToPoint(itable->robot.collide,iAsser,x,y,teta)>0){
            nextState = SETPOS_INIT;
            iAsser->setLinearMaxSpeed(10000);
            iAsser->setMaxTorque(100);
            ireturn = 1;
        }
        break;
        
    default:
        if(initStat) LOG_STATE("default");
        nextState = SETPOS_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;
}


int turnSolarPannel(tableState* itable, Asser* iAsser,Arduino* arduino){
    LOG_SCOPE("SolarPanel");
    static fsmSolarPanel_t currentState = SOLARPANEL_INIT;
    fsmSolarPanel_t nextState = currentState;
    static bool initStat = true;
    int ireturn = 0;
    int deplacementreturn;
    const int axeX = 800;
    static int solarPanelNumber;
    int offsetRobot1;
    int offsetRobot2;
    const int table[9] = {1225,1000,775,225,0,-225,-775,-1000,-1225};

    if(itable->robot.colorTeam == YELLOW){
        offsetRobot1 = 10;
        offsetRobot2 = 35;
    }
    else{
        offsetRobot1 = -70;
        offsetRobot2 = -105;
    }
    
    switch (currentState)
    {
    case SOLARPANEL_INIT :
        if(initStat) LOG_STATE("SOLARPANEL_INIT");
        nextState = SOLARPANEL_SETHOME;
        if(itable->robot.colorTeam == YELLOW){
            solarPanelNumber = 0;
        }
        else{
            solarPanelNumber = 8;
        }
        break;

    case SOLARPANEL_SETHOME :
        if(initStat) LOG_STATE("SOLARPANEL_SETHOME");
        if(itable->robot.colorTeam == YELLOW){
            if(initPositon2(itable, iAsser,800,1250,-90)){
                nextState = SOLARPANEL_FORWARD;
            }
        }
        else{
            if(initPositon2(itable, iAsser,800,-1250,-90)){
                nextState = SOLARPANEL_FORWARD;
            }
        }
        break;


    case SOLARPANEL_FORWARD :
        if(initStat) LOG_STATE("SOLARPANEL_FORWARD");
        deplacementreturn = deplacementLinearPoint(itable->robot.collide,iAsser,axeX,table[solarPanelNumber]-offsetRobot1);
        itable->panneauSolaireRotate[solarPanelNumber].etat = true;
        if (itable->startTime+90000+20000 < millis()){
            nextState = SOLARPANEL_END;
                }
        if(deplacementreturn>0){
            nextState = SOLARPANEL_PUSHFOR; 
        }
        else if(deplacementreturn<0){
            ireturn = -1;
            nextState = SOLARPANEL_INIT;
        }
        break;

    case SOLARPANEL_PUSHFOR :
        if(initStat) LOG_STATE("SOLARPANEL_PUSHFOR");
        if(pullpush(arduino)){
            if(itable->robot.colorTeam == YELLOW){
                itable->panneauSolaireRotate[solarPanelNumber].color = YELLOW;
                solarPanelNumber++;
                if(solarPanelNumber==6){
                    nextState = SOLARPANEL_END;
                }
                else if(solarPanelNumber<3 || !itable->panneauSolaireRotate[solarPanelNumber].etat){
                    nextState = SOLARPANEL_FORWARD;
                }
                else{
                    nextState = SOLARPANEL_BACKWARD;
                }
            }
            else{
                itable->panneauSolaireRotate[solarPanelNumber].color = BLUE;
                solarPanelNumber--;
                if(solarPanelNumber==2){
                    nextState = SOLARPANEL_END;
                }
                else if(solarPanelNumber>5 || !itable->panneauSolaireRotate[solarPanelNumber].etat){
                    nextState = SOLARPANEL_FORWARD;
                }
                else{
                    nextState = SOLARPANEL_BACKWARD;
                }
            }
        }
        break;

    case SOLARPANEL_BACKWARD :
        if(initStat) LOG_STATE("SOLARPANEL_BACKWARD");
        deplacementreturn = deplacementLinearPoint(itable->robot.collide,iAsser,axeX,table[solarPanelNumber]-offsetRobot2);
        if(deplacementreturn>0){
            nextState = SOLARPANEL_PUSHBACK;
        }
        else if(deplacementreturn<0){
            ireturn = -1;
            nextState = SOLARPANEL_INIT;
        }
        break;

    case SOLARPANEL_PUSHBACK :
        if(initStat) LOG_STATE("SOLARPANEL_PUSHBACK");
        if(pullpush(arduino)){
            if(itable->robot.colorTeam == YELLOW){
                nextState = SOLARPANEL_FORWARD;
            }
            else{
                nextState = SOLARPANEL_FORWARD;
            }
        }
        break;

    case SOLARPANEL_END :
        if(initStat) LOG_STATE("SOLARPANEL_END");
        //nextState = SOLARPANEL_INIT;
        ireturn = 1;
        break;
    
    default:
        if(initStat) LOG_ERROR("SOLARPANEL_DEFAULT");
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}

int takePlant(Asser* iAsser,Arduino* arduino,tableState*itable,int yPos,int xStart, int xEnd, int numPlante){
    LOG_SCOPE("take plant");
    int ireturn = 0;
    static bool initStat = true;
    static fsmtakePlant_t currentState = TAKEPLANT_INIT;
    fsmtakePlant_t nextState = currentState;
    int deplacementreturn;
    static int positionToGo = 0;
    int x,y,teta;
    static bool servoChange;
    static bool servoChange2;


    switch (currentState)
    {
    case TAKEPLANT_INIT :
        if(initStat) LOG_STATE("TAKEPLANT_INIT");
        nextState = TAKEPLANT_FORWARD;
        positionToGo = plantPosition[numPlante].x+100;
        arduino->servoPosition(4,100);
        iAsser->setLinearMaxSpeed(200);
        servoChange = false;
        servoChange2 = false;
        break;
    case TAKEPLANT_FORWARD :
        if(initStat) LOG_STATE("TAKEPLANT_FORWARD");
        deplacementreturn = deplacementLinearPoint(itable->robot.collide,iAsser,positionToGo,yPos);
        iAsser->getCoords(x,y,teta);
        if(x>positionToGo-200 && servoChange == false){
            servoChange = true;
            arduino->servoPosition(4,170);
        }
        if(x>positionToGo-50 && servoChange2 == false){
            servoChange2 = true;
            arduino->servoPosition(4,100);
        }
        if(deplacementreturn>=1){
            nextState = TAKEPLANT_BACKWARD;
        }
        else if(deplacementreturn<=-1){
            nextState = TAKEPLANT_INIT;
            ireturn = -1;
            iAsser->setLinearMaxSpeed(10000);
        }
        break;
    case TAKEPLANT_REFORWARD : 
        if(initStat){ LOG_STATE("TAKEPLANT_REFORWARD");
            positionToGo += 400;
        }
        deplacementreturn = deplacementLinearPoint(itable->robot.collide,iAsser,positionToGo,yPos);
        if(deplacementreturn>=1){
            nextState = TAKEPLANT_BACKWARD;
        }
        else if(deplacementreturn<=-1){
            nextState = TAKEPLANT_INIT;
            ireturn = -1;
            iAsser->setLinearMaxSpeed(10000);
        }
        break;
    case TAKEPLANT_BACKWARD :
        if(initStat){ LOG_STATE("TAKEPLANT_BACKWARD");
            positionToGo -= 115;
        }
        deplacementreturn = deplacementLinearPoint(itable->robot.collide,iAsser,positionToGo,yPos);
        if(deplacementreturn>=1){
            nextState = TAKEPLANT_TAKE;
        }
        else if(deplacementreturn<=-1){
            nextState = TAKEPLANT_REFORWARD;
        }
        break;
    case TAKEPLANT_TAKE :
        if(initStat) LOG_STATE("TAKEPLANT_TAKE");
        deplacementreturn = catchPlant(arduino);
        if(deplacementreturn){
            nextState = TAKEPLANT_END;
        }
        break;
    case TAKEPLANT_END :
        if(initStat) LOG_STATE("TAKEPLANT_END");
        nextState = TAKEPLANT_INIT;
        iAsser->setLinearMaxSpeed(10000);
        ireturn = 1;
        break;
    
    default:
        if(initStat) LOG_STATE("default");
        nextState = TAKEPLANT_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}

int takePlant2( Asser* iAsser,Arduino* arduino,tableState*itable,int xStart,int yStart, int xEnd, int yEnd){
    LOG_SCOPE("take plant");
    int ireturn = 0;
    static bool initStat = true;
    static fsmtakePlant_t currentState = TAKEPLANT_INIT;
    fsmtakePlant_t nextState = currentState;
    int deplacementreturn;
    int xtogo = xEnd - (115/sqrt((xEnd-xStart)*(xEnd-xStart)+(yEnd-yStart)*(yEnd-yStart)))*(xEnd-xStart);
    int ytogo = yEnd - (115/sqrt((xEnd-xStart)*(xEnd-xStart)+(yEnd-yStart)*(yEnd-yStart)))*(yEnd-yStart);

    switch (currentState)
    {
    case TAKEPLANT_INIT :
        if(initStat) LOG_STATE("TAKEPLANT_INIT");
        nextState = TAKEPLANT_FORWARD;
        arduino->servoPosition(4,100);
        iAsser->setLinearMaxSpeed(200);
        break;
    case TAKEPLANT_FORWARD :
        if(initStat) LOG_STATE("TAKEPLANT_FORWARD");
        deplacementreturn = deplacementgoToPointNoTurn(itable->robot.collide,iAsser,xEnd,yEnd);
        if(deplacementreturn>=1){
            nextState = TAKEPLANT_BACKWARD;
        }
        else if(deplacementreturn<=-1){
            nextState = TAKEPLANT_INIT;
            ireturn = -1;
            iAsser->setLinearMaxSpeed(10000);
        }
        break;
    case TAKEPLANT_BACKWARD :
        if(initStat){ LOG_STATE("TAKEPLANT_BACKWARD");
        }
        deplacementreturn = deplacementgoToPointNoTurn(itable->robot.collide,iAsser,xtogo,ytogo,MOVE_BACKWARD);
        if(deplacementreturn>=1){
            nextState = TAKEPLANT_TAKE;
        }
        else if(deplacementreturn<=-1){
            nextState = TAKEPLANT_INIT;
        }
        break;
    case TAKEPLANT_TAKE :
        if(initStat) LOG_STATE("TAKEPLANT_TAKE");
        deplacementreturn = catchPlant(arduino);
        if(deplacementreturn){
            nextState = TAKEPLANT_END;
        }
        break;
    case TAKEPLANT_END :
        if(initStat) LOG_STATE("TAKEPLANT_END");
        nextState = TAKEPLANT_INIT;
        iAsser->setLinearMaxSpeed(10000);
        ireturn = 1;
        break;
    
    default:
        if(initStat) LOG_STATE("default");
        nextState = TAKEPLANT_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}

int jardinierePutPlant(tableState* itable, Asser* iAsser,Arduino* arduino,int x,int y,int teta){
    LOG_SCOPE("putPlant");
    int ireturn = 0;
    static bool initStat = true;
    static fsmTPutPlant_t currentState = PUTPLANT_INIT;
    fsmTPutPlant_t nextState = currentState;
    int deplacementreturn;


    switch (currentState)
    {
    //****************************************************************
    case PUTPLANT_INIT :
        if(initStat) LOG_STATE("PUTPLANT_INIT");
        nextState = PUTPLANT_GOBORDER;
        break;
    //****************************************************************
    case PUTPLANT_GOBORDER :
        if(initStat) LOG_STATE("PUTPLANT_GOBORDER");
        deplacementreturn = deplacementgoToPoint(itable->robot.collide,iAsser,x,y,teta,MOVE_FORWARD,ROTATION_DIRECT);
        if(deplacementreturn > 0){
            iAsser->stop();
            iAsser->brakeMotor(true);
            nextState = PUTPLANT_PLACE;
        }
        else if(deplacementreturn < 0){
            nextState = PUTPLANT_INIT;
            ireturn = -1;
        }        
        break;
    //****************************************************************
    case PUTPLANT_PLACE :
        if(initStat) LOG_STATE("PUTPLANT_PLACE");
        if(releasePlant(arduino)){
            iAsser->stop();
            iAsser->brakeMotor(false);
            iAsser->enableMotor(true);
            nextState = PUTPLANT_INIT;
            ireturn = 1;
        }
        break;
    //****************************************************************    
    default:
        if(initStat) LOG_ERROR("default");
        nextState = PUTPLANT_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;
}



bool allJardiniereFull(tableState* itable){
    if(itable->robot.colorTeam == YELLOW){
        return  itable->JardiniereFull[3].etat && itable->JardiniereFull[4].etat && itable->JardiniereFull[0].etat;
    }
    else{
        return itable->JardiniereFull[1].etat && itable->JardiniereFull[2].etat && itable->JardiniereFull[5].etat;
    }
}

void resetActionneur(Asser* iAsser, Arduino* arduino){
    arduino->servoPosition(1,180);
    arduino->servoPosition(2,CLAMPSLEEP);
    arduino->moveStepper(ELEVATORUP,1);
    iAsser->setLinearMaxSpeed(10000);
}









int returnToHome(tableState* itable,Asser* iAsser){
    static int step = 0;
    bool breturn = false;
    if(step == 0){
        iAsser->setLookForward(700,itable->robot.colorTeam == YELLOW ? 1200 : -1200,0);
        step++;   
    }
    else if(step == 1 && !iAsser->getError(ANGULAR_ERROR)){
        iAsser->linearSetpoint(700,itable->robot.colorTeam == YELLOW ? 1200 : -1200);
        step++;
    }
    else if(step == 2){
        if(!iAsser->getError(LINEAR_ERROR)){
            step++;
        }        
    }
    else if(step == 3){
        breturn = true;
    }

    return breturn; 
}

int FSMMatch(tableState* itable, Asser* iAsser,Arduino* arduino){
    int  bFinMatch = turnSolarPannel(itable,iAsser, arduino);
    if(bFinMatch == 1){
        printf("FIN turnSolarPannel\n");
    }
    // fsmMatch_t nextStateMatch = currentStateMatch;
    // switch (currentStateMatch)
    // {
    // case SOLARPANNEL:{
    //     bool bret = turnSolarPannel(iAsser, arduino,collideB);
    //     if(bret){
    //         nextStateMatch = RETURNHOME;
    //     }
    //     break;
    // }
    // case RETURNHOME:{
    //     int bFinMatch = returnToHome(iAsser,collideF);
    //     break;
    // }       
    
    // default:
    //     break;
    // }

    return bFinMatch;
}

int TestPinceFSM(tableState* itable, Asser* iAsser,Arduino* arduino){
    LOG_SCOPE("TestPince");
    int ireturn = 0;
    static bool initStat = true;
    static fsmTestPince_t currentState = TESTPINCE_INIT;
    fsmTestPince_t nextState = currentState;
    int deplacementreturn;


    switch (currentState)
    {
    //****************************************************************
    case TESTPINCE_INIT :
        if(initStat) LOG_STATE("TESTPINCE_INIT");
        nextState = TESTPINCE_GOPLANT;
        break;
    //****************************************************************
    case TESTPINCE_GOPLANT :
        if(initStat) LOG_STATE("TESTPINCE_GOPLANT");
        deplacementreturn = deplacementgoToPoint(itable->robot.collide,iAsser,-700,-505,0,MOVE_FORWARD,ROTATION_DIRECT);
        if(deplacementreturn>0){
            nextState = TESTPINCE_TAKEPLANT;
        }
        else if(deplacementreturn<0){
            return deplacementreturn;
            nextState = TESTPINCE_INIT;
        }
        break;
    //****************************************************************        
    case TESTPINCE_TAKEPLANT :
        if(initStat) LOG_STATE("TESTPINCE_TAKEPLANT");
        //deplacementreturn = takePlant(mainRobot,iAsser,arduino,-505,-700,0,2);
        if(deplacementreturn>0){
            nextState = TESTPINCE_GOCORNE;
        }
        else if(deplacementreturn<0){
            return deplacementreturn;
            nextState = TESTPINCE_INIT;
        }
        break;
    //****************************************************************        
    case TESTPINCE_GOCORNE :
        if(initStat) LOG_STATE("TESTPINCE_GOCORNE");
        deplacementreturn = deplacementgoToPoint(itable->robot.collide,iAsser,-700,-762,180,MOVE_FORWARD,ROTATION_DIRECT);
        if(deplacementreturn>0){
            nextState = TESTPINCE_GOJARDINIER;
        }
        else if(deplacementreturn<0){
            return deplacementreturn;
            nextState = TESTPINCE_INIT;
        }
        break;
    //****************************************************************
    case TESTPINCE_GOJARDINIER :
        if(initStat) LOG_STATE("TESTPINCE_GOJARDINIER");
        deplacementreturn = deplacementgoToPoint(itable->robot.collide,iAsser,-870,-762,-180,MOVE_FORWARD,ROTATION_DIRECT);
        if(deplacementreturn>0){
            nextState = TESTPINCE_PLACE;
        }
        else if(deplacementreturn<0){
            return deplacementreturn;
            nextState = TESTPINCE_INIT;
        }
        break;
    //****************************************************************
    case TESTPINCE_PLACE :
        if(initStat) LOG_STATE("TESTPINCE_PLACE");
        if(releasePlant(arduino)){
            iAsser->stop();
            nextState = TESTPINCE_GOBACKWARD;
        }
        break;
    //****************************************************************
    case TESTPINCE_GOBACKWARD :
        if(initStat) LOG_STATE("TESTPINCE_GOBACKWARD");
        deplacementreturn = deplacementgoToPoint(itable->robot.collide,iAsser,-700,-762,-180,MOVE_BACKWARD,ROTATION_DIRECT);
        if(deplacementreturn>0){
            nextState = TESTPINCE_GOHOME;
        }
        else if(deplacementreturn<0){
            return deplacementreturn;
            nextState = TESTPINCE_INIT;
        }
        break;
    //****************************************************************
    case TESTPINCE_GOHOME :
        if(initStat) LOG_STATE("TESTPINCE_GOHOME");
        deplacementreturn = deplacementgoToPoint(itable->robot.collide,iAsser,-800,-1250,-90,MOVE_FORWARD,ROTATION_DIRECT);
        if(deplacementreturn>0){
            nextState = TESTPINCE_INIT;
            ireturn = 1;
        }
        else if(deplacementreturn<0){
            return deplacementreturn;
            nextState = TESTPINCE_INIT;
        }
        break;
    //****************************************************************    
    default:
        if(initStat) LOG_ERROR("default");
        nextState = TESTPINCE_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;
}

void ennemieInAction(tableState* itable, int x_ennemie, int y_ennemie ){
    //itable->jardiniereFree;


}
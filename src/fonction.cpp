#include "fonction.h"


// Fonction pour mettre à jour l'état de la FSM en fonction de l'entrée
int initPositon(Asser* robot,int x, int y,int teta){
    LOG_SCOPE("initPos");
    static unsigned long startTime;
    static int step = -1;

    int TetaStart = 90;
    int TetaSecond = -180;
    int xSecond = 300;
    int xStart = 1000 - ROBOT_Y_OFFSET;
    int yStart = 1500 - ROBOT_Y_OFFSET;
    if(y<0){
        TetaStart = -90;
    }
    if(y<0){
        yStart = -yStart;
    }
    if(x<0){
        TetaSecond = 0;
        xSecond = -xSecond;
        xStart = -xStart;
    }

    if(step == -1){
        robot->setCoords(0,0,0);
        LOG_STATE("SETP -1 ");
        step++;
        robot->setLinearMaxSpeed(200);
        robot->setMaxTorque(20);
        startTime = millis()+100;
    }
    else if(step == 0 && startTime < millis()){
        robot->linearSetpoint(-400,0);
        LOG_STATE("SETP 0 ");
        step++;
        startTime = millis()+3000;
    }
    else if(step == 1 && startTime < millis()){
        robot->setCoords(0,yStart,TetaStart);
        robot->linearSetpoint(0,y);
        LOG_STATE("SETP 1 ");
        step++;
    }
    else if(step == 2 && !robot->getError(LINEAR_ERROR)){
        robot->angularSetpoint(TetaSecond,0);
        LOG_STATE("SETP 2 ");
        step++;
    }
    else if(step == 3 && !robot->getError(ANGULAR_ERROR)){
        robot->linearSetpoint(xSecond,y);
        LOG_STATE("SETP 3 ");
        startTime = millis()+3000;
        step++;
    }
    else if(step == 4 && startTime < millis()){
        robot->setCoords(xStart, y,TetaSecond);
        robot->linearSetpoint(x , y);
        LOG_STATE("SETP 4 ");
        step++;
    }
    else if(step == 5 && !robot->getError(LINEAR_ERROR)){
        robot->angularSetpoint(teta,0);
        LOG_STATE("SETP 5 ");
        step++;
    }
    else if(step == 6 && !robot->getError(ANGULAR_ERROR)){
        LOG_STATE("SETP 6 ");
        step++;
    }
    if(step>6){
        step = -1;
        robot->setLinearMaxSpeed(10000);
        robot->setMaxTorque(100);
    }
    return step == -1;
}

int initPositonY(robotCDFR mainRobot, Asser* robot,int x, int y,int teta){
    LOG_SCOPE("initPositonY");
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
        robot->getCoords(xSave,ySave,tetaSave);
        robot->setLinearMaxSpeed(200);
        robot->setMaxTorque(20);
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
        if(deplacementLinearPoint(mainRobot,robot,xSave,ySave+yBack)>0){
            nextState = SETPOS_FIRSTBACKWARD;
            robot->setCoords(xSave,yStart,TetaStart);
        }
        break;

    case SETPOS_FIRSTBACKWARD :
        if(initStat) LOG_STATE("SETPOS_FIRSTBACKWARD");    
        if(deplacementgoToPoint(mainRobot,robot,xSave,y,teta)>0){
            nextState = SETPOS_INIT;
            robot->setLinearMaxSpeed(10000);
            robot->setMaxTorque(100);
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

int initPositon2(robotCDFR mainRobot, Asser* robot,int x, int y,int teta){
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
        robot->getCoords(xSave,ySave,tetaSave);
        robot->setLinearMaxSpeed(200);
        robot->setMaxTorque(20);
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
        if(deplacementLinearPoint(mainRobot,robot,xSave,ySave+yBack)>0){
            nextState = SETPOS_FIRSTBACKWARD;
            robot->setCoords(xSave,yStart,TetaStart);
        }
        break;

    case SETPOS_FIRSTBACKWARD :
        if(initStat) LOG_STATE("SETPOS_FIRSTBACKWARD");    
        if(deplacementgoToPoint(mainRobot,robot,xSave,y,TetaSecond)>0){
            nextState = SETPOS_SECONDBACKWARD;
        }
        break;

    case SETPOS_SECONDBACKWARD :
        if(initStat) LOG_STATE("SETPOS_SECONDBACKWARD");
        if(deplacementLinearPoint(mainRobot,robot,xSave+xSecond,y)>0){
            robot->setCoords(xStart, y,TetaSecond);
            nextState = SETPOS_SECONDFORWARD;
        }
        break;

    case SETPOS_SECONDFORWARD :
        if(initStat) LOG_STATE("SETPOS_SECONDFORWARD");
        if(deplacementgoToPoint(mainRobot,robot,x,y,teta)>0){
            nextState = SETPOS_INIT;
            robot->setLinearMaxSpeed(10000);
            robot->setMaxTorque(100);
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

int initY(robotCDFR mainRobot, Asser* robot,int x, int y,int teta){
    LOG_SCOPE("initY");
    int ireturn = 0;
    static bool initStat = true;
    static fsminitY_t currentState = INTIY_INIT;
    fsminitY_t nextState = currentState;
    int xSave,ySave,tetaSave;
    int deplacementy = 400;
    int TetaStart = 90;
    int yStart = 1500 - ROBOT_Y_OFFSET;
    if(y<0){
        TetaStart = -90;
    }
    if(y<0){
        yStart = -yStart;
    }
    if(y<0){
        deplacementy = -deplacementy;
    }
    
    switch (currentState)
    {
    case INTIY_INIT :
        if(initStat) LOG_STATE("INTIY_INIT");
        nextState = INTIY_BACKWARD;
        robot->getCoords(xSave,ySave,tetaSave);
        robot->linearSetpoint(xSave , ySave - deplacementy);
        robot->setLinearMaxSpeed(200);
        robot->setMaxTorque(20);
        break;
    case INTIY_BACKWARD :
        if(initStat) LOG_STATE("INTIY_BACKWARD");    
        if(deplacementLinearPoint(mainRobot,robot,xSave, ySave - deplacementy)>0){
            robot->setCoords(xSave,ySave,TetaStart);
            robot->setLinearMaxSpeed(10000);
            robot->setMaxTorque(100);
            ireturn = 1;
            nextState = INTIY_INIT;
        }
        break;
    default:
        if(initStat) LOG_STATE("default");
        nextState = INTIY_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;

}



int turnSolarPannel(robotCDFR mainRobot, Asser* robot,Arduino* arduino){
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

    if(mainRobot.tableStatus.colorTeam == YELLOW){
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
        if(mainRobot.tableStatus.colorTeam == YELLOW){
            solarPanelNumber = 0;
        }
        else{
            solarPanelNumber = 8;
        }
        break;

    case SOLARPANEL_SETHOME :
        if(initStat) LOG_STATE("SOLARPANEL_SETHOME");
        if(mainRobot.tableStatus.colorTeam == YELLOW){
            if(initPositon2(mainRobot, robot,800,1250,-90)){
                nextState = SOLARPANEL_FORWARD;
            }
        }
        else{
            if(initPositon2(mainRobot, robot,800,-1250,-90)){
                nextState = SOLARPANEL_FORWARD;
            }
        }
        break;


    case SOLARPANEL_FORWARD :
        if(initStat) LOG_STATE("SOLARPANEL_FORWARD");
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,axeX,table[solarPanelNumber]-offsetRobot1);
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
            if(mainRobot.tableStatus.colorTeam == YELLOW){
                mainRobot.tableStatus.panneauSolaireRotate[solarPanelNumber] = YELLOW;
                solarPanelNumber++;
                if(solarPanelNumber==6){
                    nextState = SOLARPANEL_END;
                }
                else if(solarPanelNumber<3){
                    nextState = SOLARPANEL_FORWARD;
                }
                else{
                    nextState = SOLARPANEL_BACKWARD;
                }
            }
            else{
                mainRobot.tableStatus.panneauSolaireRotate[solarPanelNumber] = BLUE;
                solarPanelNumber--;
                if(solarPanelNumber==2){
                    nextState = SOLARPANEL_END;
                }
                else if(solarPanelNumber>5){
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
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,axeX,table[solarPanelNumber]-offsetRobot2);
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
            if(mainRobot.tableStatus.colorTeam == YELLOW){
                nextState = SOLARPANEL_FORWARD;
            }
            else{
                nextState = SOLARPANEL_FORWARD;
            }
        }
        break;

    case SOLARPANEL_END :
        if(initStat) LOG_STATE("SOLARPANEL_END");
        nextState = SOLARPANEL_INIT;
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

int takePlant(robotCDFR mainRobot, Asser* robot,Arduino* arduino,tableState*itable,int yPos,int xStart, int xEnd, int numPlante){
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
        positionToGo = itable->plantPosition[numPlante].x+100;
        arduino->servoPosition(4,100);
        robot->setLinearMaxSpeed(200);
        servoChange = false;
        servoChange2 = false;
        break;
    case TAKEPLANT_FORWARD :
        if(initStat) LOG_STATE("TAKEPLANT_FORWARD");
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,positionToGo,yPos);
        robot->getCoords(x,y,teta);
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
            robot->setLinearMaxSpeed(10000);
        }
        break;
    case TAKEPLANT_REFORWARD : 
        if(initStat){ LOG_STATE("TAKEPLANT_REFORWARD");
            positionToGo += 400;
        }
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,positionToGo,yPos);
        if(deplacementreturn>=1){
            nextState = TAKEPLANT_BACKWARD;
        }
        else if(deplacementreturn<=-1){
            nextState = TAKEPLANT_INIT;
            ireturn = -1;
            robot->setLinearMaxSpeed(10000);
        }
        break;
    case TAKEPLANT_BACKWARD :
        if(initStat){ LOG_STATE("TAKEPLANT_BACKWARD");
            positionToGo -= 115;
        }
        deplacementreturn = deplacementLinearPoint(mainRobot,robot,positionToGo,yPos);
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
        robot->setLinearMaxSpeed(10000);
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

int takePlant2(robotCDFR mainRobot, Asser* robot,Arduino* arduino,tableState*itable,int xStart,int yStart, int xEnd, int yEnd){
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
        robot->setLinearMaxSpeed(200);
        break;
    case TAKEPLANT_FORWARD :
        if(initStat) LOG_STATE("TAKEPLANT_FORWARD");
        deplacementreturn = deplacementgoToPointNoTurn(mainRobot,robot,xEnd,yEnd);
        if(deplacementreturn>=1){
            nextState = TAKEPLANT_BACKWARD;
        }
        else if(deplacementreturn<=-1){
            nextState = TAKEPLANT_INIT;
            ireturn = -1;
            robot->setLinearMaxSpeed(10000);
        }
        break;
    case TAKEPLANT_BACKWARD :
        if(initStat){ LOG_STATE("TAKEPLANT_BACKWARD");
        }
        deplacementreturn = deplacementgoToPointNoTurn(mainRobot,robot,xtogo,ytogo,MOVE_BACKWARD);
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
        robot->setLinearMaxSpeed(10000);
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

int jardinierePutPlant(robotCDFR mainRobot, Asser* robot,Arduino* arduino,int x,int y,int teta){
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
        deplacementreturn = deplacementgoToPoint(mainRobot,robot,x,y,teta,MOVE_FORWARD,ROTATION_DIRECT);
        if(deplacementreturn > 0){
            robot->stop();
            robot->brakeMotor(true);
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
            robot->stop();
            robot->brakeMotor(false);
            robot->enableMotor(true);
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
    if(itable->colorTeam == YELLOW){
        return  itable->JardiniereFull[3] && itable->JardiniereFull[4];//itable->JardiniereFull[0] &&
    }
    else{
        return itable->JardiniereFull[1] && itable->JardiniereFull[2] ;//&& itable->JardiniereFull[5]
    }
}

void resetActionneur(Asser* robot, Arduino* arduino){
    arduino->servoPosition(1,180);
    arduino->servoPosition(2,CLAMPSLEEP);
    arduino->moveStepper(ELEVATORUP,1);
    robot->setLinearMaxSpeed(10000);
}



int lastPlant(robotCDFR mainRobot,Asser* robot,Arduino* arduino,tableState*itable){
    LOG_SCOPE("lastPlant");
    int ireturn = 0;
    static bool initStat = true;
    static fsminitLastPlant_t currentState = LASTPLANT_INIT;
    fsminitLastPlant_t nextState = currentState;
    int deplacementreturn;


    switch (currentState)
    {
    //****************************************************************
    case LASTPLANT_INIT :
        if(initStat) LOG_STATE("LASTPLANT_INIT");
        nextState = LASTPLANT_TAKE;
        break;
    //****************************************************************
    case LASTPLANT_TAKE :
        if(initStat) LOG_STATE("LASTPLANT_TAKE");
        if(takePlant2(mainRobot,robot,arduino,itable,itable->plantPosition[0].x,itable->plantPosition[0].y + 250,itable->plantPosition[0].x,itable->plantPosition[0].y - 150)){
            nextState = LASTPLANT_GOPOT;       
        }

        break;
    //****************************************************************
    case LASTPLANT_GOPOT :
        if(initStat) LOG_STATE("LASTPLANT_GOPOT");
        if(deplacementgoToPoint(mainRobot,robot,887,-1130,90)){
            nextState = LASTPLANT_TURN;
            arduino->servoPosition(2,CLAMPOPEN);
        }
        break;
    //****************************************************************
    case LASTPLANT_REALSE :
        if(initStat) LOG_STATE("LASTPLANT_REALSE");
        nextState = LASTPLANT_TURN;
        break;
    //****************************************************************
    case LASTPLANT_TURN :
        if(initStat) LOG_STATE("LASTPLANT_TURN");
        if(deplacementgoToPoint(mainRobot,robot,887,-1170,90)){
            nextState = LASTPLANT_INIT;
            ireturn = 1;
        }
        break;
    //****************************************************************
    //****************************************************************    
    default:
        if(initStat) LOG_ERROR("default");
        nextState = LASTPLANT_INIT;
        break;
    }

    initStat = false;
    if(nextState != currentState){
        initStat = true;
    }
    currentState = nextState;
    return ireturn;
}

//








int returnToHome(Asser* robot){
    static int step = 0;
    bool breturn = false;
    if(step == 0){
        robot->setLookForward(0,-1300,0);
        step++;   
    }
    else if(step == 1 && !robot->getError(ANGULAR_ERROR)){
        robot->linearSetpoint(0,-1300);
        step++;
    }
    else if(step == 2){
        if(!robot->getError(LINEAR_ERROR)){
            step++;
        }        
    }
    else if(step == 3){
        breturn = true;
    }

    return breturn; 
}

int FSMMatch(robotCDFR mainRobot, Asser* robot,Arduino* arduino){
    int  bFinMatch = turnSolarPannel(mainRobot,robot, arduino);
    if(bFinMatch == 1){
        printf("FIN turnSolarPannel\n");
    }
    // fsmMatch_t nextStateMatch = currentStateMatch;
    // switch (currentStateMatch)
    // {
    // case SOLARPANNEL:{
    //     bool bret = turnSolarPannel(robot, arduino,collideB);
    //     if(bret){
    //         nextStateMatch = RETURNHOME;
    //     }
    //     break;
    // }
    // case RETURNHOME:{
    //     int bFinMatch = returnToHome(robot,collideF);
    //     break;
    // }       
    
    // default:
    //     break;
    // }

    return bFinMatch;
}

int TestPinceFSM(robotCDFR mainRobot, Asser* robot,Arduino* arduino){
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
        deplacementreturn = deplacementgoToPoint(mainRobot,robot,-700,-505,0,MOVE_FORWARD,ROTATION_DIRECT);
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
        //deplacementreturn = takePlant(mainRobot,robot,arduino,-505,-700,0,2);
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
        deplacementreturn = deplacementgoToPoint(mainRobot,robot,-700,-762,180,MOVE_FORWARD,ROTATION_DIRECT);
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
        deplacementreturn = deplacementgoToPoint(mainRobot,robot,-870,-762,-180,MOVE_FORWARD,ROTATION_DIRECT);
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
            robot->stop();
            nextState = TESTPINCE_GOBACKWARD;
        }
        break;
    //****************************************************************
    case TESTPINCE_GOBACKWARD :
        if(initStat) LOG_STATE("TESTPINCE_GOBACKWARD");
        deplacementreturn = deplacementgoToPoint(mainRobot,robot,-700,-762,-180,MOVE_BACKWARD,ROTATION_DIRECT);
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
        deplacementreturn = deplacementgoToPoint(mainRobot,robot,-800,-1250,-90,MOVE_FORWARD,ROTATION_DIRECT);
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

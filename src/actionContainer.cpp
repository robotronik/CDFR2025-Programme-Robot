#include "actionContainer.hpp"

actionContainer::actionContainer( CmdAsserv* irobot, Arduino* iarduino, TableState* itable){
    robot = irobot;
    arduino = iarduino;
    table = itable;

    takePlante0droite = new action("takePlante0droite",irobot,iarduino,itable);
    takePlante0droitebis = new action("takePlante0droitebis",irobot,iarduino,itable);
    takePlante0gauche = new action("takePlante0gauche",irobot,iarduino,itable);
    takePlante0gaucheBis = new action("takePlante0gaucheBis",irobot,iarduino,itable);
    takePlante1droite = new action("takePlante1droite",irobot,iarduino,itable);
    takePlante1gauche = new action("takePlante1gauche",irobot,iarduino,itable);
    takePlante2droite = new action("takePlante2droite",irobot,iarduino,itable);
    takePlante2gauche = new action("takePlante2gauche",irobot,iarduino,itable);
    takePlante3droite = new action("takePlante3droite",irobot,iarduino,itable);
    takePlante3gauche = new action("takePlante3gauche",irobot,iarduino,itable);
    takePlante4droite = new action("takePlante4droite",irobot,iarduino,itable);
    takePlante4gauche = new action("takePlante4gauche",irobot,iarduino,itable);
    takePlante5droite = new action("takePlante5droite",irobot,iarduino,itable);
    takePlante5gauche = new action("takePlante5gauche",irobot,iarduino,itable);

    putInJardiniere0 = new action("putInJardiniere0",irobot,iarduino,itable);
    putInJardiniere1 = new action("putInJardiniere1",irobot,iarduino,itable);
    putInJardiniere2 = new action("putInJardiniere2",irobot,iarduino,itable);
    putInJardiniere3 = new action("putInJardiniere3",irobot,iarduino,itable);
    putInJardiniere4 = new action("putInJardiniere4",irobot,iarduino,itable);
    putInJardiniere5 = new action("putInJardiniere5",irobot,iarduino,itable);

    pushPotAction0 = new action("pushPotAction0",irobot,iarduino,itable);
    pushPotAction1 = new action("pushPotAction1",irobot,iarduino,itable);
    pushPotAction4 = new action("pushPotAction2",irobot,iarduino,itable);
    pushPotAction5 = new action("pushPotAction3",irobot,iarduino,itable);

    waitFin = new action("waitFin",irobot,iarduino,itable);
    turnSolarPanelAction = new action("turnSolarPanelAction",irobot,iarduino,itable);
    returnToHomeAction = new action("returnToHomeAction",irobot,iarduino,itable);
    VolZone = new action("VolZone",irobot,iarduino,itable);
    VolJardiniereBlue = new action ("VolJardinière",irobot,iarduino,itable);
    VolJardiniereYellow = new action ("VolJardiniereYellow",irobot,iarduino,itable);

}
void actionContainer::initAction( CmdAsserv* irobot, Arduino* iarduino, TableState* itable){
    
// ACTION 1
    takePlante0droite->setStartPoint(plantPosition[0].x - MARGESTOCKPLANTX,plantPosition[0].y - MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante0droite->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[0].x - MARGESTOCKPLANTX,plantPosition[0].y - MARGESTOCKPLANTY,plantPosition[0].x + MARGESTOCKPLANTX/DIVIDE,plantPosition[0].y + MARGESTOCKPLANTY/DIVIDE,0);
    });
    takePlante0droite->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[0].etat = false;
    });
    takePlante0droite->setCostAction(1,0,itable,plantPosition[0].x - MARGESTOCKPLANTX,plantPosition[0].y - MARGESTOCKPLANTY); //num action puis num zone de l'action
    listeAction.push_back(takePlante0droite);

// ACTION 1
    takePlante0droitebis->setStartPoint(plantPosition[0].x + MARGESTOCKPLANTX,plantPosition[0].y - MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante0droitebis->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[0].x + MARGESTOCKPLANTX,plantPosition[0].y - MARGESTOCKPLANTY,plantPosition[0].x - MARGESTOCKPLANTX/DIVIDE,plantPosition[0].y + MARGESTOCKPLANTY/DIVIDE,0);
    });
    takePlante0droitebis->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[0].etat = false;
    });
    takePlante0droitebis->setCostAction(1,0,itable,plantPosition[0].x + MARGESTOCKPLANTX,plantPosition[0].y - MARGESTOCKPLANTY); //num action puis num zone de l'action
    listeAction.push_back(takePlante0droitebis);

// ACTION 1
    takePlante0gauche->setStartPoint(plantPosition[0].x - MARGESTOCKPLANTX,plantPosition[0].y + MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante0gauche->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[0].x - MARGESTOCKPLANTX,plantPosition[0].y + MARGESTOCKPLANTY,plantPosition[0].x + MARGESTOCKPLANTX/DIVIDE,plantPosition[0].y - MARGESTOCKPLANTY/DIVIDE,0);
    });
    takePlante0gauche->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[0].etat = false;
    });
    takePlante0gauche->setCostAction(1,0,itable,plantPosition[0].x - MARGESTOCKPLANTX,plantPosition[0].y + MARGESTOCKPLANTY); //num action puis num zone de l'action
    listeAction.push_back(takePlante0gauche);

// ACTION 1
    takePlante0gaucheBis->setStartPoint(plantPosition[0].x + MARGESTOCKPLANTX,plantPosition[0].y + MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante0gaucheBis->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[0].x + MARGESTOCKPLANTX,plantPosition[0].y + MARGESTOCKPLANTY,plantPosition[0].x - MARGESTOCKPLANTX/DIVIDE,plantPosition[0].y - MARGESTOCKPLANTY/DIVIDE,0);
    });
    takePlante0gaucheBis->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[0].etat = false;
    });
    takePlante0gaucheBis->setCostAction(1,0,itable,plantPosition[0].x + MARGESTOCKPLANTX,plantPosition[0].y + MARGESTOCKPLANTY); //num action puis num zone de l'action
    listeAction.push_back(takePlante0gaucheBis);

// ACTION 1
    takePlante1droite->setStartPoint(plantPosition[1].x - MARGESTOCKPLANTX,plantPosition[1].y - MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante1droite->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[1].x - MARGESTOCKPLANTX,plantPosition[1].y - MARGESTOCKPLANTY,plantPosition[1].x + MARGESTOCKPLANTX/DIVIDE,plantPosition[1].y + MARGESTOCKPLANTY/DIVIDE,1);
    });
    takePlante1droite->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[1].etat = false;
    });
    takePlante1droite->setCostAction(1,1,itable,plantPosition[1].x - MARGESTOCKPLANTX,plantPosition[1].y - MARGESTOCKPLANTY);
    listeAction.push_back(takePlante1droite);

// ACTION 1
    takePlante1gauche->setStartPoint(plantPosition[1].x + MARGESTOCKPLANTX,plantPosition[1].y + MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante1gauche->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[1].x + MARGESTOCKPLANTX,plantPosition[1].y + MARGESTOCKPLANTY,plantPosition[1].x - MARGESTOCKPLANTX/DIVIDE,plantPosition[1].y - MARGESTOCKPLANTY/DIVIDE,1);
    });
    takePlante1gauche->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[1].etat = false;
    });
    takePlante1gauche->setCostAction(1,1,itable,plantPosition[1].x + MARGESTOCKPLANTX,plantPosition[1].y + MARGESTOCKPLANTY);
    listeAction.push_back(takePlante1gauche);

// ACTION 1
    takePlante2droite->setStartPoint(plantPosition[2].x - MARGESTOCKPLANTX,plantPosition[2].y - MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante2droite->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[2].x - MARGESTOCKPLANTX,plantPosition[2].y - MARGESTOCKPLANTY,plantPosition[2].x + MARGESTOCKPLANTX/DIVIDE,plantPosition[2].y + MARGESTOCKPLANTY/DIVIDE,2);
    });
    takePlante2droite->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[2].etat = false;
    });
    takePlante2droite->setCostAction(1,2,itable,plantPosition[2].x - MARGESTOCKPLANTX,plantPosition[2].y - MARGESTOCKPLANTY);
    listeAction.push_back(takePlante2droite);

// ACTION 1
    takePlante2gauche->setStartPoint(plantPosition[2].x + MARGESTOCKPLANTX,plantPosition[2].y + MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante2gauche->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[2].x + MARGESTOCKPLANTX,plantPosition[2].y + MARGESTOCKPLANTY,plantPosition[2].x - MARGESTOCKPLANTX/DIVIDE,plantPosition[2].y - MARGESTOCKPLANTY/DIVIDE,2);
    });
    takePlante2gauche->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[2].etat = false;
    });
    takePlante2gauche->setCostAction(1,2,itable,plantPosition[2].x + MARGESTOCKPLANTX,plantPosition[2].y + MARGESTOCKPLANTY);
    listeAction.push_back(takePlante2gauche);

// ACTION 1
    takePlante3droite->setStartPoint(plantPosition[3].x + MARGESTOCKPLANTX,plantPosition[3].y - MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante3droite->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[3].x + MARGESTOCKPLANTX,plantPosition[3].y - MARGESTOCKPLANTY,plantPosition[3].x - MARGESTOCKPLANTX/DIVIDE,plantPosition[3].y + MARGESTOCKPLANTY/DIVIDE,3);
    });
    takePlante3droite->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[3].etat = false;
    });
    takePlante3droite->setCostAction(1,3,itable,plantPosition[3].x + MARGESTOCKPLANTX,plantPosition[3].y - MARGESTOCKPLANTY);
    listeAction.push_back(takePlante3droite);

// ACTION 1
    takePlante3gauche->setStartPoint(plantPosition[3].x + MARGESTOCKPLANTX,plantPosition[3].y + MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante3gauche->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[3].x + MARGESTOCKPLANTX,plantPosition[3].y + MARGESTOCKPLANTY,plantPosition[3].x - MARGESTOCKPLANTX/DIVIDE,plantPosition[3].y - MARGESTOCKPLANTY/DIVIDE,3);
    });
    takePlante3gauche->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[3].etat = false;
    });
    takePlante3gauche->setCostAction(1,3,itable,plantPosition[3].x + MARGESTOCKPLANTX,plantPosition[3].y + MARGESTOCKPLANTY);
    listeAction.push_back(takePlante3gauche);

// ACTION 1
    takePlante4droite->setStartPoint(plantPosition[4].x + MARGESTOCKPLANTX,plantPosition[4].y - MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante4droite->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[4].x + MARGESTOCKPLANTX,plantPosition[4].y - MARGESTOCKPLANTY,plantPosition[4].x - MARGESTOCKPLANTX/DIVIDE,plantPosition[4].y + MARGESTOCKPLANTY/DIVIDE,4);
    });
    takePlante4droite->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[4].etat = false;
    });
    takePlante4droite->setCostAction(1,4,itable,plantPosition[4].x + MARGESTOCKPLANTX,plantPosition[4].y - MARGESTOCKPLANTY);
    listeAction.push_back(takePlante4droite);

// ACTION 1
    takePlante4gauche->setStartPoint(plantPosition[4].x - MARGESTOCKPLANTX,plantPosition[4].y + MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante4gauche->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[4].x - MARGESTOCKPLANTX,plantPosition[4].y + MARGESTOCKPLANTY,plantPosition[4].x + MARGESTOCKPLANTX/DIVIDE,plantPosition[4].y - MARGESTOCKPLANTY/DIVIDE,4);
    });
    takePlante4gauche->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[4].etat = false;
    });
    takePlante4gauche->setCostAction(1,4,itable,plantPosition[4].x - MARGESTOCKPLANTX,plantPosition[4].y + MARGESTOCKPLANTY);
    listeAction.push_back(takePlante4gauche);

// ACTION 1
    takePlante5droite->setStartPoint(plantPosition[5].x + MARGESTOCKPLANTX,plantPosition[5].y - MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante5droite->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[5].x + MARGESTOCKPLANTX,plantPosition[5].y - MARGESTOCKPLANTY,plantPosition[5].x - MARGESTOCKPLANTX/DIVIDE,plantPosition[5].y + MARGESTOCKPLANTY/DIVIDE,5);
    });
    takePlante5droite->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[5].etat = false;
    });
    takePlante5droite->setCostAction(1,5,itable,plantPosition[5].x + MARGESTOCKPLANTX,plantPosition[5].y - MARGESTOCKPLANTY);
    listeAction.push_back(takePlante5droite);

// ACTION 1
    takePlante5gauche->setStartPoint(plantPosition[5].x - MARGESTOCKPLANTX,plantPosition[5].y + MARGESTOCKPLANTY,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    takePlante5gauche->setRunAction([&](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return takePlant2(iAsser,iarduino,itable,plantPosition[5].x - MARGESTOCKPLANTX,plantPosition[5].y + MARGESTOCKPLANTY,plantPosition[5].x + MARGESTOCKPLANTX/DIVIDE,plantPosition[5].y - MARGESTOCKPLANTY/DIVIDE,5);
    });
    takePlante5gauche->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->robot.robotHavePlante = true;
        itable->planteStockFull[5].etat = false;
    });
    takePlante5gauche->setCostAction(1,5,itable,plantPosition[5].x - MARGESTOCKPLANTX,plantPosition[5].y + MARGESTOCKPLANTY);
    listeAction.push_back(takePlante5gauche);




     
// ACTION 2 YELLOW 
    putInJardiniere0->setStartPoint(JardinierePosition[0].x, JardinierePosition[0].y+MARGEJADINIERE, 90, CmdAsserv::MOVE_FORWARD, CmdAsserv::ROTATION_DIRECT);
    putInJardiniere0->setEndPoint(JardinierePosition[0].x, JardinierePosition[0].y+MARGEJADINIERE, 90, CmdAsserv::MOVE_BACKWARD, CmdAsserv::ROTATION_DIRECT);
    putInJardiniere0->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return jardinierePutPlant(itable, iAsser, iarduino, JardinierePosition[0].x, JardinierePosition[0].y+110, 90);
    });
    putInJardiniere0->goodEnd([](TableState*itable,CmdAsserv*irobot){
        int16_t x,y,teta;
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[0].etat = true;
        irobot->get_coordinates(x,y,teta);
        irobot->set_coordinates(x,- 1500 + ROBOT_Y_OFFSET ,90);
        itable->incrementScore(PLANTE_JARDINIERE);
    });
    putInJardiniere0->setCostAction(2,0,itable,JardinierePosition[0].x, JardinierePosition[0].y+MARGEJADINIERE);
    listeAction.push_back(putInJardiniere0);


// ACTION 2 BLUE
    putInJardiniere1->setStartPoint(JardinierePosition[1].x, JardinierePosition[1].y+MARGEJADINIERE, 90, CmdAsserv::MOVE_BACKWARD, CmdAsserv::ROTATION_DIRECT);
    putInJardiniere1->setEndPoint(JardinierePosition[1].x, JardinierePosition[1].y+MARGEJADINIERE, 90, CmdAsserv::MOVE_BACKWARD, CmdAsserv::ROTATION_DIRECT);
    putInJardiniere1->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return jardinierePutPlant(itable, iAsser, iarduino, JardinierePosition[1].x, JardinierePosition[1].y+110, 90);
    });
    putInJardiniere1->goodEnd([](TableState*itable,CmdAsserv*irobot){
        int16_t x,y,teta;
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[1].etat = true;
        irobot->get_coordinates(x,y,teta);
        irobot->set_coordinates(x,-1500 + ROBOT_Y_OFFSET ,90);
        itable->incrementScore(PLANTE_JARDINIERE);
    });
    putInJardiniere1->setCostAction(2,1,itable,JardinierePosition[1].x, JardinierePosition[1].y+MARGEJADINIERE);
    listeAction.push_back(putInJardiniere1);


// ACTION 2 BLUE
    putInJardiniere2->setStartPoint(-700,-732,180,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    putInJardiniere2->setEndPoint(-700,-732,180,CmdAsserv::MOVE_BACKWARD,CmdAsserv::ROTATION_DIRECT);
    putInJardiniere2->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return jardinierePutPlant(itable,iAsser,iarduino,-890,-732,-180);
    });
    putInJardiniere2->goodEnd([](TableState*itable,CmdAsserv*irobot){
        int16_t x,y,teta;
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[2].etat = true;
        irobot->get_coordinates(x,y,teta);
        irobot->set_coordinates(-1000 + ROBOT_Y_OFFSET,y ,-180);
        itable->incrementScore(PLANTE_JARDINIERE);
    });
    putInJardiniere2->setCostAction(2,2,itable,-700,-732);
    listeAction.push_back(putInJardiniere2);


// ACTION 2 YELLOW
    putInJardiniere3->setStartPoint(-700,732,180,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    putInJardiniere3->setEndPoint(-700,732,180,CmdAsserv::MOVE_BACKWARD,CmdAsserv::ROTATION_DIRECT);
    putInJardiniere3->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return jardinierePutPlant(itable,iAsser,iarduino,-890,732,-180);
    });
    putInJardiniere3->goodEnd([](TableState*itable,CmdAsserv*irobot){
        int16_t x,y,teta;
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[3].etat = true;
        irobot->get_coordinates(x,y,teta);
        irobot->set_coordinates(-1000 + ROBOT_Y_OFFSET,y ,-180);
        itable->incrementScore(PLANTE_JARDINIERE);
    });
    putInJardiniere3->setCostAction(2,3,itable,-700,732);
    listeAction.push_back(putInJardiniere3);


// ACTION 2 YELLOW
    putInJardiniere4->setStartPoint(JardinierePosition[4].x, JardinierePosition[4].y-MARGEJADINIERE, -90, CmdAsserv::MOVE_BACKWARD, CmdAsserv::ROTATION_DIRECT);
    putInJardiniere4->setEndPoint(JardinierePosition[4].x, JardinierePosition[4].y-MARGEJADINIERE, -90, CmdAsserv::MOVE_BACKWARD, CmdAsserv::ROTATION_DIRECT);
    putInJardiniere4->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return jardinierePutPlant(itable, iAsser,iarduino,JardinierePosition[4].x, JardinierePosition[4].y-110,-90);
    });
    putInJardiniere4->goodEnd([](TableState*itable,CmdAsserv*irobot){
        int16_t x,y,teta;
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[4].etat = true;
        irobot->get_coordinates(x,y,teta);
        irobot->set_coordinates(x,1500 - ROBOT_Y_OFFSET ,-90);
        itable->incrementScore(PLANTE_JARDINIERE);
    });
    putInJardiniere4->setCostAction(2,4,itable,JardinierePosition[4].x, JardinierePosition[4].y-MARGEJADINIERE);
    listeAction.push_back(putInJardiniere4);

// ACTION 2 BLUE
    putInJardiniere5->setStartPoint(JardinierePosition[5].x, JardinierePosition[5].y-MARGEJADINIERE, -90, CmdAsserv::MOVE_FORWARD, CmdAsserv::ROTATION_DIRECT);
    putInJardiniere5->setEndPoint(JardinierePosition[5].x, JardinierePosition[5].y-MARGEJADINIERE, -90, CmdAsserv::MOVE_BACKWARD, CmdAsserv::ROTATION_DIRECT);
    putInJardiniere5->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return jardinierePutPlant(itable,iAsser,iarduino,JardinierePosition[5].x , JardinierePosition[5].y-110, -90);
    });
    putInJardiniere5->goodEnd([](TableState*itable,CmdAsserv*irobot){
        int16_t x,y,teta;
        itable->robot.robotHavePlante = false;
        itable->JardiniereFull[5].etat = true;
        irobot->get_coordinates(x,y,teta);
        irobot->set_coordinates(x,1500 - ROBOT_Y_OFFSET ,-90);
        itable->incrementScore(PLANTE_JARDINIERE);
    });
    putInJardiniere5->setCostAction(2,5,itable,JardinierePosition[5].x, JardinierePosition[5].y-MARGEJADINIERE);
    listeAction.push_back(putInJardiniere5);

// ACTION 3
    turnSolarPanelAction->setKeyMoment(65000);
    turnSolarPanelAction->setStartPoint(810,(itable->robot.colorTeam == YELLOW ? 1100 : -1100),(itable->robot.colorTeam == YELLOW ? -90 : 90), CmdAsserv::MOVE_FORWARD, CmdAsserv::ROTATION_DIRECT);
    turnSolarPanelAction->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return turnSolarPannel(itable, iAsser, iarduino);
    });
    turnSolarPanelAction->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->solarPanelTurn.etat = true;
    });
    turnSolarPanelAction->setCostAction(3,0,itable,0,0);
    listeAction.push_back(turnSolarPanelAction);


        // ACTION 4
    returnToHomeAction->setStartPoint(700,(itable->robot.colorTeam == YELLOW ? 1200 : -1200), CmdAsserv::MOVE_FORWARD, CmdAsserv::ROTATION_DIRECT);
    returnToHomeAction->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        int iret = 0;
        if(FastReleasePlant(iarduino)){iret = -100;}
        return iret;});
    returnToHomeAction->setKeyMoment(85000);
    returnToHomeAction->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->incrementScore(RETURN_HOME);
    });
    returnToHomeAction->setCostAction(4,0,itable,0,0);
    listeAction.push_back(returnToHomeAction);

    //ACTION 5
    waitFin->setStartPoint(700,0,(itable->robot.colorTeam == YELLOW ? -90 : 90),CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    waitFin->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable){
        while (itable->startTime+88000 > millis()){
            sleep(0.25);
        }
        return 1;
    });
    waitFin->setCostAction(5,0,itable,0,0);
    listeAction.push_back(waitFin);

//PUSH POT ACTION 6
    pushPotAction0->setStartPoint(JardinierePosition[0].x+PUSHPOTMARGEX1,-1500+PUSHPOTMARGEY,-180, CmdAsserv::MOVE_FORWARD, CmdAsserv::ROTATION_DIRECT);
    pushPotAction0->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return deplacementgoToPoint(itable->robot.collide, iAsser, JardinierePosition[0].x+PUSHPOTMARGEX2, -1500+PUSHPOTMARGEY, -180, CmdAsserv::MOVE_BACKWARD, CmdAsserv::ROTATION_DIRECT);
    });
    pushPotAction0->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->jardiniereFree[0].etat = true;
    });
    pushPotAction0->setCostAction(6,0,itable,JardinierePosition[0].x+PUSHPOTMARGEX1,-1500+PUSHPOTMARGEY);
    listeAction.push_back(pushPotAction0);


//PUSH POT ACTION 6
    pushPotAction1->setStartPoint(JardinierePosition[1].x+PUSHPOTMARGEX2,-1500+PUSHPOTMARGEY, CmdAsserv::MOVE_BACKWARD, CmdAsserv::ROTATION_DIRECT);
    pushPotAction1->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return 1; });
    pushPotAction1->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->jardiniereFree[1].etat = true;
    });
    pushPotAction1->setCostAction(6,1,itable,JardinierePosition[1].x+PUSHPOTMARGEX2,-1500+PUSHPOTMARGEY);
    listeAction.push_back(pushPotAction1);


//PUSH POT ACTION 6
    pushPotAction4->setStartPoint(JardinierePosition[4].x+PUSHPOTMARGEX2,1500-PUSHPOTMARGEY, CmdAsserv::MOVE_BACKWARD, CmdAsserv::ROTATION_DIRECT);
    pushPotAction4->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return 1;});
    pushPotAction4->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->jardiniereFree[4].etat = true;
    });
    pushPotAction4->setCostAction(6,4,itable,JardinierePosition[4].x+PUSHPOTMARGEX2,1500-PUSHPOTMARGEY);
    listeAction.push_back(pushPotAction4);


//PUSH POT ACTION 6
    pushPotAction5->setStartPoint(JardinierePosition[5].x+PUSHPOTMARGEX1,1500-PUSHPOTMARGEY,-180, CmdAsserv::MOVE_FORWARD, CmdAsserv::ROTATION_DIRECT);
    pushPotAction5->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return deplacementgoToPoint(itable->robot.collide, iAsser, JardinierePosition[5].x+PUSHPOTMARGEX2, 1500-PUSHPOTMARGEY-20, -180, CmdAsserv::MOVE_BACKWARD, CmdAsserv::ROTATION_DIRECT);
    });
    pushPotAction5->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->jardiniereFree[5].etat = true;
    });
    pushPotAction5->setCostAction(6,5,itable,JardinierePosition[5].x+PUSHPOTMARGEX1,1500-PUSHPOTMARGEY);
    listeAction.push_back(pushPotAction5);
    
    //ACTION 7 : VOL_ZONE
    VolZone->setStartPoint((itable->robot.colorTeam == YELLOW ? -820 : 820),(itable->robot.colorTeam == YELLOW ? -1290 : 1290),-180,CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    VolZone->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return deplacementgoToPoint(itable->robot.collide, iAsser, 200, (itable->robot.colorTeam == YELLOW ? -1250 : 1250), -180, CmdAsserv::MOVE_FORWARD,CmdAsserv::ROTATION_DIRECT);
    });
    VolZone->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->FIN = true;
        itable->incrementScore(RETURN_HOME);
    });
    VolZone->setCostAction(7,4,itable,(itable->robot.colorTeam == YELLOW ? -820 : 820),(itable->robot.colorTeam == YELLOW ? -1290 : 1290));
    listeAction.push_back(VolZone);
    
    
    //ACTION 8 : VOL_JARDINIERE BLUE : ma team = BLUE
    VolJardiniereBlue->setStartPoint(JardinierePosition[4].x - 30, JardinierePosition[4].y-MARGEJADINIERE-20, -90, CmdAsserv::MOVE_FORWARD, CmdAsserv::ROTATION_DIRECT);
    VolJardiniereBlue->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return VolPlante(itable,iAsser,iarduino,JardinierePosition[4].x-30, JardinierePosition[4].y-110, -90);
    });
    VolJardiniereBlue->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->FIN = true;
        itable->incrementScore(RETURN_HOME);
    });
    VolJardiniereBlue->setCostAction(8,4,itable,JardinierePosition[4].x, JardinierePosition[4].y-MARGEJADINIERE-20);
    listeAction.push_back(VolJardiniereBlue);

    //ACTION 8 : VOL_JARDINIERE Yellow : ma team = Yellow
    VolJardiniereYellow->setStartPoint(JardinierePosition[1].x, JardinierePosition[1].y+MARGEJADINIERE+20, 90, CmdAsserv::MOVE_FORWARD, CmdAsserv::ROTATION_DIRECT);
    VolJardiniereYellow->setRunAction([](action* iaction, CmdAsserv* iAsser, Arduino* iarduino, TableState*itable) {
        return VolPlante(itable,iAsser,iarduino,JardinierePosition[1].x, JardinierePosition[1].y+110, 90);
    });
    VolJardiniereYellow->goodEnd([](TableState*itable,CmdAsserv*irobot){
        itable->FIN = true;
        itable->incrementScore(RETURN_HOME);
    });
    VolJardiniereYellow->setCostAction(8,1,itable,JardinierePosition[1].x, JardinierePosition[1].y+MARGEJADINIERE+20);
    listeAction.push_back(VolJardiniereYellow);

    

    //Choose first action
    choosNextAction();
}

int actionContainer::actionContainerRun(CmdAsserv * robot,TableState* itable){
    int iActionReturn, iChoosNextReturn = 0, iRet = 0;
    iActionReturn = currentAction->runAction();
    if(iActionReturn == -100){
        iRet = -100;
    }
    else if(iActionReturn!=0){
        resetActionneur(robot,arduino);
        initAction(robot, arduino, table);
    }
    // else if(forceNextAction()){
    //     resetActionneur(robot,arduino);
    //     iChoosNextReturn = choosNextAction();
    // }

    if(iChoosNextReturn == -1){
        iRet = -1;
    }
    return iRet;
}

actionContainer::~actionContainer(){
}

bool actionContainer::forceNextAction(void){
    bool bRet = false;
    for (action* elem : listeAction) {
        if(elem->actionNeedForce()){
            bRet = true;
        }
    }
    return bRet;
}

int actionContainer::choosNextAction(void){
    LOG_GREEN_INFO("CHOOSE NEW ACTION: ");
    int bestCost = -1;
    for (action* elem : listeAction) {
        int cost = elem->costAction();
        //LOG_INFO("elem = ", currentAction->getName(), "cost = ", cost);
        if(cost>bestCost){
            bestCost = cost;
            currentAction = elem;
        }
    }
    if(bestCost!=-1) {LOG_GREEN_INFO("CHOOSE : ",currentAction->getName(), " / cost : ",bestCost);}
    return bestCost;
}

void actionContainer::resetAllAction(void){
    for (action* elem : listeAction) {
        elem->resetActionEnable();
    }
}
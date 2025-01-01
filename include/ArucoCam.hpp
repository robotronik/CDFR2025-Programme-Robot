#pragma once

class ArucoCam {   
private:
    int pid;
    int id;
public:
    ArucoCam(int cam_number);
    ~ArucoCam();
    bool getPos();
};
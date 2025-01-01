#pragma once

class ArucoCam {   
private:
    int pid;
public:
    ArucoCam(int cam_number);
    ~ArucoCam();
    bool getPos();
};
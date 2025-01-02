#pragma once

class ArucoCam {   
private:
    int pid;
    int id;
public:
    ArucoCam(int cam_number, const std::string &calibration_file_path);
    ~ArucoCam();
    bool getPos();
};
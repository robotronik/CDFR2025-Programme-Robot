#include "lidar/Lidar.hpp"
#include <vector>

static bool checkSLAMTECLIDARHealth(ILidarDriver * drv);

Lidar::Lidar(){
    drv = NULL;
}
Lidar::~Lidar(){
    lidarDelete();
}
bool Lidar::setup(const char* serialPort ,int baudrate){

    static sl_result op_result;
    drv = *createLidarDriver();

    if (!drv) {
        fprintf(stderr, "insufficent memory, exit\n");
        exit(-2);
    }

    sl_lidar_response_device_info_t devinfo;
    bool connectSuccess = false;
    IChannel* _channel;
    

    _channel = (*createSerialPortChannel(serialPort, baudrate));
    if (SL_IS_OK((drv)->connect(_channel))) {
        op_result = drv->getDeviceInfo(devinfo);

        if (SL_IS_OK(op_result)) 
        {
            connectSuccess = true;
        }
        else{
            delete drv;
            drv = NULL;
        }
    }


    if (!connectSuccess) {
        (fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n", serialPort));
        lidarDelete();
        return false;
    }

    // print out the device serial number, firmware and hardware version number..
    printf("============================\n");
    printf("SLAMTEC LIDAR S/N: ");
    for (int pos = 0; pos < 16 ;++pos) {
        printf("%02X", devinfo.serialnum[pos]);
    }
    printf("\n"
            "Firmware Ver: %d.%02d\n"
            "Hardware Rev: %d\n"
            , devinfo.firmware_version>>8
            , devinfo.firmware_version & 0xFF
            , (int)devinfo.hardware_version);

            
    // Get all supported scan modes
    std::vector<LidarScanMode> scanModes;
    sl_result res = drv->getAllSupportedScanModes(scanModes);
    if (SL_IS_FAIL(res)) {
        fprintf(stderr, "Error: Unable to retrieve scan modes.\n");
        lidarDelete();
        return false;
    }

    // Print each scan mode
    printf("Supported Scan Modes:\n");
    for (const auto &mode : scanModes) {
        printf("Mode ID: %u\n", mode.id);
        printf("Mode Name: %s\n", mode.scan_mode);
        printf("Time per Sample: %f µs\n", mode.us_per_sample);
        printf("Max Distance: %f m\n", mode.max_distance);
        printf("Answer Type: %d\n\n", (int)mode.ans_type);
    }

    // check health...
    if (!checkSLAMTECLIDARHealth(drv)) {
        lidarDelete();
        return false;
    }
    printf("============================\n");
    
    drv->setMotorSpeed(); // TODO Find the correct value
    // start scan...
    drv->startScan(0,1);

    return true;
}


void Lidar::lidarStop(void){
    if (drv){
        drv->stop();
        delay(200);
        drv->setMotorSpeed(0);
        lidarDelete();
    }
}
void Lidar::lidarDelete(){
    if (drv) {
        delete drv;
        drv = NULL;
    }
}


bool Lidar::getlidarData(lidarAnalize_t data[], int& countdata){
    sl_lidar_response_measurement_node_hq_t nodes[8192];
    size_t   count = _countof(nodes);
    sl_result op_result;

    op_result = drv->grabScanDataHq(nodes, count,0);

    if (SL_IS_OK(op_result)) {
        drv->ascendScanData(nodes, count);
        int pos;
        countdata = 0;
        for (pos = 0; pos < (int)count; ++pos) {
            bool valid = (nodes[pos].quality >> SL_LIDAR_RESP_MEASUREMENT_QUALITY_SHIFT) != 0;
            if (valid){
                data[countdata].onTable = 0;
                data[countdata].dist = nodes[pos].dist_mm_q2/4.0f;
                data[countdata].angle = (nodes[pos].angle_z_q14 * 90.f) / 16384.f;
                countdata++;
            }
        }
    }
    return SL_IS_OK(op_result);
}


bool checkSLAMTECLIDARHealth(ILidarDriver * drv)
{
    sl_result     op_result;
    sl_lidar_response_device_health_t healthinfo;

    op_result = drv->getHealth(healthinfo);
    if (SL_IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
        printf("SLAMTEC Lidar health status : %d\n", healthinfo.status);
        if (healthinfo.status == SL_LIDAR_STATUS_ERROR) {
            fprintf(stderr, "Error, slamtec lidar internal error detected. Please reboot the device to retry.\n");
            // enable the following code if you want slamtec lidar to be reboot by software
            // drv->reset();
            return false;
        } else {
            return true;
        }

    } else {
        fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
        return false;
    }
}
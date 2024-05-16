
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sl_lidar.h" 
#include "sl_lidar_driver.h"

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

#ifdef _WIN32
#include <Windows.h>
#define delay(x)   ::Sleep(x)
#else
#include <unistd.h>
static inline void delay(sl_word_size_t ms){
    while (ms>=1000){
        usleep(1000*1000);
        ms-=1000;
    };
    if (ms!=0)
        usleep(ms*1000);
}
#endif

using namespace sl;



void print_usage(int argc, const char * argv[])
{
    printf("Simple LIDAR data grabber for SLAMTEC LIDAR.\n"
           "Version:  %s \n"
           "Usage:\n"
           " $ sudo chmod 0666 /dev/ttyUSB0"
           " $ ./simple_grabber --channel --serial /dev/ttyUSB0 256000\n"
           
           , SL_LIDAR_SDK_VERSION,  argv[0], argv[0]);
}



sl_result capture_and_display(ILidarDriver * drv)
{
    sl_result ans;
    
	sl_lidar_response_measurement_node_hq_t nodes[8192];
    size_t   count = _countof(nodes);

    printf("waiting for data...\n");

    ans = drv->grabScanDataHq(nodes, count, 0);
    if (SL_IS_OK(ans) || ans == SL_RESULT_OPERATION_TIMEOUT) {
        drv->ascendScanData(nodes, count);
                
        for (int pos = 0; pos < (int)count ; ++pos) {
            if ((nodes[pos].dist_mm_q2 < 3600.0) && (nodes[pos].dist_mm_q2 > 1.0)){
                printf("%s theta: %03.2f Dist: %08.2f \n", 
                    (nodes[pos].flag & SL_LIDAR_RESP_HQ_FLAG_SYNCBIT) ?"S ":"  ", 
                    (nodes[pos].angle_z_q14 * 90.f) / 16384.f,
                    nodes[pos].dist_mm_q2/4.0f);
            }}
        
    } else {
        printf("error code: %x\n", ans);
    }

    return ans;
}

int main(int argc, const char * argv[]) {
	const char *opt_channel = NULL;
    const char *opt_channel_param_first = NULL;
    sl_u32      opt_channel_param_second = 0;
    sl_result   op_result;
	int         opt_channel_type = CHANNEL_TYPE_SERIALPORT;

    IChannel* _channel;


    
    if (argc < 5) {
        print_usage(argc, argv);
        return -1;
    }

	const char * opt_is_channel = argv[1];
	if(strcmp(opt_is_channel, "--channel")==0)
	{
		opt_channel = argv[2];
		if(strcmp(opt_channel, "-s")==0||strcmp(opt_channel, "--serial")==0)
		{
			opt_channel_param_first = argv[3];
			if (argc>4) opt_channel_param_second = strtoul(argv[4], NULL, 10);
		}
		else if(strcmp(opt_channel, "-u")==0||strcmp(opt_channel, "--udp")==0)
		{
			opt_channel_param_first = argv[3];
			if (argc>4) opt_channel_param_second = strtoul(argv[4], NULL, 10);
			opt_channel_type = CHANNEL_TYPE_UDP;
		}
		else
		{
			print_usage(argc, argv);
			return -1;
		}
	}
    else
	{
		print_usage(argc, argv);
		return -1;
	}

    // create the driver instance
	ILidarDriver * drv = *createLidarDriver();

    if (!drv) {
        fprintf(stderr, "insufficent memory, exit\n");
        exit(-2);
    }

    sl_lidar_response_device_health_t healthinfo;
    sl_lidar_response_device_info_t devinfo;
    do {
        // try to connect
        if (opt_channel_type == CHANNEL_TYPE_SERIALPORT) {
            _channel = (*createSerialPortChannel(opt_channel_param_first, opt_channel_param_second));
        }
        else if (opt_channel_type == CHANNEL_TYPE_UDP) {
            _channel = *createUdpChannel(opt_channel_param_first, opt_channel_param_second);
        }
        
        if (SL_IS_FAIL((drv)->connect(_channel))) {
			switch (opt_channel_type) {	
				case CHANNEL_TYPE_SERIALPORT:
					fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n"
						, opt_channel_param_first);
					break;
				case CHANNEL_TYPE_UDP:
					fprintf(stderr, "Error, cannot connect to the ip addr %s with the udp port %u.\n"
						, opt_channel_param_first, opt_channel_param_second);
					break;
			}
        }

        // retrieving the device info
        ////////////////////////////////////////
        op_result = drv->getDeviceInfo(devinfo);

        if (SL_IS_FAIL(op_result)) {
            if (op_result == SL_RESULT_OPERATION_TIMEOUT) {
                // you can check the detailed failure reason
                fprintf(stderr, "Error, operation time out.\n");
            } else {
                fprintf(stderr, "Error, unexpected error, code: %x\n", op_result);
                // other unexpected result
            }
            break;
        }

        // print out the device serial number, firmware and hardware version number..
        printf("SLAMTEC LIDAR S/N: ");
        for (int pos = 0; pos < 16 ;++pos) {
            printf("%02X", devinfo.serialnum[pos]);
        }

        printf("\n"
                "Version:  %s \n"
                "Firmware Ver: %d.%02d\n"
                "Hardware Rev: %d\n"
                , "SL_LIDAR_SDK_VERSION"
                , devinfo.firmware_version>>8
                , devinfo.firmware_version & 0xFF
                , (int)devinfo.hardware_version);


        // check the device health
        ////////////////////////////////////////
        op_result = drv->getHealth(healthinfo);
        if (SL_IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
            printf("Lidar health status : ");
            switch (healthinfo.status) 
			{
				case SL_LIDAR_STATUS_OK:
					printf("OK.");
					break;
				case SL_LIDAR_STATUS_WARNING:
					printf("Warning.");
					break;
				case SL_LIDAR_STATUS_ERROR:
					printf("Error.");
					break;
            }
            printf(" (errorcode: %d)\n", healthinfo.error_code);

        } else {
            fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
            break;
        }


        if (healthinfo.status == SL_LIDAR_STATUS_ERROR) {
            fprintf(stderr, "Error, slamtec lidar internal error detected. Please reboot the device to retry.\n");
            // enable the following code if you want slamtec lidar to be reboot by software
            // drv->reset();
            break;
        }

		switch (opt_channel_type) 
		{	
			case CHANNEL_TYPE_SERIALPORT:
				drv->setMotorSpeed();
			break;
		}

        // take only one 360 deg scan and display the result as a histogram
        ////////////////////////////////////////////////////////////////////////////////
        if (SL_IS_FAIL(drv->startScan( 0,1 ))) // you can force slamtec lidar to perform scan operation regardless whether the motor is rotating
        {
            fprintf(stderr, "Error, cannot start the scan operation.\n");
            break;
        }

		delay(1500);

        if (SL_IS_FAIL(capture_and_display(drv))) {
            fprintf(stderr, "Error, cannot grab scan data.\n");
            break;

        }

    } while(1);

    //drv->stop();
    switch (opt_channel_type) 
	{	
		case CHANNEL_TYPE_SERIALPORT:
			delay(20);
			drv->setMotorSpeed(0);
		break;
	}
    
    if(drv) {
        delete drv;
        drv = NULL;
    }
    return 0;
}

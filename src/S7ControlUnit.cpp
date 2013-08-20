//
//  S7ControlUnit.cpp
//  ControlUnitTest
//
//  Created by Claudio Bisegni on 7/20/13.
//  Copyright (c) 2013 INFN. All rights reserved.
//

#include "S7ControlUnit.h"
//#include "SinWaveCommand.h"
namespace cucs = chaos::cu::control_manager::slow_command;
namespace cu_driver = chaos::cu::driver_manager::driver;


/*
 Construct a new CU with an identifier
 */
S7ControlUnit::S7ControlUnit(string &customDeviceID) {
    _deviceID = customDeviceID;
}

S7ControlUnit::~S7ControlUnit() {
	
}

/*
 Return the default configuration
 */
void S7ControlUnit::unitDefineActionAndDataset() throw(CException) {
    //set the base information
    RangeValueInfo rangeInfoTemp;
    //cuSetup.addStringValue(CUDefinitionKey::CS_CM_CU_DESCRIPTION, "This is a beautifull CU");
    
    //add managed device di
    setDeviceID(_deviceID);
    
    //install a command
    //installCommand<SinWaveCommand>("sinwave_base");
    
    //set it has default
   // setDefaultCommand("sinwave_base");
    
    //setup the dataset
    addAttributeToDataSet("sinWave",
                          "The sin waveform",
                          DataType::TYPE_BYTEARRAY,
                          DataType::Output,
                          10000);
    
    addAttributeToDataSet("points",
                          "The number of point that compose the wave",
                          DataType::TYPE_INT32,
                          DataType::Input);
    rangeInfoTemp.defaultValue = "30";
    rangeInfoTemp.maxRange = "";
    rangeInfoTemp.minRange = "30";
    setAttributeRangeValueInfo("points", rangeInfoTemp);
    
    addAttributeToDataSet("frequency",
                          "The frequency of the wave [1-10Mhz]",
                          DataType::TYPE_DOUBLE,
                          DataType::Input);
    
    addAttributeToDataSet("bias",
                          "The bias of the wave",
                          DataType::TYPE_DOUBLE,
                          DataType::Input);
    
    addAttributeToDataSet("phase",
                          "The phase of the wave",
                          DataType::TYPE_DOUBLE,
                          DataType::Input);
    
    addAttributeToDataSet("gain",
                          "The gain of the wave",
                          DataType::TYPE_DOUBLE,
                          DataType::Input);

    addAttributeToDataSet("gain_noise",
                          "The gain of the noise of the wave",
                          DataType::TYPE_DOUBLE,
                          DataType::Input);

}

void S7ControlUnit::defineSharedVariable() {
    bool quit = false;
    addSharedVariable("quit", 1, chaos::DataType::TYPE_BOOLEAN);
    setSharedVariableValue("quit", &quit, sizeof(bool));
}

void S7ControlUnit::unitDefineDriver(std::vector<cu_driver::DrvRequestInfo>& neededDriver) {
	cu_driver::DrvRequestInfo drv1 = {"Sl7TcpDriver","1.0.0","plccalc.lnf.infn.it:102"};
	neededDriver.push_back(drv1);
}

// Abstract method for the initialization of the control unit
void S7ControlUnit::unitInit() throw(CException) {
	
}

// Abstract method for the start of the control unit
void S7ControlUnit::unitStart() throw(CException) {
	
}

// Abstract method for the stop of the control unit
void S7ControlUnit::unitStop() throw(CException) {
	
}

// Abstract method for the deinit of the control unit
void S7ControlUnit::unitDeinit() throw(CException) {
	
}
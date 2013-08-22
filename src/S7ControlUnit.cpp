//
//  S7ControlUnit.cpp
//  ControlUnitTest
//
//  Created by Claudio Bisegni on 7/20/13.
//  Copyright (c) 2013 INFN. All rights reserved.
//

#include "S7ControlUnit.h"
#include <boost/regex.hpp>

namespace cucs = chaos::cu::control_manager::slow_command;
namespace cu_driver = chaos::cu::driver_manager::driver;

#define S7CUAPP_ LAPP_ << "[S7ControlUnit] - "

//! the regular expresion that match with the rigth sintax of the definition string
static const boost::regex  t_definition_param_regex("(([0-4]):([0-9]+):([0-9]+):([0-9]+))(\\|([0-4]):([0-9]+):([0-9]+):([0-9]+))*");
static const boost::regex  t_single_block_regex("([0-4]):([0-9]+):([0-9]+):([0-9]+)");

bool S7ControlUnit::checkDefinitionparam(string& definitionParamter) {
	return regex_match(definitionParamter, t_definition_param_regex, boost::match_extra);
}

/*
 Construct a new CU with an identifier
 */
S7ControlUnit::S7ControlUnit(string _deviceID, string _definitionParamter):deviceID(_deviceID), definitionParamter(_definitionParamter) {
}

S7ControlUnit::~S7ControlUnit() {
	
}

/*
 Return the default configuration
 */
void S7ControlUnit::unitDefineActionAndDataset() throw(CException) {
	//check one more time the definition param and get the match
	if(!regex_match(definitionParamter, t_definition_param_regex, boost::match_extra)) throw CException(1, "Definition parameter si not well formed", __FUNCTION__);
	   
	boost::smatch match;
	std::string::const_iterator start = definitionParamter.begin();
	std::string::const_iterator end = definitionParamter.end();

	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	
	while(regex_search(start, end, what, t_single_block_regex, flags)) {

		S7CUAPP_ "Found definition bloc ->" << what[0];
		S7CUAPP_ "type		->" << what[1];
		S7CUAPP_ "DB Num	->" << what[2];
		S7CUAPP_ "Offset	->" << what[3];
		S7CUAPP_ "Length	->" << what[4];

		start = what[0].second;
		// update flags:
		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;
	}

    //set the base information
    RangeValueInfo rangeInfoTemp;
    //cuSetup.addStringValue(CUDefinitionKey::CS_CM_CU_DESCRIPTION, "This is a beautifull CU");
    
    //add managed device di
    setDeviceID(deviceID);
    
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


//intervalled scheduled method
void S7ControlUnit::unitRun() throw(CException) {
	
}

// Abstract method for the stop of the control unit
void S7ControlUnit::unitStop() throw(CException) {
	
}

// Abstract method for the deinit of the control unit
void S7ControlUnit::unitDeinit() throw(CException) {
	
}

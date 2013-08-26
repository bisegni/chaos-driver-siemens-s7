/*
 *	S7ControlUnit.cpp
 *	!CHOAS
 *	Created by Bisegni Claudio.
 *
 *    	Copyright 2013 INFN, National Institute of Nuclear Physics
 *
 *    	Licensed under the Apache License, Version 2.0 (the "License");
 *    	you may not use this file except in compliance with the License.
 *    	You may obtain a copy of the License at
 *
 *    	http://www.apache.org/licenses/LICENSE-2.0
 *
 *    	Unless required by applicable law or agreed to in writing, software
 *    	distributed under the License is distributed on an "AS IS" BASIS,
 *    	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    	See the License for the specific language governing permissions and
 *    	limitations under the License.
 */

#include "S7ControlUnit.h"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace cucs = chaos::cu::control_manager::slow_command;
namespace cu_driver = chaos::cu::driver_manager::driver;

#define S7CUAPP_ LAPP_ << "[S7ControlUnit] - "

//! the regular expresion that match with the rigth sintax of the definition string
static const boost::regex  t_definition_param_regex("([a-zA-Z0-9 ]+):([0-4]):([0-9]+):([0-9]+):([0-9]+):([0-9]+)(\\|([a-zA-Z0-9 ]*):([0-4]):([0-9]+):([0-9]+):([0-9]+):([0-9]+))*");
static const boost::regex  t_single_block_regex("([a-zA-Z0-9 ]+):([0-4]):([0-9]+):([0-9]+):([0-9]+):([0-9]+)");

bool S7ControlUnit::checkDefinitionparam(string& definitionParamter) {
	return regex_match(definitionParamter, t_definition_param_regex, boost::match_extra);
}

/*
 Construct a new CU with an identifier
 */
S7ControlUnit::S7ControlUnit(string _device_id, string _definition_paramteter, string _plc_addr_port):device_id(_device_id), definition_paramteter(_definition_paramteter), plc_addr_port(_plc_addr_port) {
	plc_s7_accessor = NULL;
}

S7ControlUnit::~S7ControlUnit() {
	
}

/*
 Return the default configuration
 */
void S7ControlUnit::unitDefineActionAndDataset() throw(CException) {
	uint32_t max_variable_length = 0;
	
	//check one more time the definition param and get the match
	if(!regex_match(definition_paramteter, t_definition_param_regex, boost::match_extra)) throw CException(1, "Definition parameter si not well formed", __FUNCTION__);
	   
	//we can work
	
	//set the device id
    setDeviceID(device_id);
	
	//scan the definition string
	boost::smatch match;
	std::string::const_iterator start = definition_paramteter.begin();
	std::string::const_iterator end = definition_paramteter.end();

	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	
	while(regex_search(start, end, what, t_single_block_regex, flags)) {

		S7CUAPP_ "Found definition bloc ->" << what[0];
		t_plc_variable *tmp_var = new t_plc_variable();
		if(!tmp_var) throw CException(2, "Error allcoating varible memory", __FUNCTION__);
		
		plc_tracked_variable.push_back(tmp_var);
		S7CUAPP_ "name			->" << what[1];
		S7CUAPP_ "type			->" << (tmp_var->type = (chaos::DataType::DataType)boost::lexical_cast<int>(what[2].str()));
		S7CUAPP_ "db number		->" << (tmp_var->variable.db_num = boost::lexical_cast<int>(what[3].str()));
		S7CUAPP_ "start			->" << (tmp_var->variable.start = boost::lexical_cast<int>(what[4].str()));
		S7CUAPP_ "byte count	->" << (tmp_var->variable.byte_count = boost::lexical_cast<int>(what[5].str()));
		S7CUAPP_ "max length	->" << (tmp_var->variable.max_len = boost::lexical_cast<int>(what[6].str()));

		switch ((chaos::DataType::DataType)tmp_var->type) {
			case chaos::DataType::TYPE_BYTEARRAY :
			case chaos::DataType::TYPE_STRING:
			break;
			
			default:
			max_variable_length = tmp_var->variable.max_len;
			break;
		}
		
		addAttributeToDataSet(what[1].str().c_str(),
							  "PLC Varible",
							  (chaos::DataType::DataType)tmp_var->type,
							  DataType::Output,
							  max_variable_length);
		
		start = what[0].second;
		// update flags:
		flags |= boost::match_prev_avail;
		flags |= boost::match_not_bob;
	}

}

void S7ControlUnit::unitDefineDriver(std::vector<cu_driver::DrvRequestInfo>& neededDriver) {
	cu_driver::DrvRequestInfo drv1 = {"SiemensS7TcpDriver", "1.0.0", plc_addr_port.c_str() };
	neededDriver.push_back(drv1);
}

// Abstract method for the initialization of the control unit
void S7ControlUnit::unitInit() throw(CException) {
	S7CUAPP_ "unitInit";
	plc_s7_accessor = AbstractControlUnit::getAccessoInstanceByIndex(0);
}

// Abstract method for the start of the control unit
void S7ControlUnit::unitStart() throw(CException) {
	S7CUAPP_ "unitStart";
}


//intervalled scheduled method
void S7ControlUnit::unitRun() throw(CException) {
	
	double tmpResult = 0.0;
	cu_driver::DrvMsg message;
	memset(&message, 0, sizeof(cu_driver::DrvMsg));
	
	message.opcode = cu_driver::siemens_s7::OP_GET_DOUBLE;
	message.resultData = &tmpResult;
	
	for (int idx = 0;idx < plc_tracked_variable.size(); idx++) {
		//prepare message for sync request to the driver
		
		//let the inputData point to the second field of the struct
		message.inputData = ((char *)&plc_tracked_variable[idx]) + sizeof(int);
		plc_s7_accessor->send(&message);
		S7CUAPP_ "value read =" << std::setprecision(20) << tmpResult;
	}
	
}

// Abstract method for the stop of the control unit
void S7ControlUnit::unitStop() throw(CException) {
	S7CUAPP_ "unitStop";
}

// Abstract method for the deinit of the control unit
void S7ControlUnit::unitDeinit() throw(CException) {
	S7CUAPP_ "unitDeinit";
}

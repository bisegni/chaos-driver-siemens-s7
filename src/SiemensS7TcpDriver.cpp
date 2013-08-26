/*
 *	SiemensS7TcpDriver.h
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
#include "SiemensS7TcpDriver.h"

#include <string>

#include <chaos/cu_toolkit/driver_manager/driver/AbstractDriverPlugin.h>

#include <boost/regex.hpp>

namespace cu_driver = chaos::cu::driver_manager::driver;

#define SL7DRVLAPP_		LAPP_ << "[SiemensS7TcpDriver] "
#define SL7DRVLDBG_		LDBG_ << "[SiemensS7TcpDriver] "
#define SL7DRVLERR_		LERR_ << "[SiemensS7TcpDriver] "


//! Regular expression for check server hostname and port
static const boost::regex PlcHostNameAndPort("([a-zA-Z0-9]+(.[a-zA-Z0-9]+)+):([0-9]{3,5})");
//! Regular expression for check server ip and port
static const boost::regex PlcIpAnPort("(\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b):([0-9]{4,5})");


//GET_PLUGIN_CLASS_DEFINITION
//we need only to define the driver because we don't are makeing a plugin
OPEN_CU_DRIVER_PLUGIN_CLASS_DEFINITION(SiemensS7TcpDriver, 1.0.0, chaos::cu::driver_manager::driver::siemens_s7::SiemensS7TcpDriver)
REGISTER_CU_DRIVER_PLUGIN_CLASS_INIT_ATTRIBUTE(SiemensS7TcpDriver,http_address/dnsname:port)
CLOSE_CU_DRIVER_PLUGIN_CLASS_DEFINITION

//register the two plugin
OPEN_REGISTER_PLUGIN
REGISTER_PLUGIN(chaos::cu::driver_manager::driver::siemens_s7::SiemensS7TcpDriver)
CLOSE_REGISTER_PLUGIN

//default constructor definition
DEFAULT_CU_DRIVER_PLUGIN_CONSTRUCTOR_WITH_NS(chaos::cu::driver_manager::driver::siemens_s7, SiemensS7TcpDriver) {
	
}

//default descrutcor
cu_driver::siemens_s7::SiemensS7TcpDriver::~SiemensS7TcpDriver() {
	
}

void cu_driver::siemens_s7::SiemensS7TcpDriver::driverInit(const char *initParameter) throw(chaos::CException) {
	SL7DRVLAPP_ << "Init siemens s7 plc driver";
	//check the input parameter
	boost::smatch match;
	std::string inputStr = initParameter;
	bool isIpAndPort		= regex_match(inputStr, match, PlcIpAnPort, boost::match_extra);
	bool isHostnameAndPort  = isIpAndPort ? false:regex_match(inputStr, match, PlcHostNameAndPort, boost::match_extra);
	
	if(!isIpAndPort && !isHostnameAndPort) {
		SL7DRVLERR_ << "The address " << inputStr << " is not well formed";
		throw new chaos::CException(1, "the initialization paramter for the siemens sl7 is not well formed", "SiemensS7TcpDriver::driverInit");
	}
	
	std::string address = match[1];
	std::string port = match[(int)(match.size()-1)];
	
	SL7DRVLAPP_ << "using address " << address << " and port " << port;
	
	
	fds.rfd=openSocket(std::atoi(port.c_str()), address.c_str());
    fds.wfd=fds.rfd;
    
    if (fds.rfd>0) {
		di = daveNewInterface(fds,"IF1",0, daveProtoISOTCP, daveSpeed187k);
		if(di) daveSetTimeout(di,10000000);
		
		dc = di ? daveNewConnection(di,2,0,0):NULL;  // insert your rack and slot here
		
		if(di && dc && (daveInitAdapter(di) == 0) && (daveConnectPLC(dc) == 0)) {
			
		} else {
			if(dc) daveDisconnectPLC(dc);
			if(fds.rfd)closeSocket(fds.rfd);
			SL7DRVLERR_ << "Error opening address";
			throw new chaos::CException(2, "Error opening address", "SiemensS7TcpDriver::driverInit");
		}
	} else {
		SL7DRVLERR_ << "Error opening address";
		throw new chaos::CException(3, "Error opening address", "SiemensS7TcpDriver::driverInit");
	}
}

void cu_driver::siemens_s7::SiemensS7TcpDriver::driverDeinit() throw(chaos::CException) {
	SL7DRVLAPP_ << "Deinit siemens s7 plc driver";
	if(dc) {
		daveDisconnectPLC(dc);
		dc = NULL;
	}
	if(fds.rfd) {
		closeSocket(fds.rfd);
		fds.rfd = fds.wfd = 0;
	}

}

cu_driver::MsgManagmentResultType::MsgManagmentResult cu_driver::siemens_s7::SiemensS7TcpDriver::getDouble(PlcVariable& variable_info, void *mem_for_result) {
	int res = 0;
	double *usr_doub_ptr = static_cast<double*>(mem_for_result);
	res=daveReadBytes(dc, daveDB, variable_info.db_num, variable_info.start, variable_info.byte_count, NULL);
	if(0==res) {
		*usr_doub_ptr = static_cast<double>(daveGetFloat(dc));
		return cu_driver::MsgManagmentResultType::MMR_EXECUTED;
	} else {
		return cu_driver::MsgManagmentResultType::MMR_ERROR;
	}
	
}
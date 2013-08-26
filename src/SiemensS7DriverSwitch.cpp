/*
 *	SiemensS7DriverSwitch.cpp
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

//! Execute a command
cu_driver::MsgManagmentResultType::MsgManagmentResult cu_driver::siemens_s7::SiemensS7TcpDriver::execOpcode(cu_driver::DrvMsgPtr cmd) {
	cu_driver::MsgManagmentResultType::MsgManagmentResult result = cu_driver::MsgManagmentResultType::MMR_EXECUTED;
	switch (cmd->opcode) {
		case OP_GET_DOUBLE:
			//cmd->data point to the paramter of the method
			PlcVariable *variable_info = static_cast<PlcVariable*>(cmd->inputData);
			result = SiemensS7TcpDriver::getDouble(*variable_info, cmd->resultData);
			break;
	}
	return result;
}
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
#ifndef __ControlUnitTest__TestDeviceDriver__
#define __ControlUnitTest__TestDeviceDriver__

#include "SiemensS7TcpDriverTypes.h"
#include "s7lib/nodave.h"
#include "s7lib/openSocket.h"

#include <chaos/cu_toolkit/driver_manager/driver/AbstractDriverPlugin.h>

//this need to be out the nasmespace
DEFINE_CU_DRIVER_DEFINITION_PROTOTYPE(SiemensS7TcpDriver)

namespace chaos {
	namespace cu {
		namespace driver_manager {
			namespace driver {
				namespace siemens_s7 {
					//namespace cu_driver = chaos::cu::driver_manager::driver;
					
					/*
					 driver definition
					 */
					class SiemensS7TcpDriver: ADD_CU_DRIVER_PLUGIN_SUPERCLASS {
						
						//conenction information
						daveInterface *di;
						daveConnection *dc;
						daveFileDescriptors fds;
						
						inline driver::MsgManagmentResultType::MsgManagmentResult initPLCConnection(driver::DrvMsgPtr initPack);
						inline driver::MsgManagmentResultType::MsgManagmentResult deinitPLCConnection();
						
						void driverInit(const char *initParameter) throw(chaos::CException);
						void driverDeinit() throw(chaos::CException);
						
					protected:
						//! Return a variable of the type boolean
						/*!
						 */
						driver::MsgManagmentResultType::MsgManagmentResult getDouble(PlcVariable& variable_info, void *mem_for_result);
						
					public:
						SiemensS7TcpDriver();
						~SiemensS7TcpDriver();
						//! Execute a command
						driver::MsgManagmentResultType::MsgManagmentResult execOpcode(driver::DrvMsgPtr cmd);
					};
				}
			}
		}
	}
}

#endif /* defined(__ControlUnitTest__SiemensS7TcpDriver__) */

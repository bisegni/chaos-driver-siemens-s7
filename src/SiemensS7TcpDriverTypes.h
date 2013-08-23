/*
 *	SiemensS7TcpDriverTypes.h
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
#ifndef SiemensS7ControlUnit_SiemensS7TcpDriverTypes_h
#define SiemensS7ControlUnit_SiemensS7TcpDriverTypes_h

#include <chaos/cu_toolkit/driver_manager/driver/DriverTypes.h>

namespace chaos {
	namespace cu {
		namespace driver_manager {
			namespace driver {
				namespace siemens_s7 {
					
					typedef enum {
						OP_GET_DOUBLE = OpcodeType::OP_USER
					} SiemensS7TcpDriverOpecode;
					
					typedef struct {
						int db_num;
						int start;
						int byte_count;
						int max_len;
					} t_variable_struct;
				}
			}
		}
	}
}
#endif

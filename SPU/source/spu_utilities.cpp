#include "spu_utilities.hpp"

const char* SPUErrorsMessenger(SPUStatusCode status) {
	switch (status) {
		case SPU_NO_ERROR: 				return "SPU ERROR - NO ERROR";

		case SPU_NO_FILE_TO_RUN:		return "SPU ERROR - NO FILE TO RUN";
		case SPU_FILE_OPEN_ERROR:		return "SPU ERROR - FILE WAS NOT OPENED";
		case SPU_FILE_CLOSE_ERROR:		return "SPU ERROR - FILE WAS NOT CLOSED";
		case SPU_FILE_READ_ERROR:		return "SPU ERROR - FILE READ ERROR";

		case SPU_ALLOC_ERROR:			return "SPU ERROR - MEMORY ALLOCATION ERROR";

		case SPU_COMMAND_ERROR:			return "SPU ERROR - UNKNOWN COMMAND FOR PROCESSOR";
	}
}

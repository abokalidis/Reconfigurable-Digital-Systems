#include <hls_stream.h>
#include <ap_int.h>
#include <iostream>
#include <stdint.h>

using namespace hls;

#include "my_ip_hls.hpp"


#define STREAM_TEST_ITERATIONS 10


int main() {

	uint32 i = 0;
	uint32 j = 0;
	uint32 rule0 = 10;
	uint32 rule1 = 30;
	uint32 rule2 = 50;

	stream<axiWord> slaveIn("slaveIn");
	stream<axiWord> masterOut("masterOut");

	rulescnts counters ={0,0,0};

    for(j=0;j<3;j++){
	for (i=0;i<STREAM_TEST_ITERATIONS;i++) {
		axiWord dataIn = {0,0,0};
		dataIn.data = i*10;
		dataIn.strb = 0b1111;
		if (i == STREAM_TEST_ITERATIONS-1)
			dataIn.last = 1;
		else
			dataIn.last = 0;
		slaveIn.write(dataIn);

		my_ip_hls(slaveIn, masterOut, rule0, rule1, rule2, counters);

		if (!masterOut.empty()) {
			axiWord dataOut = {0,0,0};
			masterOut.read(dataOut);
			printf("%d: read data: %u\n",(int)i, (int)dataOut.data);
		}
		else {
			printf("%d: no data available!\n",(int)i);
		}
	}
    }
	printf("\nrule0cnt : %d rule1cnt : %d rule2cnt : %d\n ", (int)counters.rule0cnt,(int)counters.rule1cnt,(int)counters.rule2cnt);
    printf("\n\n\n");

	return 0;
}

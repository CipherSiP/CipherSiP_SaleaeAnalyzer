#ifndef CAN_SIMULATION_DATA_GENERATOR
#define CAN_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>

class CspCanBitAnalyzerSettings;

class CspCanBitSimulationDataGenerator
{
public:
	CspCanBitSimulationDataGenerator();
	~CspCanBitSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, CspCanBitAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	CspCanBitAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;



protected:  //functions
	U16 ComputeCrc( std::vector<BitState>& bits, U32 num_bits );
	void AddCrc();
	void CreateDataOrRemoteFrame( U32 identifier, bool use_extended_frame_format, bool remote_frame, std::vector<U8>& data, bool get_ack_in_response );
	void WriteFrame( bool error = false );

protected: //vars
	ClockGenerator mClockGenerator;
	SimulationChannelDescriptor mCanSimulationData;  //if we had more than one channel to simulate, they would need to be in an array

	U8 mValue;

	std::vector<BitState> mFakeStartOfFrameField;
	std::vector<BitState> mFakeArbitrationField;
	std::vector<BitState> mFakeControlField;
	std::vector<BitState> mFakeDataField;
	std::vector<BitState> mFakeCrcFieldWithoutDelimiter;
	std::vector<BitState> mFakeAckField;
	std::vector<BitState> mFakeEndOfFrame;
	std::vector<BitState> mFakeStuffedBits;
	std::vector<BitState> mFakeFixedFormBits;
};
#endif //CSPCANBIT_SIMULATION_DATA_GENERATOR
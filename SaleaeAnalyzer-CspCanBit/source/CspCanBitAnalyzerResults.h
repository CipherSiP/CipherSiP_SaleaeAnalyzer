#ifndef CSPCANBIT_ANALYZER_RESULTS
#define CSPCANBIT_ANALYZER_RESULTS

#include <AnalyzerResults.h>

enum CanFrameType { IdentifierField, IdentifierFieldEx, ControlField, CalibrationField, DataField, CrcField, AckField, CanError };
#define REMOTE_FRAME ( 1 << 0 )

//#define FRAMING_ERROR_FLAG ( 1 << 0 )
//#define PARITY_ERROR_FLAG ( 1 << 1 )

class CspCanBitAnalyzer;
class CspCanBitAnalyzerSettings;

class CspCanBitAnalyzerResults : public AnalyzerResults
{
public:
	CspCanBitAnalyzerResults( CspCanBitAnalyzer* analyzer, CspCanBitAnalyzerSettings* settings );
	virtual ~CspCanBitAnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	CspCanBitAnalyzerSettings* mSettings;
	CspCanBitAnalyzer*         mAnalyzer;
};

#endif //CSPCANBIT_ANALYZER_RESULTS

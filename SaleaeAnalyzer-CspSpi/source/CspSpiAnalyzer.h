#ifndef CSPSPI_ANALYZER_H
#define CSPSPI_ANALYZER_H

#include <Analyzer.h>
#include "CspSpiAnalyzerResults.h"
#include "CspSpiSimulationDataGenerator.h"

class CspSpiAnalyzerSettings;
class ANALYZER_EXPORT CspSpiAnalyzer : public Analyzer2
{
public:
	CspSpiAnalyzer();
	virtual ~CspSpiAnalyzer();
	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //functions
	void Setup();
	void AdvanceToActiveEnableEdge();
	bool IsInitialClockPolarityCorrect();
	void AdvanceToActiveEnableEdgeWithCorrectClockPolarity();
	bool WouldAdvancingTheClockToggleEnable();
	void GetWord();

#pragma warning( push )
#pragma warning( disable : 4251 ) //warning C4251: 'SerialAnalyzer::<...>' : class <...> needs to have dll-interface to be used by clients of class
protected:  //vars
	std::auto_ptr< CspSpiAnalyzerSettings > mSettings;
	std::auto_ptr< CspSpiAnalyzerResults > mResults;
	bool mSimulationInitilized;
	CspSpiSimulationDataGenerator mSimulationDataGenerator;

	AnalyzerChannelData* mMosi; 
	AnalyzerChannelData* mMiso;
	AnalyzerChannelData* mClock;
	AnalyzerChannelData* mEnable;

	U64 mCurrentSample;
	AnalyzerResults::MarkerType mArrowMarker;
	std::vector<U64> mArrowLocations;

	U16 mFrameNum;

#pragma warning( pop )
};

#define FRAME_HEADER  0x01
#define FRAME_DATA    0x02
#define FRAME_INDEX_BIT_OFFSET 16
#define SET_FRAME_DATA(index, data) (data | (index<<FRAME_INDEX_BIT_OFFSET))

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //CSPSPI_ANALYZER_H

#ifndef CSPCANBIT_ANALYZER_SETTINGS
#define CSPCANBIT_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

//#define RECESSIVE BIT_HIGH
//#define DOMINANT BIT_LOW

class CspCanBitAnalyzerSettings : public AnalyzerSettings
{
public:
	CspCanBitAnalyzerSettings();
	virtual ~CspCanBitAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	void UpdateInterfacesFromSettings();

	Channel mCspCanChannel;	
	U32 mBitRate;
	bool mInverted;

	Channel mCanChannel;
	U32     mTxClkFreq;
	U32     mGranularity;
	U32     mBitsPerSymbel;


	BitState Recessive();
	BitState Dominant();

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel > mCanChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger > mBitRateInterface;
	std::auto_ptr< AnalyzerSettingInterfaceBool    > mCanChannelInvertedInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel > mCspCanChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger > mTxClkFreqInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger > mGranularityInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger > mBitsPerSymbelInterface;

};

#endif //CSPCANBIT_ANALYZER_SETTINGS

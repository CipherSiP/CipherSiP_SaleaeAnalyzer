#include "CspCanBitAnalyzerSettings.h"
#include <AnalyzerHelpers.h>
#include <sstream>
#include <cstring>

CspCanBitAnalyzerSettings::CspCanBitAnalyzerSettings()
:	mCanChannel ( UNDEFINED_CHANNEL ),
	mBitRate ( 1000000 ),
	mInverted( false ),
	mCspCanChannel( UNDEFINED_CHANNEL ),
	mTxClkFreq(192000000),
    mGranularity(7),
	mBitsPerSymbel(2)
{
	mCanChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mCanChannelInterface->SetTitleAndTooltip( "CAN", "Controller Area Network - Input" );
	mCanChannelInterface->SetChannel( mCanChannel );

	mBitRateInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mBitRateInterface->SetTitleAndTooltip( "Bit Rate (Bits/s)",  "Specify the bit rate in bits per second." );
    mBitRateInterface->SetMax( 25000000 );
	mBitRateInterface->SetMin( 10000 );
	mBitRateInterface->SetInteger( mBitRate );

	mCanChannelInvertedInterface.reset( new AnalyzerSettingInterfaceBool( ) );
	mCanChannelInvertedInterface->SetTitleAndTooltip( "Inverted (CAN High)", "Use this option when recording CAN High directly" );
	mCanChannelInvertedInterface->SetValue( mInverted );

	mCspCanChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mCspCanChannelInterface->SetTitleAndTooltip( "CSP_CAN", "CSP_CAN" );
	mCspCanChannelInterface->SetChannel( mCspCanChannel );
	
	mTxClkFreqInterface.reset(new AnalyzerSettingInterfaceInteger());
	mTxClkFreqInterface->SetTitleAndTooltip("Tx Clk", "FPGA Tx Clk (Hz)");
	mTxClkFreqInterface->SetMax(600000000);
	mTxClkFreqInterface->SetMin(48000000);
	mTxClkFreqInterface->SetInteger(mTxClkFreq);

	mGranularityInterface.reset(new AnalyzerSettingInterfaceInteger());
	mGranularityInterface->SetTitleAndTooltip("CSP Granularity", "CSP Granularity");
	mGranularityInterface->SetMax(16);
	mGranularityInterface->SetMin(2);
	mGranularityInterface->SetInteger(mGranularity);

	mBitsPerSymbelInterface.reset(new AnalyzerSettingInterfaceInteger());
	mBitsPerSymbelInterface->SetTitleAndTooltip("CSP Bits Per Symbel", "CSP Bits Per Symbel");
	mBitsPerSymbelInterface->SetMax(4);
	mBitsPerSymbelInterface->SetMin(2);
	mBitsPerSymbelInterface->SetInteger(mBitsPerSymbel);

	AddInterface( mCanChannelInterface.get() );
	AddInterface( mBitRateInterface.get() );
	AddInterface( mCanChannelInvertedInterface.get() );
	AddInterface( mCspCanChannelInterface.get() );
	AddInterface( mTxClkFreqInterface.get());
	AddInterface( mGranularityInterface.get());
	AddInterface( mBitsPerSymbelInterface.get());

	//AddExportOption( 0, "Export as text/csv file", "text (*.txt);;csv (*.csv)" );
	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mCanChannel, "CAN", false );
	AddChannel( mCspCanChannel, "CSP_CAN", false );
}

CspCanBitAnalyzerSettings::~CspCanBitAnalyzerSettings()
{
}

bool CspCanBitAnalyzerSettings::SetSettingsFromInterfaces()
{
	Channel can_channel = mCanChannelInterface->GetChannel();
	Channel CspCan_channel = mCspCanChannelInterface->GetChannel();
	
	if( can_channel == UNDEFINED_CHANNEL )
	{
		SetErrorText( "Please select a channel for the CAN interface" );
		return false;
	}
	if( CspCan_channel == UNDEFINED_CHANNEL )
	{
		SetErrorText( "Please select a channel for the CSP_CAN interface" );
		return false;
	}
	mCanChannel    = can_channel;
	mBitRate       = mBitRateInterface->GetInteger();
	mInverted      = mCanChannelInvertedInterface->GetValue();

	mCspCanChannel = CspCan_channel;
	mTxClkFreq         = mTxClkFreqInterface->GetInteger();
	mGranularity   = mGranularityInterface->GetInteger();
	mBitsPerSymbel = mBitsPerSymbelInterface->GetInteger();

	ClearChannels();
	AddChannel( mCanChannel, "CAN", true );
	AddChannel( mCspCanChannel, "CSP_CAN", true );

	return true;
}

void CspCanBitAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	const char* name_string;	//the first thing in the archive is the name of the protocol analyzer that the data belongs to.
	text_archive >> &name_string;
	if( strcmp( name_string, "CspCANAnalyzer" ) != 0 )
		AnalyzerHelpers::Assert( "SaleaeCspCanBitAnalyzer: Provided with a settings string that doesn't belong to us;" );

	text_archive >> mCanChannel;
	text_archive >> mBitRate;
	text_archive >> mInverted; //SimpleArchive catches exception and returns false if it fails.
	text_archive >> mCspCanChannel;
	text_archive >> mTxClkFreq;
	text_archive >> mGranularity;
	text_archive >> mBitsPerSymbel;

	ClearChannels();
	AddChannel( mCanChannel, "CAN", true );
	AddChannel( mCspCanChannel, "CSP_CAN", true );

	UpdateInterfacesFromSettings();
}

const char* CspCanBitAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << "CspCANAnalyzer";
	text_archive << mCanChannel;
	text_archive << mBitRate;
	text_archive << mInverted; 
	text_archive << mCspCanChannel;
	text_archive << mTxClkFreq;
	text_archive << mGranularity;
	text_archive << mBitsPerSymbel;

	return SetReturnString( text_archive.GetString() );
}

void CspCanBitAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mCanChannelInterface->SetChannel( mCanChannel );
	mBitRateInterface->SetInteger( mBitRate );
	mCanChannelInvertedInterface->SetValue( mInverted );
	mCspCanChannelInterface->SetChannel( mCspCanChannel );
	mTxClkFreqInterface->SetInteger(mTxClkFreq);
	mGranularityInterface->SetInteger(mGranularity);
	mBitsPerSymbelInterface->SetInteger(mBitsPerSymbel);
}

BitState CspCanBitAnalyzerSettings::Recessive()
{
	if( mInverted )
		return BIT_LOW;
	return BIT_HIGH;
}
BitState CspCanBitAnalyzerSettings::Dominant()
{
	if( mInverted )
		return BIT_HIGH;
	return BIT_LOW;
}

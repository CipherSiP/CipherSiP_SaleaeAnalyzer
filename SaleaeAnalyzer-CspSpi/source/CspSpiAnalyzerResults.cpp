#include "CspSpiAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "CspSpiAnalyzer.h"
#include "CspSpiAnalyzerSettings.h"
#include <iostream>
#include <sstream>
#include <algorithm>

#pragma warning(disable: 4996) //warning C4996: 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead.

CspSpiAnalyzerResults::CspSpiAnalyzerResults( CspSpiAnalyzer* analyzer, CspSpiAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
	mIsCurrWrite = false;
	mCurrentReg  = FPGA_ICD_NO_REG;

	mListOfRegisters = {{ FPGA_ICD_INT_STATUS_REG      , "INT_STATUS"}, 		
						{ FPGA_ICD_INT_MASK_REG        , "INT_MASK"},   		
						{ FPGA_ICD_TX_ERROR            , "TX_ERROR" },
						{ FPGA_ICD_RX_ERROR            , "RX_ERROR" },
						{ FPGA_ICD_TX_OCCUPIED         , "TX_OCCUPIED"},
						{ FPGA_ICD_TX_XFER_INIT        , "TX_XFER_INIT"},
						{ FPGA_ICD_TX_XFER_CMPLT       , "TX_XFER_CMPLT"},
						{ FPGA_ICD_TX_XFER_ABORT       , "TX_XFER_ABORT"},
						{ FPGA_ICD_TX_CHUNK_XFER       , "TX_CHUNK_XFER"},
						{ FPGA_ICD_RX_OCCUPIED         , "RX_OCCUPIED"},
						{ FPGA_ICD_RX_XFER_INIT        , "RX_XFER_INIT"},
						{ FPGA_ICD_RX_XFER_CMPLT       , "RX_XFER_CMPLT"},
						{ FPGA_ICD_RX_XFER_ABORT       , "RX_XFER_ABORT"},
						{ FPGA_ICD_RX_CHUNK_XFER       , "RX_CHUNK_XFER"},
						{ FPGA_ICD_RX_UNEXPECTED_START , "RX_UNEXPECTED_START"},
						{ FPGA_ICD_RX_UNEXPECTED_STOP  , "RX_UNEXPECTED_STOP"},
						{ FPGA_ICD_RX_UNEXPECTED_DATA  , "RX_UNEXPECTED_DATA"},
						{ FPGA_ICD_TX_FIFO_UR          , "TX_FIFO_UR"},
						{ FPGA_ICD_TX_FIFO_OR          , "TX_FIFO_OR"},
						{ FPGA_ICD_TX_FIFO_AE          , "TX_FIFO_AE"},
						{ FPGA_ICD_TX_FIFO_AF          , "TX_FIFO_AF"},
						{ FPGA_ICD_RX_FIFO_OR          , "RX_FIFO_OR"},
						{ FPGA_ICD_RX_FIFO_UR          , "RX_FIFO_UR"},
						{ FPGA_ICD_RX_FIFO_AE          , "RX_FIFO_AE"},
						{ FPGA_ICD_RX_FIFO_AF          , "RX_FIFO_AF"},
						{ FPGA_ICD_CMD_REG             , "CMD"},		
						{ FPGA_ICD_CSP_PARAMS_REG      , "CSP_PARAMS"},		
						{ FPGA_ICD_DEVICE_PARAMS_REG   , "DEVICE_PARAMS"}, 	
						{ FPGA_ICD_CAN_IF_REG          , "CAN_IF"},		
						{ FPGA_ICD_TX_STREAM_PARAMS_REG, "TX_STREAM_PARAMS"}, 
						{ FPGA_ICD_RX_STREAM_PARAMS_REG, "RX_STREAM_PARAMS"},
						{ FPGA_ICD_CSP_CTRL_REG        , "CSP_CTRL"}, 		
						{ FPGA_ICD_FIFO_STATUS_REG     , "FIFO_STATUS"}, 		
						{ FPGA_ICD_TX_FIFO_REG         , "TX_FIFO"}, 		
						{ FPGA_ICD_RX_FIFO_REG         , "RX_FIFO"},  		
						{ FPGA_ICD_DUMMY_REG           , "DUMMY"},  		
						{ FPGA_ICD_FPGA_VERSION_REG    , "FPGA_VERSION"},
						{ FPGA_ICD_BUILD_DATE_REG      , "BUILD_DATE"},		
						{ FPGA_ICD_BUILD_TIME_REG      , "BUILD_TIME"} };
}

CspSpiAnalyzerResults::~CspSpiAnalyzerResults()
{
}

void CspSpiAnalyzerResults::ParseHeader(U8 Data, char result_str[])
{
	mIsCurrWrite = (Data & 0x80) ? true : false;
	Data &= 0x7F;

	auto itr = std::find_if(mListOfRegisters.cbegin(), mListOfRegisters.cend(), [&Data](const REG_T& s) {
		return s.reg_address == Data; });

	if (itr != mListOfRegisters.cend())
	{
		mCurrentReg = itr->reg_address;
		strcat(result_str, (mIsCurrWrite ? "W - " : "R - "));
		strcat(result_str, itr->text);
	}
	else
	{
		mCurrentReg = FPGA_ICD_NO_REG;
		strcat(result_str, "Unknowen register");
	}
}
void CspSpiAnalyzerResults::ParseIntStatusReg(U8 data, U8 dataIndex, char result_str[])
{
	switch (dataIndex)
	{
		case 0:
		{
			data&INT_STAT_DEVICE_BUSY_MASK ?  strcat (result_str,"DEVICE_BUSY ") : 0;
			data&INT_STAT_DEVICE_READY_MASK ?  strcat (result_str,"DEVICE_READY ") : 0;
			data&INT_STAT_TX_ERROR_MASK ?  strcat (result_str,"TX_ERROR ") : 0;
			data&INT_STAT_RX_ERROR_MASK ?  strcat (result_str,"RX_ERROR ") : 0;
		}
		break;
		case 1:
		{
			data&INT_STAT_TX_OCCUPIED_MASK ?  strcat (result_str,"TX_OCCUPIED ") : 0;
			data&INT_STAT_TX_XFER_INIT_MASK ?  strcat (result_str,"TX_XFER_INIT ") : 0;
			data&INT_STAT_TX_XFER_CMPLT_MASK ?  strcat (result_str,"TX_XFER_CMPLT ") : 0;
			data&INT_STAT_TX_XFER_ABORT_MASK ?  strcat (result_str,"TX_XFER_ABORT ") : 0;
			data&INT_STAT_TX_CHUNK_XFER_MASK ?  strcat (result_str,"TX_CHUNK_XFER ") : 0;
		}
		break;
		case 2:
		{
			data&INT_STAT_RX_OCCUPIED_MASK ?  strcat (result_str,"RX_OCCUPIED ") : 0;
			data&INT_STAT_RX_XFER_INIT_MASK ?  strcat (result_str,"RX_XFER_INIT ") : 0;
			data&INT_STAT_RX_XFER_CMPLT_MASK ?  strcat (result_str,"RX_XFER_CMPLT ") : 0;
			data&INT_STAT_RX_XFER_ABORT_MASK ?  strcat (result_str,"RX_XFER_ABORT ") : 0;
			data&INT_STAT_RX_CHUNK_XFER_MASK ?  strcat (result_str,"RX_CHUNK_XFER ") : 0;
			data&INT_STAT_RX_UNEXPECTED_START_MASK ?  strcat (result_str,"RX_UNEXPECTED_START ") : 0;
			data&INT_STAT_RX_UNEXPECTED_STOP_MASK ?  strcat (result_str,"RX_UNEXPECTED_STOP ") : 0;
			data&INT_STAT_RX_UNEXPECTED_DATA_MASK ?  strcat (result_str,"RX_UNEXPECTED_DATA ") : 0;
		}
		break;
		case 3:
		{
			data&INT_STAT_TX_FIFO_UR_MASK ? strcat(result_str, "TX_FIFO_UR ") : 0;
			data&INT_STAT_TX_FIFO_OR_MASK ? strcat(result_str, "TX_FIFO_OR ") : 0;
			data&INT_STAT_TX_FIFO_AE_MASK ? strcat(result_str, "TX_FIFO_AE ") : 0;
			data&INT_STAT_TX_FIFO_AF_MASK ? strcat(result_str, "TX_FIFO_AF ") : 0;
			data&INT_STAT_RX_FIFO_OR_MASK ? strcat(result_str, "RX_FIFO_OR ") : 0;
			data&INT_STAT_RX_FIFO_UR_MASK ? strcat(result_str, "RX_FIFO_UR ") : 0;
			data&INT_STAT_RX_FIFO_AE_MASK ? strcat(result_str, "RX_FIFO_AE ") : 0;
			data&INT_STAT_RX_FIFO_AF_MASK ? strcat(result_str, "RX_FIFO_AF ") : 0;
		}
		break;
		default:
			strcat(result_str, "Error Status reg dataIndex ");
			break;

	}

	/*if(strlen(result_str) == 0)
	{
		strcat(result_str, "None");
	}*/
}

void CspSpiAnalyzerResults::ParseData(U8 data, U8 dataIndex, char result_str[])
{
	switch (mCurrentReg)
	{
		case FPGA_ICD_INT_STATUS_REG:
		{
			ParseIntStatusReg(data, dataIndex, result_str);
		}
		break;
		case FPGA_ICD_INT_MASK_REG:
		case FPGA_ICD_TX_ERROR:
		case FPGA_ICD_RX_ERROR:
		case FPGA_ICD_TX_OCCUPIED:
		case FPGA_ICD_TX_XFER_INIT:
		case FPGA_ICD_TX_XFER_CMPLT:
		case FPGA_ICD_TX_XFER_ABORT:
		case FPGA_ICD_TX_CHUNK_XFER:
		case FPGA_ICD_RX_OCCUPIED:
		case FPGA_ICD_RX_XFER_INIT:
		case FPGA_ICD_RX_XFER_CMPLT:
		case FPGA_ICD_RX_XFER_ABORT:
		case FPGA_ICD_RX_CHUNK_XFER:
		case FPGA_ICD_RX_UNEXPECTED_START:
		case FPGA_ICD_RX_UNEXPECTED_STOP:
		case FPGA_ICD_RX_UNEXPECTED_DATA:
		case FPGA_ICD_TX_FIFO_UR:
		case FPGA_ICD_TX_FIFO_OR:
		case FPGA_ICD_TX_FIFO_AE:
		case FPGA_ICD_TX_FIFO_AF:
		case FPGA_ICD_RX_FIFO_OR:
		case FPGA_ICD_RX_FIFO_UR:
		case FPGA_ICD_RX_FIFO_AE:
		case FPGA_ICD_RX_FIFO_AF:
		case FPGA_ICD_CMD_REG:
		case FPGA_ICD_CSP_PARAMS_REG:
		case FPGA_ICD_DEVICE_PARAMS_REG:
		case FPGA_ICD_CAN_IF_REG:
		case FPGA_ICD_TX_STREAM_PARAMS_REG:
		case FPGA_ICD_RX_STREAM_PARAMS_REG:
		case FPGA_ICD_CSP_CTRL_REG:
		case FPGA_ICD_FIFO_STATUS_REG:
		case FPGA_ICD_TX_FIFO_REG:
		case FPGA_ICD_RX_FIFO_REG:
		case FPGA_ICD_DUMMY_REG:
		case FPGA_ICD_FPGA_VERSION_REG:
		case FPGA_ICD_BUILD_DATE_REG:
		case FPGA_ICD_BUILD_TIME_REG:
		//break;
		default:
			AnalyzerHelpers::GetNumberString(data, DisplayBase::Hexadecimal, mSettings->mBitsPerTransfer, result_str, 128);
		break;
	}

}

void CspSpiAnalyzerResults::handleMosiFrame(Frame& frame, char result_str[])
{
	U16 data1 = frame.mData1 & FRAME_DATA_MASK;
	U16 frameIndex = (frame.mData1 & FRAME_INDEX_MASK) >> FRAME_INDEX_BIT_OFFSET;

	if (frame.mType == FRAME_HEADER)
	{
		ParseHeader(data1, result_str);
	}
	else if (frame.mType == FRAME_DATA)
	{
		if (mIsCurrWrite)
		{
			ParseData(data1, frameIndex - 1, result_str);
		}
		else
		{
			//AnalyzerHelpers::GetNumberString(data1, DisplayBase::Hexadecimal, mSettings->mBitsPerTransfer, result_str, 128);
			//AddResultString(number_str);
			sprintf(result_str, "Mosi D R");
		}
	}
	else
	{
		sprintf(result_str, "Error Frame type");
	}
}

void CspSpiAnalyzerResults::handleMisoFrame(Frame& frame, char result_str[])
{
	U16 data1 = frame.mData1 & FRAME_DATA_MASK;
	U16 data2 = frame.mData2 & FRAME_DATA_MASK;

	U16 frameIndex = (frame.mData2 & FRAME_INDEX_MASK) >> FRAME_INDEX_BIT_OFFSET;

	if (frame.mType == FRAME_HEADER)
	{
		ParseHeader(data1, result_str);
	}
	else if (frame.mType == FRAME_DATA)
	{
		if (mIsCurrWrite)
		{
			AnalyzerHelpers::GetNumberString(data2, DisplayBase::Hexadecimal, mSettings->mBitsPerTransfer, result_str, 128);
			sprintf(result_str, "Miso D W");
		}
		else
		{
			ParseData(data2, frameIndex - 1, result_str);
		}
	}
	else
	{
		sprintf(result_str, "Error Frame type");
	}
}

void CspSpiAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )  //unrefereced vars commented out to remove warnings.
{
	char result_str[256] = "";

	ClearResultStrings();
	Frame frame = GetFrame( frame_index );

	if( ( frame.mFlags & SPI_ERROR_FLAG ) == 0 )
	{
		if( channel == mSettings->mMosiChannel )
		{
			handleMosiFrame(frame, result_str);
			AddResultString(result_str);

		}
		else
		{
			handleMisoFrame(frame, result_str);
			AddResultString(result_str);

		}
	}else
	{
			AddResultString( "Error" );
			AddResultString( "Settings mismatch" );
			AddResultString( "The initial (idle) state of the CLK line does not match the settings." );
	}
}

void CspSpiAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 /*export_type_user_id*/ )
{
	//export_type_user_id is only important if we have more than one export type.


	std::stringstream ss;
	void* f = AnalyzerHelpers::StartFile( file );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	ss << "Time [s],Packet ID,MOSI,MISO" << std::endl;

	bool mosi_used = true;
	bool miso_used = true;

	if( mSettings->mMosiChannel == UNDEFINED_CHANNEL )
		mosi_used = false;

	if( mSettings->mMisoChannel == UNDEFINED_CHANNEL )
		miso_used = false;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );

		if( ( frame.mFlags & SPI_ERROR_FLAG ) != 0 )
			continue;
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char mosi_str[128] = "";
		if( mosi_used == true )
			AnalyzerHelpers::GetNumberString( frame.mData1, display_base, mSettings->mBitsPerTransfer, mosi_str, 128 );

		char miso_str[128] = "";
		if( miso_used == true )
			AnalyzerHelpers::GetNumberString( frame.mData2, display_base, mSettings->mBitsPerTransfer, miso_str, 128 );

		U64 packet_id = GetPacketContainingFrameSequential( i ); 
		if( packet_id != INVALID_RESULT_INDEX )
			ss << time_str << "," << packet_id << "," << mosi_str << "," << miso_str << std::endl;
		else
			ss << time_str << ",," << mosi_str << "," << miso_str << std::endl;  //it's ok for a frame not to be included in a packet.
	
		AnalyzerHelpers::AppendToFile( (U8*)ss.str().c_str(), ss.str().length(), f );
		ss.str( std::string() );
							
		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			AnalyzerHelpers::EndFile( f );
			return;
		}
	}

	UpdateExportProgressAndCheckForCancel( num_frames, num_frames );
	AnalyzerHelpers::EndFile( f );
}

void CspSpiAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
    ClearTabularText();
	Frame frame = GetFrame( frame_index );

	bool mosi_used = true;
	bool miso_used = true;

	if( mSettings->mMosiChannel == UNDEFINED_CHANNEL )
		mosi_used = false;

	if( mSettings->mMisoChannel == UNDEFINED_CHANNEL )
		miso_used = false;

	char mosi_str[256] = "";
    char miso_str[256] = "";

    std::stringstream ss;

    if( ( frame.mFlags & SPI_ERROR_FLAG ) == 0 )
    {
		if (mosi_used == true)
		{			
			handleMosiFrame(frame, mosi_str);
		}
		if (miso_used == true)
		{
			handleMisoFrame(frame, miso_str);
		}

        if( mosi_used == true && miso_used == true )
        {
			if (frame.mType == FRAME_HEADER)
			{
				if(strlen(mosi_str) != 0)
					ss << "MOSI: " << mosi_str;
			}
			else if (frame.mType == FRAME_DATA)
			{
				if (mIsCurrWrite)
				{
					if (strlen(mosi_str) != 0)
						ss << "MOSI: " << mosi_str;
				}
				else
				{
					if (strlen(miso_str) != 0)
						ss << "MISO: " << miso_str;
				}
			}
			else
			{
				ss << "MOSI: " << mosi_str << ";  MISO: " << miso_str;
			}
        }else
        {
            if( mosi_used == true )
            {
                ss << "MOSI: " << mosi_str;
            }else
            {
                ss << "MISO: " << miso_str;
            }
        }
    }
    else
    {
        ss << "The initial (idle) state of the CLK line does not match the settings.";
    }

	AddTabularText( ss.str().c_str() );
}

void CspSpiAnalyzerResults::GeneratePacketTabularText( U64 /*packet_id*/, DisplayBase /*display_base*/ )  //unrefereced vars commented out to remove warnings.
{
	ClearResultStrings();
	AddResultString( "not supported" );
}

void CspSpiAnalyzerResults::GenerateTransactionTabularText( U64 /*transaction_id*/, DisplayBase /*display_base*/ )  //unrefereced vars commented out to remove warnings.
{
	ClearResultStrings();
	AddResultString( "not supported" );
}


#ifndef CSPSPI_ANALYZER_RESULTS
#define CSPSPI_ANALYZER_RESULTS

#include <AnalyzerResults.h>
#include <vector>
#include <iterator>

#define SPI_ERROR_FLAG ( 1 << 0 )

class CspSpiAnalyzer;
class CspSpiAnalyzerSettings;

class CspSpiAnalyzerResults : public AnalyzerResults
{
	typedef struct
	{
		U8   reg_address;
		char text[128];
	}REG_T;

public:
	CspSpiAnalyzerResults( CspSpiAnalyzer* analyzer, CspSpiAnalyzerSettings* settings );
	virtual ~CspSpiAnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions
	
protected:  //vars
	CspSpiAnalyzerSettings* mSettings;
	CspSpiAnalyzer* mAnalyzer;

	std::vector<REG_T> mListOfRegisters;
	U8   mCurrentReg;
	bool mIsCurrWrite;

	void handleMosiFrame(Frame& frame, char result_str[]);
	void handleMisoFrame(Frame& frame, char result_str[]);

	void ParseHeader(U8 Data, char buffer[]);
	void ParseData(U8 data, U8 dataIndex, char result_str[]);

	void ParseIntStatusReg(U8 data, U8 dataIndex, char result_str[]);
};

#define FRAME_DATA_MASK  0x0000FFFF
#define FRAME_INDEX_MASK 0xFFFF0000

#define FPGA_ICD_INT_STATUS_REG           0x01		// R: Interrupt status register
#define FPGA_ICD_INT_MASK_REG             0x05		// R/W: Interrupt mask register
#define FPGA_ICD_TX_ERROR                 0x0B
#define FPGA_ICD_RX_ERROR                 0x0C
#define FPGA_ICD_TX_OCCUPIED              0x0D
#define FPGA_ICD_TX_XFER_INIT             0x0E
#define FPGA_ICD_TX_XFER_CMPLT            0x0F
#define FPGA_ICD_TX_XFER_ABORT            0x10
#define FPGA_ICD_TX_CHUNK_XFER            0x11
#define FPGA_ICD_RX_OCCUPIED              0x12
#define FPGA_ICD_RX_XFER_INIT             0x13
#define FPGA_ICD_RX_XFER_CMPLT            0x14
#define FPGA_ICD_RX_XFER_ABORT            0x15
#define FPGA_ICD_RX_CHUNK_XFER            0x16
#define FPGA_ICD_RX_UNEXPECTED_START      0x17
#define FPGA_ICD_RX_UNEXPECTED_STOP       0x18
#define FPGA_ICD_RX_UNEXPECTED_DATA       0x19
#define FPGA_ICD_TX_FIFO_UR               0x1A
#define FPGA_ICD_TX_FIFO_OR               0x1B
#define FPGA_ICD_TX_FIFO_AE               0x1C
#define FPGA_ICD_TX_FIFO_AF               0x1D
#define FPGA_ICD_RX_FIFO_OR               0x1E
#define FPGA_ICD_RX_FIFO_UR               0x1F
#define FPGA_ICD_RX_FIFO_AE               0x20
#define FPGA_ICD_RX_FIFO_AF               0x21
#define FPGA_ICD_CMD_REG                  0x22		// W: Command register
#define FPGA_ICD_CSP_PARAMS_REG           0x26		// R/W: CSP parameter register
#define FPGA_ICD_DEVICE_PARAMS_REG        0x2A		// R/W: Device parameter register
#define FPGA_ICD_CAN_IF_REG               0x2E		// W: CAN core interface register
#define FPGA_ICD_TX_STREAM_PARAMS_REG     0x32		// R/W: Tx stream parameter register
#define FPGA_ICD_RX_STREAM_PARAMS_REG     0x42		// R: Rx stream parameter register
#define FPGA_ICD_CSP_CTRL_REG             0x52		// R/W: CSP control register
#define FPGA_ICD_FIFO_STATUS_REG          0x53		// R: FIFO status register
#define FPGA_ICD_TX_FIFO_REG              0x63		// W: Tx FIFO interface
#define FPGA_ICD_RX_FIFO_REG              0x67		// R: Rx FIFO interface
#define FPGA_ICD_DUMMY_REG                0x6B		// R/W: Dummy register (for sweat test)
#define FPGA_ICD_FPGA_VERSION_REG         0x6C		// R: FPGA version register
#define FPGA_ICD_BUILD_DATE_REG           0x6D		// R: FPGA build date register
#define FPGA_ICD_BUILD_TIME_REG           0x71
#define FPGA_ICD_NO_REG                   0xFF


/* INT_STATUS_REG - First byte*/
#define INT_STAT_DEVICE_BUSY_MASK			   0x01
#define INT_STAT_DEVICE_READY_MASK			   0x02
#define INT_STAT_TX_ERROR_MASK				   0x04
#define INT_STAT_RX_ERROR_MASK				   0x08
/* INT_STATUS_REG - Second byte*/
#define INT_STAT_TX_OCCUPIED_MASK			   0x01
#define INT_STAT_TX_XFER_INIT_MASK			   0x02
#define INT_STAT_TX_XFER_CMPLT_MASK		       0x04
#define INT_STAT_TX_XFER_ABORT_MASK		       0x08
#define INT_STAT_TX_CHUNK_XFER_MASK		       0x10
/* INT_STATUS_REG - Third byte*/
#define INT_STAT_RX_OCCUPIED_MASK			   0x01
#define INT_STAT_RX_XFER_INIT_MASK			   0x02
#define INT_STAT_RX_XFER_CMPLT_MASK		       0x04
#define INT_STAT_RX_XFER_ABORT_MASK			   0x08
#define INT_STAT_RX_CHUNK_XFER_MASK			   0x10
#define INT_STAT_RX_UNEXPECTED_START_MASK	   0x20
#define INT_STAT_RX_UNEXPECTED_STOP_MASK	   0x40
#define INT_STAT_RX_UNEXPECTED_DATA_MASK	   0x80
/* INT_STATUS_REG - Fourth byte*/
#define INT_STAT_TX_FIFO_UR_MASK			   0x01
#define INT_STAT_TX_FIFO_OR_MASK			   0x02
#define INT_STAT_TX_FIFO_AE_MASK			   0x04
#define INT_STAT_TX_FIFO_AF_MASK			   0x08
#define INT_STAT_RX_FIFO_OR_MASK			   0x10
#define INT_STAT_RX_FIFO_UR_MASK			   0x20
#define INT_STAT_RX_FIFO_AE_MASK			   0x40
#define INT_STAT_RX_FIFO_AF_MASK			   0x80
 
#endif //CSPSPI_ANALYZER_RESULTS
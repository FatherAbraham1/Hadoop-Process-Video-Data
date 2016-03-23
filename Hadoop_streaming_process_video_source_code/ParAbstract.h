/**
 *    file: ParAbstract.h
 *   brief: class of ParAbstract.
 *  author: xuhm
 * created: 2014.11.27
 */

#ifndef MEDIAPROCESSIF_PARABSTRACT_H
#define MEDIAPROCESSIF_PARABSTRACT_H

#include "CommonDef.h"
#include <fstream>

typedef struct STRUCT_HEADER_INFO
{
	UINT32 uiHdrLength;
	INT8 *pcHdrBuff;
	STRUCT_HEADER_INFO()
	{
		uiHdrLength = 0;
		pcHdrBuff = NULL;
	}
} STRUCT_HEADER_INFO ;

typedef struct chunk
{
	UINT32 m_chunkId;
	UINT32 m_startSampleId;
	UINT32 m_endSampleId;

	chunk()
	{
		m_chunkId = 0;
		m_startSampleId = 0;
		m_endSampleId = 0;
	}
} chunk;

typedef struct atom_stsc
{
	UINT32 m_first_chunk;
	UINT32 m_sample_per_chunk;
	UINT32 m_sample_dsc_index;

	atom_stsc()
	{
		m_first_chunk = 0;
		m_sample_per_chunk = 0;
		m_sample_dsc_index = 0;
	}
} atom_stsc;

typedef struct atom_stsd
{
	UINT32 iSize;
	INT8   acBuff[128];
	atom_stsd()
	{
		iSize = 0;
		memset(acBuff, 0, 128);
	}
} atom_stsd;

class stbl
{
public:

	stbl()
	{
		clearData();
	}

	virtual ~stbl()
	{

	}

	VOID clearData()
	{
		m_stss.clear();
		m_stsc.clear();
		m_stsz.clear();
		m_stco.clear();
	}

	VOID add_stss(UINT32 value)
	{
		m_stss.push_back(value);
		return;
	}

	VOID add_stsc(atom_stsc value)
	{
		m_stsc.push_back(value);
		return;
	}

	VOID add_stsz(UINT32 value)
	{
		m_stsz.push_back(value);
		return;
	}

	VOID add_stco(UINT32 value)
	{
		m_stco.push_back(value);
		return;
	}

	VOID set_stsd(UINT32 iSize, INT8 *pcBuff)
	{
		if (128 >= iSize)
		{
			m_stsd.iSize = iSize;
			memcpy(m_stsd.acBuff, pcBuff, iSize);
		}
		return;
	}

public:

	vector<UINT32> m_stss;
	vector<atom_stsc> m_stsc;
	vector<UINT32> m_stsz;
	vector<UINT32> m_stco;

	atom_stsd m_stsd;
};

/// ParAbstract class
class CParAbstract
{
public:

	static CParAbstract* getInstance();

	static VOID deleteInstance();

	virtual bool init(UINT32 totalSize, UINT32 fragCount);

	virtual INT32 analyzeData(INT8 *buffer, LONG &offset, LONG &length);

	virtual bool getPartitionHeaderInfo(map< pair< LONG, LONG >, map< pair< LONG, LONG >, string > > &partitionHeaderInfo);

protected:

	/**
	 * @brief		Constructor of CParAbstract.
	 * @param		no.
	 * @return		no.
	 */
	CParAbstract();

	/**
	 * @brief		Destructor of CParAbstract.
	 * @param		no
	 * @return		no
	 */
	virtual ~CParAbstract();

private:

	INT32 initMediaInfo();

	INT32 getDataMap();

	VOID getChunkMap();

	VOID getStss(UINT32 uiPos);

	VOID getStsc(UINT32 uiPos);

	VOID getStsz(UINT32 uiPos);

	VOID getStco(UINT32 uiPos);

	VOID getAvcC(UINT32 uiPos);

	UINT32 getBoxPos(UINT32 uiBeginPos, UINT32 uiCmpBoxType);

	UINT32 getTrakBoxPos();

	UINT32 getMdiaBoxPos();

	UINT32 getMinfBoxPos();

	INT32 getHdlrType(UINT32 uiTrackPos, INT8 *pcHdlrType);

	UINT32 getStblBoxPos();

	UINT32 getStssBoxPos();

	UINT32 getStscBoxPos();

	UINT32 getStszBoxPos();

	UINT32 getStcoBoxPos();

	UINT32 getStsdBoxPos();

	UINT32 getAvc1BoxPos();

	UINT32 getAvcCBoxPos();

	INT32 packedFragmentHeader(INT8 *pcHeaderBuff, UINT32 uiOffset, UINT32 uiLength, vector<STRUCT_FRAME_INFO> &vSampleCount);

	INT32 getFragmentHeaderSize(INT32 iSampleCount);

private:

	static CParAbstract *m_pParAbstract;

	UINT32 m_uiStblPos;

	UINT32 m_uiTotalSize;

	UINT32 m_uiFragCount;

	UINT32 m_uiAVGSize;

	LONG m_lDataLength;

	INT8 *m_pDataBuffer;

	stbl m_stbl;

	vector<chunk> m_chunk;

	map< pair< LONG, LONG >, map< pair< LONG, LONG >, string > > m_partitionHeaderInfo;
};


#endif /* MEDIAPROCESSIF_PARABSTRACT_H */

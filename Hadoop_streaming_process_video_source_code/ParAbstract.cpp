/**
 *    file: ParAbstract.cpp
 *   brief: class of ParAbstract.
 *  author: xuhm
 * created: 2014.11.27
 */

#include "ParAbstract.h"
#include <sys/stat.h>
#include <arpa/inet.h>


CParAbstract *CParAbstract::m_pParAbstract = NULL;

CParAbstract* CParAbstract::getInstance()
{
	if (NULL == m_pParAbstract)
	{
		m_pParAbstract = new CParAbstract();
		if (NULL == m_pParAbstract)
		{
			// log
		}
	}
	return m_pParAbstract;
}

VOID CParAbstract::deleteInstance()
{
	if (NULL != m_pParAbstract)
	{
		delete m_pParAbstract;
		m_pParAbstract = NULL;
	}
	return;
}

/**
 * @brief		Constructor of CParAbstract.
 * @param		no.
 * @return		no.
 */
CParAbstract::CParAbstract()
 : m_uiStblPos(0)
 , m_uiTotalSize(0)
 , m_uiFragCount(0)
 , m_uiAVGSize(0)
 , m_lDataLength(0)
 , m_pDataBuffer(NULL)
{

}

/**
 * @brief		Destructor of CParAbstract.
 * @param		no
 * @return		no
 */
CParAbstract::~CParAbstract()
{

}

bool CParAbstract::init(UINT32 totalSize, UINT32 fragCount)
{
	bool bRet = true;
	if ((fragCount == 0) ||
		(totalSize == 0))
	{
		bRet = false;
	}
	else
	{
		m_uiStblPos = 0;
		m_uiTotalSize = 0;
		m_uiFragCount = 0;
		m_uiAVGSize = 0;
		m_lDataLength = 0;
		m_pDataBuffer = NULL;
		m_stbl.clearData();
		m_chunk.clear();
		m_partitionHeaderInfo.clear();

		m_uiTotalSize = totalSize;
		m_uiFragCount = fragCount;
		m_uiAVGSize = totalSize / fragCount;
	}
	return bRet;
}

INT32 CParAbstract::analyzeData(INT8 *buffer, LONG &offset, LONG &length)
{
	INT32 iRet = PROCESS_NO_ERROR;

	if ((NULL == buffer) ||
		(0 == length))
	{
		cout<<"input error!"<<endl;
		iRet = PROCESS_ERROR;
		return iRet;
	}

	UINT32 uiBoxLen = 0;
	UINT32 uiBoxType = 0;
	UINT32 uiRealBoxLen = 0;
	UINT32 uiRealBoxType = 0;
	memcpy(&uiBoxLen, buffer, sizeof(UINT32));
	uiRealBoxLen = ntohl(uiBoxLen);
	memcpy(&uiBoxType, buffer + sizeof(UINT32), sizeof(UINT32));
//	printf("$$$ ");
//	for (int i = 0; i < 8; ++i)
//	{
//		printf("%d ", buffer[i]);
//	}
//	cout << endl;
	uiRealBoxType = ntohl(uiBoxType);
	if (offset == 0)
	{
		if (uiRealBoxType != MP4_BOX_TYPE_FTYP)
		{
			cout<<"invalid input media!"<<endl;
			iRet = PROCESS_ERROR;
			return iRet;
		}

		offset += uiRealBoxLen;
		length = 8;
	}
	else
	{
		if (uiRealBoxType != MP4_BOX_TYPE_MOOV)
		{
			offset += uiRealBoxLen;
			if (offset >= m_uiTotalSize)
			{
				cout<<"((((place..."<<endl;
				cout<<"invalid input media!"<<endl;
				iRet = PROCESS_ERROR;
				return iRet;
			}
			length = 8;
		}
		else
		{
			if (8 < length)
			{
				iRet = PROCESS_COMPLETE;

				m_pDataBuffer = buffer;
				m_lDataLength = length;

				if (0 > initMediaInfo())
				{
					cout<<"initMediaInfo error!"<<endl;
					iRet = PROCESS_ERROR;
					return iRet;
				}

				if (0 > getDataMap())
				{
					cout<<"getDataMap error!"<<endl;
					iRet = PROCESS_ERROR;
					return iRet;
				}
			}
			else
			{
				offset += 0;
				length = uiRealBoxLen;
			}
		}
	}

	return iRet;
}

INT32 CParAbstract::getDataMap()
{
	INT32 iRet = 0;

	UINT32 uiLastSplitPos = 0;
	UINT32 uiLastPos = 0;
	UINT32 uiOffset = 0;
	UINT32 uiSplitCount = 0;

	INT32 iThreshold = 0;

	STRUCT_HEADER_INFO stFragHdrInfo;

	STRUCT_FRAME_INFO stFrameInfo;
	vector<STRUCT_FRAME_INFO> vSampleCount;
	vSampleCount.clear();

	map< pair< LONG, LONG >, string > mGopMap;
	mGopMap.clear();

	INT32 iSSIndex = 0;
	UINT32 uiChunkIndex = 0;
	for (; iSSIndex < (m_stbl.m_stss.size()); ++iSSIndex)
	{
		if (iSSIndex == (m_stbl.m_stss.size() - 1))
		{
			for (INT32 iSZIndex = m_stbl.m_stss[iSSIndex]; iSZIndex <= m_chunk[m_chunk.size() - 1].m_endSampleId; ++iSZIndex)
			{
				for ( ; uiChunkIndex < m_chunk.size(); ++uiChunkIndex)
				{
					if (iSZIndex <= m_chunk[uiChunkIndex].m_endSampleId)
					{
						break;
					}
				}

				uiOffset = m_stbl.m_stco[uiChunkIndex];

				for (INT32 iCSIndex = m_chunk[uiChunkIndex].m_startSampleId; iCSIndex < iSZIndex; ++iCSIndex)
				{
					uiOffset += m_stbl.m_stsz[iCSIndex];
				}

				memset(&stFrameInfo, 0, sizeof(stFrameInfo));
				stFrameInfo.iSampleOffset = uiOffset - uiLastPos;
				stFrameInfo.iSampleSize = m_stbl.m_stsz[iSZIndex];
				vSampleCount.push_back(stFrameInfo);

				uiOffset += m_stbl.m_stsz[iSZIndex];
			}
		}
		else
		{
			for (INT32 iSZIndex = m_stbl.m_stss[iSSIndex]; iSZIndex < m_stbl.m_stss[iSSIndex + 1]; ++iSZIndex)
			{
				for ( ; uiChunkIndex < m_chunk.size(); ++uiChunkIndex)
				{
					if (iSZIndex <= m_chunk[uiChunkIndex].m_endSampleId)
					{
						break;
					}
				}

				uiOffset = m_stbl.m_stco[uiChunkIndex];

				for (INT32 iCSIndex = m_chunk[uiChunkIndex].m_startSampleId; iCSIndex < iSZIndex; ++iCSIndex)
				{
					uiOffset += m_stbl.m_stsz[iCSIndex];
				}

				memset(&stFrameInfo, 0, sizeof(stFrameInfo));
				stFrameInfo.iSampleOffset = uiOffset - uiLastPos;
				stFrameInfo.iSampleSize = m_stbl.m_stsz[iSZIndex];
				vSampleCount.push_back(stFrameInfo);

				uiOffset += m_stbl.m_stsz[iSZIndex];
			}
		}

		memset(&stFragHdrInfo, 0, sizeof(stFragHdrInfo));
		stFragHdrInfo.uiHdrLength = getFragmentHeaderSize(vSampleCount.size());
		stFragHdrInfo.pcHdrBuff = new INT8[stFragHdrInfo.uiHdrLength];
		if (NULL == stFragHdrInfo.pcHdrBuff)
		{
			cout<<"memory error!"<<endl;
			iRet = -1;
			return iRet;
		}
		memset(stFragHdrInfo.pcHdrBuff, 0, stFragHdrInfo.uiHdrLength);
		packedFragmentHeader(stFragHdrInfo.pcHdrBuff, uiLastPos, uiOffset - uiLastPos, vSampleCount);
		vSampleCount.clear();

		mGopMap[make_pair(uiLastPos, uiOffset - uiLastPos)] = string(stFragHdrInfo.pcHdrBuff, stFragHdrInfo.uiHdrLength);

		uiLastPos = uiOffset;

		delete [] stFragHdrInfo.pcHdrBuff;

		if ((m_uiFragCount - 1) == uiSplitCount)
		{
			continue;
		}

		if ((uiOffset - uiLastSplitPos) >= (m_uiAVGSize - iThreshold))
		{
			iThreshold = (uiOffset - uiLastSplitPos) - (m_uiAVGSize - iThreshold);
			if (m_uiAVGSize < iThreshold)
			{
				iThreshold = m_uiAVGSize;
			}

			m_partitionHeaderInfo[make_pair(uiLastSplitPos, uiOffset - uiLastSplitPos)] = mGopMap;

			mGopMap.clear();

			uiLastSplitPos = uiOffset;

			uiSplitCount++;
		}
	}

	m_partitionHeaderInfo[make_pair(uiLastSplitPos, m_uiTotalSize - uiLastSplitPos)] = mGopMap;
	mGopMap.clear();

	return iRet;
}

bool CParAbstract::getPartitionHeaderInfo(map< pair< LONG, LONG >, map< pair< LONG, LONG >, string > > &partitionHeaderInfo)
{
	bool bRet = true;
	partitionHeaderInfo.clear();
	partitionHeaderInfo = m_partitionHeaderInfo;
	return bRet;
}

INT32 CParAbstract::initMediaInfo()
{
	INT32 iRet = 0;

	// get stbl box position
	if (0 == m_uiStblPos)
	{
		m_uiStblPos = getStblBoxPos();
		if (0 == m_uiStblPos)
		{
			cout<<"find stbl box error!"<<endl;
			iRet = -1;
			return iRet;
		}
	}

	// stss data process
	UINT32 uiPos = 0;
	uiPos = getStssBoxPos();
	if (0 == uiPos)
	{
		cout<<"find stss box error!"<<endl;
		iRet = -1;
		return iRet;
	}

	getStss(uiPos);
	if (0 == m_stbl.m_stss.size())
	{
		cout<<"get stss data error!"<<endl;
		iRet = -1;
		return iRet;
	}

	// stsc data process
	uiPos = getStscBoxPos();
	if (0 == uiPos)
	{
		cout<<"find stsc box error!"<<endl;
		iRet = -1;
		return iRet;
	}

	getStsc(uiPos);
	if (0 == m_stbl.m_stsc.size())
	{
		cout<<"get stsc data error!"<<endl;
		iRet = -1;
		return iRet;
	}

	// stsz data process
	uiPos = getStszBoxPos();
	if (0 == uiPos)
	{
		cout<<"find stsz box error!"<<endl;
		iRet = -1;
		return iRet;
	}

	getStsz(uiPos);
	if (0 == m_stbl.m_stsz.size())
	{
		cout<<"get stsz data error!"<<endl;
		iRet = -1;
		return iRet;
	}

	// stco data process
	uiPos = getStcoBoxPos();
	if (0 == uiPos)
	{
		cout<<"find stco box error!"<<endl;
		iRet = -1;
		return iRet;
	}

	getStco(uiPos);
	if (0 == m_stbl.m_stco.size())
	{
		cout<<"get stco data error!"<<endl;
		iRet = -1;
		return iRet;
	}

	// stsd data process
	uiPos = getAvcCBoxPos();
	if (0 == uiPos)
	{
		cout<<"find avcC box error!"<<endl;
		iRet = -1;
		return iRet;
	}

	getAvcC(uiPos);
	if (0 == m_stbl.m_stsd.iSize)
	{
		cout<<"get avcC data error!"<<endl;
		iRet = -1;
		return iRet;
	}

	// chunk data process
	getChunkMap();
	if (0 == m_chunk.size())
	{
		cout<<"get chunk data error!"<<endl;
		iRet = -1;
		return iRet;
	}

	return iRet;
}

VOID CParAbstract::getChunkMap()
{
	if (!m_stbl.m_stsc.empty())
	{
		vector<atom_stsc>::iterator itr;
		itr = m_stbl.m_stsc.begin();
		atom_stsc elem_cur = *itr;
		++itr;

		UINT32 chunk_id_base = 0;
		UINT32 sample_id_base = 0;
		for ( ;itr != m_stbl.m_stsc.end(); ++itr)
		{
			UINT32 chunk_counts = itr->m_first_chunk - elem_cur.m_first_chunk;
			for (UINT32 i = 0; i < chunk_counts; i++)
			{
				chunk chk;
				chk.m_chunkId = chunk_id_base++;
				chk.m_startSampleId = sample_id_base;

				sample_id_base += elem_cur.m_sample_per_chunk;
				chk.m_endSampleId = sample_id_base - 1;

				m_chunk.push_back(chk);
			}
			elem_cur = *itr;
		}

		--itr;
		chunk chk;
		chk.m_chunkId = chunk_id_base++;
		chk.m_startSampleId = sample_id_base;

		sample_id_base += itr->m_sample_per_chunk;
		chk.m_endSampleId = sample_id_base - 1;

		m_chunk.push_back(chk);
	}

	return;
}

VOID CParAbstract::getStss(UINT32 uiPos)
{
	UINT32 uiSize = 0;
	UINT32 uiType = 0;
	UINT32 uiVersionFlags = 0;
	UINT32 uiEntryCount = 0;

	memcpy(&uiSize, m_pDataBuffer + uiPos, sizeof(UINT32));
	uiSize = ntohl(uiSize);

	memcpy(&uiType, m_pDataBuffer + uiPos + sizeof(UINT32), sizeof(UINT32));
	memcpy(&uiVersionFlags, m_pDataBuffer + uiPos + sizeof(UINT32) + sizeof(UINT32), sizeof(UINT32));
	memcpy(&uiEntryCount, m_pDataBuffer + uiPos + sizeof(UINT32) + sizeof(UINT32) + sizeof(UINT32), sizeof(UINT32));

	UINT32 uiReadSize = 0;
	UINT32 uiSampleNumber = 0;
	while ((uiSize - 4 * sizeof(UINT32) - uiReadSize) > 0)
	{
		uiSampleNumber = 0;
		memcpy(&uiSampleNumber, m_pDataBuffer + uiPos + 4 * sizeof(UINT32) + uiReadSize, sizeof(UINT32));
		uiSampleNumber = ntohl(uiSampleNumber);
		--uiSampleNumber;
		m_stbl.add_stss(uiSampleNumber);
		uiReadSize += sizeof(UINT32);
	}

	return;
}

VOID CParAbstract::getStsc(UINT32 uiPos)
{
	UINT32 uiSize = 0;
	UINT32 uiType = 0;
	UINT32 uiVersionFlags = 0;
	UINT32 uiEntryCount = 0;

	memcpy(&uiSize, m_pDataBuffer + uiPos, sizeof(UINT32));
	uiSize = ntohl(uiSize);

	memcpy(&uiType, m_pDataBuffer + uiPos + sizeof(UINT32), sizeof(UINT32));
	memcpy(&uiVersionFlags, m_pDataBuffer + uiPos + sizeof(UINT32) + sizeof(UINT32), sizeof(UINT32));
	memcpy(&uiEntryCount, m_pDataBuffer + uiPos + sizeof(UINT32) + sizeof(UINT32) + sizeof(UINT32), sizeof(UINT32));

	UINT32 uiReadSize = 0;
	atom_stsc stsc_tmp;
	while ((uiSize - 4 * sizeof(UINT32) - uiReadSize) > 0)
	{
		memset(&stsc_tmp, 0, sizeof(atom_stsc));
		memcpy(&stsc_tmp.m_first_chunk, m_pDataBuffer + uiPos + 4 * sizeof(UINT32) + uiReadSize, sizeof(UINT32));
		stsc_tmp.m_first_chunk = ntohl(stsc_tmp.m_first_chunk);
		memcpy(&stsc_tmp.m_sample_per_chunk, m_pDataBuffer + uiPos + 4 * sizeof(UINT32) + uiReadSize + sizeof(UINT32), sizeof(UINT32));
		stsc_tmp.m_sample_per_chunk = ntohl(stsc_tmp.m_sample_per_chunk);
		memcpy(&stsc_tmp.m_sample_dsc_index, m_pDataBuffer + uiPos + 4 * sizeof(UINT32) + uiReadSize + sizeof(UINT32) + sizeof(UINT32), sizeof(UINT32));
		stsc_tmp.m_sample_dsc_index = ntohl(stsc_tmp.m_sample_dsc_index);

		m_stbl.add_stsc(stsc_tmp);

		uiReadSize += 3 * sizeof(UINT32);
	}

	return;
}

VOID CParAbstract::getStsz(UINT32 uiPos)
{
	UINT32 uiSize = 0;
	UINT32 uiType = 0;
	UINT32 uiVersionFlags = 0;
	UINT32 uiSampleSize = 0;
	UINT32 uiEntryCount = 0;

	memcpy(&uiSize, m_pDataBuffer + uiPos, sizeof(UINT32));
	uiSize = ntohl(uiSize);

	memcpy(&uiType,         m_pDataBuffer + uiPos + sizeof(UINT32), sizeof(UINT32));
	memcpy(&uiVersionFlags, m_pDataBuffer + uiPos + sizeof(UINT32) + sizeof(UINT32), sizeof(UINT32));
	memcpy(&uiSampleSize,   m_pDataBuffer + uiPos + sizeof(UINT32) + sizeof(UINT32) + sizeof(UINT32), sizeof(UINT32));
	memcpy(&uiEntryCount,   m_pDataBuffer + uiPos + sizeof(UINT32) + sizeof(UINT32) + sizeof(UINT32) + sizeof(UINT32), sizeof(UINT32));

	UINT32 uiReadSize = 0;
	UINT32 uiSampleSizeTmp = 0;
	while ((uiSize - 5 * sizeof(UINT32) - uiReadSize) > 0)
	{
		uiSampleSizeTmp = 0;
		memcpy(&uiSampleSizeTmp, m_pDataBuffer + uiPos + 5 * sizeof(UINT32) + uiReadSize, sizeof(UINT32));
		uiSampleSizeTmp = ntohl(uiSampleSizeTmp);
		m_stbl.add_stsz(uiSampleSizeTmp);

		uiReadSize += sizeof(UINT32);
	}

	return;
}

VOID CParAbstract::getStco(UINT32 uiPos)
{
	UINT32 uiSize = 0;
	UINT32 uiType = 0;
	UINT32 uiVersionFlags = 0;
	UINT32 uiEntryCount = 0;

	memcpy(&uiSize, m_pDataBuffer + uiPos, sizeof(UINT32));
	uiSize = ntohl(uiSize);

	memcpy(&uiType, m_pDataBuffer + uiPos + sizeof(UINT32), sizeof(UINT32));
	memcpy(&uiVersionFlags, m_pDataBuffer + uiPos + sizeof(UINT32) + sizeof(UINT32), sizeof(UINT32));
	memcpy(&uiEntryCount, m_pDataBuffer + uiPos + sizeof(UINT32) + sizeof(UINT32) + sizeof(UINT32), sizeof(UINT32));

	UINT32 uiReadSize = 0;
	UINT32 uiChunkOffset = 0;
	while ((uiSize - 4 * sizeof(UINT32) - uiReadSize) > 0)
	{
		uiChunkOffset = 0;
		memcpy(&uiChunkOffset, m_pDataBuffer + uiPos + 4 * sizeof(UINT32) + uiReadSize, sizeof(UINT32));
		uiChunkOffset = ntohl(uiChunkOffset);

		m_stbl.add_stco(uiChunkOffset);

		uiReadSize += sizeof(UINT32);
	}

	return;
}

VOID CParAbstract::getAvcC(UINT32 uiPos)
{
	UINT32 uiSize = 0;
	UINT32 uiType = 0;

	memcpy(&uiSize, m_pDataBuffer + uiPos, sizeof(UINT32));
	uiSize = ntohl(uiSize);

	memcpy(&uiType, m_pDataBuffer + uiPos + sizeof(UINT32), sizeof(UINT32));
	uiType = ntohl(uiType);

	INT8 *pcBuf = new INT8[uiSize - 2 * sizeof(UINT32)];
	memcpy(pcBuf,  m_pDataBuffer + uiPos + 2 * sizeof(UINT32), uiSize - 2 * sizeof(UINT32));
	m_stbl.set_stsd(uiSize - 2 * sizeof(UINT32), pcBuf);
	delete [] pcBuf;

	return;
}

UINT32 CParAbstract::getBoxPos(UINT32 uiBeginPos, UINT32 uiCmpBoxType)
{
	UINT32 uiPos = uiBeginPos;

	while (true)
	{
		if (uiPos >= m_lDataLength)
		{
			uiPos = 0;
			break;
		}

		UINT32 uiBoxLen = 0;
		UINT32 uiBoxType = 0;
		UINT32 uiRealBoxLen = 0;
		UINT32 uiRealBoxType = 0;
		memcpy(&uiBoxLen, m_pDataBuffer + uiPos, sizeof(UINT32));
		uiRealBoxLen = ntohl(uiBoxLen);
		memcpy(&uiBoxType, m_pDataBuffer + uiPos + sizeof(UINT32), sizeof(UINT32));
		uiRealBoxType = ntohl(uiBoxType);

		if (uiCmpBoxType == uiRealBoxType)
		{
			return uiPos;
		}
		uiPos += uiRealBoxLen;
	}
	return uiPos;
}

UINT32 CParAbstract::getTrakBoxPos()
{
	UINT32 uiTrakBoxPos = 0;

	INT8 acHdlrType[BOX_TYPE_LENGTH] = {0};

	UINT32 uiTmpPos = 2 * sizeof(UINT32);

	while (true)
	{
		if (uiTmpPos >= m_lDataLength)
		{
			uiTrakBoxPos = 0;
			break;
		}

		UINT32 uiBoxLen = 0;
		UINT32 uiBoxType = 0;
		UINT32 uiRealBoxLen = 0;
		UINT32 uiRealBoxType = 0;
		memcpy(&uiBoxLen, m_pDataBuffer + uiTmpPos, sizeof(UINT32));
		uiRealBoxLen = ntohl(uiBoxLen);
		memcpy(&uiBoxType, m_pDataBuffer + uiTmpPos + sizeof(UINT32), sizeof(UINT32));
		uiRealBoxType = ntohl(uiBoxType);

		if (MP4_BOX_TYPE_TRAK == uiRealBoxType)
		{
			memset(acHdlrType, 0, BOX_TYPE_LENGTH);

			if (0 != getHdlrType(uiTmpPos, acHdlrType))
			{
				uiTrakBoxPos = 0;
				break;
			}

			if (0 == strncmp(acHdlrType, "vide", BOX_TYPE_LENGTH))
			{
				uiTrakBoxPos = uiTmpPos;
				break;
			}
		}
		uiTmpPos += uiRealBoxLen;
	}

	return uiTrakBoxPos;
}


UINT32 CParAbstract::getMdiaBoxPos()
{
	UINT32 uiMdiaBoxPos = 0;
	UINT32 uiTrakBoxPos = getTrakBoxPos();
	if (0 != uiTrakBoxPos)
	{
		uiMdiaBoxPos = getBoxPos(uiTrakBoxPos + 2 * sizeof(UINT32), MP4_BOX_TYPE_MDIA);
	}
	return uiMdiaBoxPos;
}

INT32 CParAbstract::getHdlrType(UINT32 uiTrackPos, INT8 *pcHdlrType)
{
	INT32 iRet = 0;
	if (NULL == pcHdlrType)
	{
		iRet = -1;
		return iRet;
	}

	UINT32 uiMdiaBoxPos = 0;
	uiMdiaBoxPos = getBoxPos(uiTrackPos + 2 * sizeof(UINT32), MP4_BOX_TYPE_MDIA);
	if (0 == uiMdiaBoxPos)
	{
		iRet = -1;
		return iRet;
	}

	UINT32 uiHdlrBoxPos = 0;
	uiHdlrBoxPos = getBoxPos(uiMdiaBoxPos + 2 * sizeof(UINT32), MP4_BOX_TYPE_HDLR);
	if (0 == uiHdlrBoxPos)
	{
		iRet = -1;
		return iRet;
	}

	memcpy(pcHdlrType, m_pDataBuffer + uiHdlrBoxPos + 4 * sizeof(UINT32), BOX_TYPE_LENGTH);

	return 0;

}

UINT32 CParAbstract::getMinfBoxPos()
{
	UINT32 uiMinfBoxPos = 0;
	UINT32 uitMdiaBoxPos = getMdiaBoxPos();
	if (0 != uitMdiaBoxPos)
	{
		uiMinfBoxPos = getBoxPos(uitMdiaBoxPos + 2 * sizeof(UINT32), MP4_BOX_TYPE_MINF);
	}
	return uiMinfBoxPos;
}

UINT32 CParAbstract::getStblBoxPos()
{
	UINT32 uiStblBoxPos = 0;
	UINT32 uiMinfBoxPos = getMinfBoxPos();
	if (0 != uiMinfBoxPos)
	{
		uiStblBoxPos = getBoxPos(uiMinfBoxPos + 2 * sizeof(UINT32), MP4_BOX_TYPE_STBL);
	}
	return uiStblBoxPos;
}

UINT32 CParAbstract::getStssBoxPos()
{
	UINT32 uiStssBoxPos = 0;
	if (0 != m_uiStblPos)
	{
		uiStssBoxPos = getBoxPos(m_uiStblPos + 2 * sizeof(UINT32), MP4_BOX_TYPE_STSS);
	}
	return uiStssBoxPos;
}

UINT32 CParAbstract::getStscBoxPos()
{
	UINT32 uiStscBoxPos =0;
	if (0 != m_uiStblPos)
	{
		uiStscBoxPos = getBoxPos(m_uiStblPos + 2 * sizeof(UINT32), MP4_BOX_TYPE_STSC);
	}
	return uiStscBoxPos;
}

UINT32 CParAbstract::getStszBoxPos()
{
	UINT32 uiStszBoxPos =0;
	if (0 != m_uiStblPos)
	{
		uiStszBoxPos = getBoxPos(m_uiStblPos + 2 * sizeof(UINT32), MP4_BOX_TYPE_STSZ);
	}
	return uiStszBoxPos;
}

UINT32 CParAbstract::getStcoBoxPos()
{
	UINT32 uiStcoBoxPos =0;
	if (0 != m_uiStblPos)
	{
		uiStcoBoxPos = getBoxPos(m_uiStblPos + 2 * sizeof(UINT32), MP4_BOX_TYPE_STCO);
	}
	return uiStcoBoxPos;
}

UINT32 CParAbstract::getStsdBoxPos()
{
	UINT32 uiStsdBoxPos =0;
	if (0 != m_uiStblPos)
	{
		uiStsdBoxPos = getBoxPos(m_uiStblPos + 2 * sizeof(UINT32), MP4_BOX_TYPE_STSD);
	}
	return uiStsdBoxPos;
}

UINT32 CParAbstract::getAvc1BoxPos()
{
	UINT32 uiAvc1BoxPos =0;
	UINT32 uiStsdBoxPos = getStsdBoxPos();
	if (0 != uiStsdBoxPos)
	{
		uiAvc1BoxPos = getBoxPos(uiStsdBoxPos + 4 * sizeof(UINT32), MP4_BOX_TYPE_AVC1);
	}
	return uiAvc1BoxPos;
}

UINT32 CParAbstract::getAvcCBoxPos()
{
	UINT32 uiAvcCBoxPos = 0;
	UINT32 uiAvc1BoxPos = getAvc1BoxPos();
	if (0 != uiAvc1BoxPos)
	{
		uiAvcCBoxPos = getBoxPos(uiAvc1BoxPos + 86, MP4_BOX_TYPE_AVCC);
	}
	return uiAvcCBoxPos;
}

INT32 CParAbstract::packedFragmentHeader(INT8 *pcHeaderBuff, UINT32 uiOffset, UINT32 uiLength, vector<STRUCT_FRAME_INFO> &vSampleCount)
{
	INT32 iRet = 0;

	if ((NULL == pcHeaderBuff) || (0 == vSampleCount.size()))
	{
		iRet = -1;
		return iRet;
	}

	// file info
	STRUCT_FILE_INFO stFileinfo;
	stFileinfo.stDataHdrBHdr.iBoxSize = getFragmentHeaderSize(vSampleCount.size());
	memset(pcHeaderBuff, 0, stFileinfo.stDataHdrBHdr.iBoxSize);
	memcpy(stFileinfo.stDataHdrBHdr.acBoxType, "dhdr", BOX_TYPE_LENGTH);
	stFileinfo.uiOffset = uiOffset;
	stFileinfo.uiLength = uiLength;
	memcpy(pcHeaderBuff, &stFileinfo, sizeof(stFileinfo));

	// decode parameters
	STRUCT_BOX_HEADER stPrmSetBHdr;
	stPrmSetBHdr.iBoxSize = m_stbl.m_stsd.iSize + sizeof(stPrmSetBHdr);
	memcpy(stPrmSetBHdr.acBoxType, "prms", BOX_TYPE_LENGTH);
	memcpy(pcHeaderBuff + sizeof(stFileinfo), &stPrmSetBHdr, sizeof(stPrmSetBHdr));
	memcpy(pcHeaderBuff + sizeof(stFileinfo) + sizeof(stPrmSetBHdr), m_stbl.m_stsd.acBuff, m_stbl.m_stsd.iSize);

	// each sample size
	STRUCT_BOX_HEADER stEachSizeBHdr;
	stEachSizeBHdr.iBoxSize = sizeof(stEachSizeBHdr) + vSampleCount.size() * sizeof(STRUCT_FRAME_INFO);
	memcpy(stEachSizeBHdr.acBoxType, "szop", BOX_TYPE_LENGTH);
	memcpy(pcHeaderBuff + sizeof(stFileinfo) + stPrmSetBHdr.iBoxSize, &stEachSizeBHdr, sizeof(stEachSizeBHdr));
	UINT32 uiSampleCountTmp = 0;
	for (vector<STRUCT_FRAME_INFO>::iterator Itor = vSampleCount.begin(); Itor != vSampleCount.end(); ++Itor)
	{
		memcpy(pcHeaderBuff + sizeof(stFileinfo) + stPrmSetBHdr.iBoxSize + sizeof(stEachSizeBHdr) + uiSampleCountTmp,
				&(Itor->iSampleOffset),
				sizeof(Itor->iSampleOffset));
		uiSampleCountTmp += sizeof(Itor->iSampleOffset);
		memcpy(pcHeaderBuff + sizeof(stFileinfo) + stPrmSetBHdr.iBoxSize + sizeof(stEachSizeBHdr) + uiSampleCountTmp,
				&(Itor->iSampleSize),
				sizeof(Itor->iSampleSize));
		uiSampleCountTmp += sizeof(Itor->iSampleSize);
	}

	return iRet;
}

INT32 CParAbstract::getFragmentHeaderSize(INT32 iSampleCount)
{
	INT32 iHeaderSize =
			sizeof(STRUCT_FILE_INFO) +                                             // file info
			sizeof(STRUCT_BOX_HEADER) + m_stbl.m_stsd.iSize +                      // pps and sps
			sizeof(STRUCT_BOX_HEADER) + iSampleCount * sizeof(STRUCT_FRAME_INFO);  // sample size and offset
	return iHeaderSize;
}


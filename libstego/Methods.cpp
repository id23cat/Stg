#include "StdAfx.h"
#include "Methods.h"

//void DemiHide(j_compress_ptr ccinfo, JpegStegoEncoder *pJSE, JBLOCKROW *MCU_data)
//{
//	int DCT_pos;
//	jpeg_component_info *compptr;
//	BYTE bit;
//
//	for (int blkn = 0; blkn < ccinfo->blocks_in_MCU; blkn++)
//	{
//		compptr = ccinfo->cur_comp_info[ccinfo->MCU_membership[blkn]];
//		if(pJSE->paste_message)
//		{
//			DCT_pos = selectPosition(MCU_data[blkn][0]);
//			if(DCT_pos != -1)
//			{
//				bit = pJSE->mit;
//				//cerr << (int)bit;
//				MCU_data[blkn][0][DCT_pos] = bit *				//set bit with sign
//					selectSign(MCU_data[blkn][0],DCT_pos);	//
//				pJSE->mit++;
//			}
//		}
//
//	}
//}

int selectPosition(JCOEF *coef)
{
	for(size_t i=0; i<ALLOW; i++)
	{
		switch(coef[allowable_position[i]])
			case 0:
			case 1:
			case -1:{
				return allowable_position[i];
					}
	}
	return -1;
}

int selectSign(JCOEF *coef, int position)
{
	if(coef[position]==1)
		return 1;
	else if(coef[position]==-1)
		return -1;
	else if(coef[position]==0)
	{
		int tmp = coef[position-7];
		tmp += coef[position+7];
		tmp += coef[position-8];
		tmp += coef[position+8];
		if(tmp<0)
			return -1;
		else
			return 1;
	}
	return 1;
}

LKoch KochZhaoPosition(JCOEF *coef)
{
	LKoch lk;
	lk.l1 = KochZhaoPositions[0][0];
	lk.l2 = KochZhaoPositions[0][1];
	lk.l3 = KochZhaoPositions[0][2];
	return lk;
}
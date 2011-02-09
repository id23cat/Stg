#ifdef _WIN32
#endif
#include "stdafx.h"
#include <stdio.h>
#include <string.h>

#include "LibInclude.h"
#include "StegoEncoder.h"

//void StegoEncoder::genOutFileName(char *in, char *out, int num)
//{
//	strcpy(out, dstdir);
//	int olen = strlen(out);
//	if(out[olen-1]!='\\' || out[olen-1]!='/')
//		strncat(out,"/",1);
//	num++;
//	if(num>-1&&filecount>0)
//	{		
//		int zeros=0;
//		int n=num;
//		for(int i=0;i<5;i++)
//		{
//			n=n/10;
//			filecount=filecount/10;
//			if(filecount == 0)break;
//			if(n==0)zeros++;
//		}
//		for(zeros;zeros>0;zeros--)
//			strncat(out,"0",1);
//		olen = strlen(out);
//		itoa(num,out+olen,10);		
//		strncat(out,"-",1);
//	}	
//	int len = strlen(in);
//	char *p=strchr(in,'/');
//	if(p==NULL)
//		p=strchr(in,'\\');
//	if(p!=NULL)
//		strcat(out,p);
//	else
//		strcat(out,in);
//	/*for(i=len-1;i>=0;i--)
//		if(in[i]=='\\'||in[i]=='/')
//			break;*/	
//}

StegoEncoder::StegoEncoder(void)
{
	paste_message = false;
	capacityBit = 0;
	//paste_length = false;
#if _DEBUG
	/*totC = 0;*/
#endif

	//redirect stderr to stego.log
	/*if((outstream=freopen("info.log","w",stdout))==NULL)
		cerr << "Can not create log file info.log"<<endl;*/
	if((errstream=freopen("stego.log","w",stderr))==NULL)
		cerr << "Can not create log file stego.log"<<endl;

	srcfiles=NULL;
	dstfiles=NULL;
	filecount=0;
	dstdir=NULL;

}

StegoEncoder::~StegoEncoder(void)
{
	//freopen("CON","w",stdout);
	freopen("CON","w",stderr);		
	/*if(outstream)
		fclose(outstream);*/
	if(errstream)
		fclose(errstream);
}

void StegoEncoder::SetMessage(BYTE *mes, size_t len)  throw(...)
{
	mesArray.SetMessage(mes, len);
	//mesArray.AddArray(mes, len);
	mit = mesArray.Begin();
	mit.SetOutOfRangeExceptionStatus(true);			//Out of range lead to throwing exception
	paste_message = true;

	/*lenArray.SetArray((BYTE*)&len, sizeof(size_t));
	lenArray.AddArray((BYTE*)&len, sizeof(size_t));
	size_t l;
	lenArray.GetArray((BYTE*)&l);
	lit = lenArray.Begin();
	paste_length = true;*/
}

//void StegoEncoder::SetMessageFile(wchar_t *mesFile) throw(...)
//{
//	FILE *instream;
//	size_t len;
//	if( _wfopen_s( &instream, mesFile, _T("rb") ) != 0 )
//	{
//		char str[256]={0};
//		sprintf(str,"Can not open file %s\n",mesFile);
//		throw Exception(str);		
//	};
//	fseek(instream, 0, SEEK_END);
//	len = ftell(instream);
//	BYTE * mes = new BYTE[len];
//	fseek(instream, 0, SEEK_SET);
//	len = fread(mes, sizeof(BYTE), len, instream);
//	//cout << *len << endl;
//	//cout << (char *) mes;
//	fclose(instream);
//	SetMessage(mes,len);
//	delete mes;
//}

void StegoEncoder::SetMessageFile(char *mesFile) throw(...)
{
	FILE *instream;
	size_t len;
	if( fopen_s( &instream, mesFile, "rb" ) != 0 )
	{
		char str[256]={0};
		sprintf(str,"Can not open file %s\n",mesFile);
		throw Exception(str);		
	};
	fseek(instream, 0, SEEK_END);
	len = ftell(instream);
	BYTE * mes = new BYTE[len];
	fseek(instream, 0, SEEK_SET);
	len = fread(mes, sizeof(BYTE), len, instream);
	//cout << *len << endl;
	//cout << (char *) mes;
	fclose(instream);
	SetMessage(mes,len);
	delete mes;
}

void StegoEncoder::SetMessageFile(char *mesFile, size_t length) throw(...)
{
	FILE *instream;
	size_t len;
	if( fopen_s( &instream, mesFile, "rb" ) != 0 )
	{
		char str[256]={0};
		sprintf(str,"Can not open file %s\n",mesFile);
		throw Exception(str);		
	};
	fseek(instream, 0, SEEK_END);
	len = ftell(instream);
	if(len>length)			//read less then total file size
		len = length;
	BYTE * mes = new BYTE[len];
	fseek(instream, 0, SEEK_SET);
	len = fread(mes, sizeof(BYTE), len, instream);
	//cout << *len << endl;
	//cout << (char *) mes;
	fclose(instream);
	SetMessage(mes,len);
	delete mes;
}

//void StegoEncoder::AddContainer(char *file)
//{
//
//}

void StegoEncoder::SetDstDir(char *dir)
{
	int len=strlen(dir);
	dstdir=new char[len];
	memcpy(dstdir,dir,len);
}

//void StegoEncoder::SetInFile(char *inf)
//{
//	size_t len = strlen(inf)+1;
//	memcpy(infile,inf,len);
//
//}
//void StegoEncoder::SetOutFile(char *outf)
//{
//	size_t len = strlen(outf)+1;
//	memcpy(outfile,outf,len);
//}

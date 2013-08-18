
 /***********************************************************************
 Copyright:2010-2012 Double One<dfghj77777@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA., or go to http://www.gnu.org/copyleft/lesser.txt

 --------------------------------------------------------------------
 @File Name:Wxm_IniFile.h
 @Describe:This is a part of xm_ini++
 @Create Date:2012-12-26 17:06:57
 @Version:1.0
 @Author��Double One
 @Blog�� http://onedouble.cnblogs.com  
 @Modifier: Double One <dfghj77777@gmail.com>
 @Modify Date��2012-12-26 17:06:57
 @Modify Reason��
 ***********************************************************************/


#ifndef _WXM_INI_H
#define _WXM_INI_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <io.h>
#include <fcntl.h>
#include <mmsystem.h>
#include "Common.h"
#include <assert.h>
#pragma comment(lib,"winmm.lib")

extern "C"{
/**
	�����ļ�
	@param filename ��������ļ���
*/
	bool Open(const char *);		
/** 
	�����ļ�
	@param filename:�ļ���
*/
	bool Save(const char *filename=NULL);		

	char *GetData();		//����ȫ�ļ�����

	int GetLineCount(int);			//�����ļ���������	
/** 
	��������е�����λ�ò�������m_pIndexList
*/
	void InitIndex();			
/**
	����ָ������λ��
	@param string ֵ
	@return �������ڵ�λ��
*/
	int FindIndex(const char *);		
/** 
	����ָ�����ݵ����ݵ�λ��
	@param index ������
	@param string ����ֵ
	@return Ŀ��λ��
*/
	int FindKey(int,const char *);	
/** 
	�ߵ���һ��
	@param p λ��
	@return ��һ������
*/
	int GotoNextLine(int); 	

/**
	��ָ��λ�ö�һ���ݼ���ֵ
	@param p λ������
	@return ָ��λ����������
*/
	char *ReadKey(OUT int &);	
/** 
	��ָ��λ�ö�һ���ַ���
	@param p  ��������
*/
	char *ReadData(int);

/** 
	���һ������
*/
	bool AddIndex(const char *);		
/** 
	�ڵ�ǰλ�ü���һ������
	@param p λ��
	@param name ���ݼ�
	@param string ����ֵ
*/
	bool AddData(int,const char *,const char *);	
	/** 
	�ڵ�ǰλ���޸�һ�����ݵ�ֵ
	@param p λ��
	@param name ���ݼ�
	@param string ����ֵ

*/
	bool ModityData(int,const char *,const char *); 

/** 
	����ͨ��ʽ��һ�ַ�������
	@param ������
	@param ������
	@return Ŀ���ַ���
*/
	char *ReadValue(const char *,const char *);	
/** 
	��ָ�����ж�һ������ͬ����Ϊ�˳����Ķ�ȡ�����ҿ��Բ����ľ��������
	@param index:������
	@param lines:�к�
*/
	int ReadValueIntByLine(const char *, int );	
/**
	����ͨ��ʽ��һint����
	@param index ������
	@param name ���ݼ�
*/
	int ReadValueInt(const char *,const char *);	

/** 
	��ָ�����ж�һ�ַ��������������������ڶ���һ�����ݣ�
	���ҿ��Բ����ľ�������֣�������ѭ���ķ�ʽ��
	@param ������
	@param �к�
	@return ����ֵ
*/
	char *ReadValueByLine(const char *, int);	
	/** 
	��ָ�����ж�һ���ݼ���Ŀ�Ļ���Ϊ��������ȡ��
	@param index ������
	@param lines �к�
	@return ���ݼ�
*/
	char *ReadKeyByLine(const char *, int);	
/** 
	����ͨ��ʽдһ����
	@param index ������
	@param name ���ݼ�
	@param string ����ֵ��int��
*/
	bool WriteValueInt(const char *,const char *, int);	
	/** 
	����ͨ��ʽдһ�ַ������ݡ����������֣�����
	@param index ������
	@param name ���ݼ�
	@param string ����ֵ
*/
	bool WriteValue(const char *,const char *, char *);	

	/** 
	�������������������Ǳ���(����)����ι��ж����У�һ��������ѭ��������
	@param index ������
*/
	int GetCountByIndex(char *);			//������������������INDEX����һ�����У�


#define ERROR_DATA -99999999
#define _MALLOC(B,C)		(B*)malloc(sizeof(B) * C)
#define _FREE(X)			if( (X) != NULL ) { free(X); (X)=NULL; } 
#define KEYDOWN(name,key) (name[key] & 0x80)

#define IN
#define OUT
#define For(i , n) for(int i = 0 ; i < (n) ; ++i)
#define SZ(x)  (int)((x).size())
#define LEN(x) (int)((x).strlen())

#ifdef DEBUG
#define _ASSERT(exp) (void)((exp)||(assert(#exp,__FILE__,__LINE__),0))
#else 
#define _ASSERT(exp)
#endif


	enum e_file_type{
		eWINDOWS = 0,
		eLINUX ,
		eMAC ,
		eLFCR ,
		eOHTER ,
		eFILETYPECOUNT,
	};

#ifdef _WINDOWS
#define _DEFAULT_FILE_TYPE eWINDOWS
#elif defined _LINUX
#define _DEFAULT_FILE_TYPE eLINUX
#elif defined _MAC
#define _DEFAULT_FILE_TYPE eMAC
#else
#define _DEFAULT_FILE_TYPE eLFCR
#endif // _WINDOWS
};

#endif
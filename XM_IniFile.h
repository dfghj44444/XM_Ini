
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
#pragma comment(lib,"winmm.lib")

class  DLL_EXPORT  XM_IniFile{
private:
	char m_FileName[MAX_PATH];	//�ļ���
	int m_DataLen;			//�ļ�����
	char *m_pData;				//�ļ�����

	int m_IndexNum;			//������Ŀ��[]����Ŀ��
	int *m_pIndexList;			//������λ���б�

	int m_Point;				//��ǰָ��
	int m_Line, m_Word;			//��ǰ�У���ǰ��

public:
	XM_IniFile();
/**
	�вι���
	@param filename ���򿪵��ļ���
*/
	XM_IniFile(char *);		
	~XM_IniFile();				

	char *GetData();		//�����ļ�����
	int GetLines(int);			//�����ļ�������
/**
	�����ļ�
	@param filename ��������ļ���
*/
	bool Open(char *);		
/** 
	�����ļ�
	@param filename:�ļ���
*/
	bool Save(char *filename=NULL);		

private:
	
/** 
	��������е�����λ�ò�������m_pIndexList
*/
	void InitIndex();			
/**
	����ָ������λ��
	@param string ֵ
	@return �������ڵ�λ��
*/
	int FindIndex(char *);		
/** 
	����ָ�����ݵ����ݵ�λ��
	@param index ������
	@param string ����ֵ
	@return Ŀ��λ��
*/
	int FindKey(int, char *);	
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
	char *ReadKey(int &);	
/** 
	��ָ��λ�ö�һ���ַ���
	@param p  ��������
*/
	char *ReadData(int);

/** 
	���һ������
*/
	bool AddIndex(char *);		
/** 
	�ڵ�ǰλ�ü���һ������
	@param p λ��
	@param name ���ݼ�
	@param string ����ֵ
*/
	bool AddData(int, char *, char *);	
/** 
	�ڵ�ǰλ���޸�һ�����ݵ�ֵ
	@param p λ��
	@param name ���ݼ�
	@param string ����ֵ

*/
	bool ModityData(int, char *, char *); 

public:


/** 
	����ͨ��ʽ��һ�ַ�������
	@param ������
	@param ������
	@return Ŀ���ַ���
*/
	char *ReadValue(char *, char *);	
/** 
	��ָ�����ж�һ������ͬ����Ϊ�˳����Ķ�ȡ�����ҿ��Բ����ľ��������
	@param index:������
	@param lines:�к�
*/
	int ReadValueInt(char *, int );	
/**
	����ͨ��ʽ��һint����
	@param index ������
	@param name ���ݼ�
*/
	int ReadValueInt(char *, char *);	

/** 
	��ָ�����ж�һ�ַ��������������������ڶ���һ�����ݣ�
	���ҿ��Բ����ľ�������֣�������ѭ���ķ�ʽ��
	@param ������
	@param �к�
	@return ����ֵ
*/
	char *ReadValue(char *, int);	
	/** 
	��ָ�����ж�һ���ݼ���Ŀ�Ļ���Ϊ��������ȡ��
	@param index ������
	@param lines �к�
	@return ���ݼ�
*/
	char *ReadKey(char *, int);	
/** 
	����ͨ��ʽдһ����
	@param index ������
	@param name ���ݼ�
	@param string ����ֵ��int��
*/
	bool WriteValueInt(char *, char *, int);	
	/** 
	����ͨ��ʽдһ�ַ������ݡ����������֣�����
	@param index ������
	@param name ���ݼ�
	@param string ����ֵ
*/
	bool WriteValue(char *, char *, char *);	

	/** 
	�������������������Ǳ���(����)����ι��ж����У�һ��������ѭ��������
	@param index ������
*/
	int GetContinueDataNum(char *);			//������������������INDEX����һ�����У�
};

#endif
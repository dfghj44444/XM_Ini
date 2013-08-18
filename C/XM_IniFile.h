
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
 @Author：Double One
 @Blog： http://onedouble.cnblogs.com  
 @Modifier: Double One <dfghj77777@gmail.com>
 @Modify Date：2012-12-26 17:06:57
 @Modify Reason：
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
	读入文件
	@param filename 被读入的文件名
*/
	bool Open(const char *);		
/** 
	保存文件
	@param filename:文件名
*/
	bool Save(const char *filename=NULL);		

	char *GetData();		//返回全文件内容

	int GetLineCount(int);			//返回文件的总行数	
/** 
	计算出所有的索引位置并保存至m_pIndexList
*/
	void InitIndex();			
/**
	返回指定标题位置
	@param string 值
	@return 索引所在的位置
*/
	int FindIndex(const char *);		
/** 
	返回指定数据的内容的位置
	@param index 索引号
	@param string 数据值
	@return 目标位置
*/
	int FindKey(int,const char *);	
/** 
	走到下一行
	@param p 位置
	@return 下一行索引
*/
	int GotoNextLine(int); 	

/**
	在指定位置读一数据键或值
	@param p 位置引用
	@return 指定位置数据名称
*/
	char *ReadKey(OUT int &);	
/** 
	在指定位置读一行字符串
	@param p  行索引号
*/
	char *ReadData(int);

/** 
	添加一个索引
*/
	bool AddIndex(const char *);		
/** 
	在当前位置加入一个数据
	@param p 位置
	@param name 数据键
	@param string 数据值
*/
	bool AddData(int,const char *,const char *);	
	/** 
	在当前位置修改一个数据的值
	@param p 位置
	@param name 数据键
	@param string 数据值

*/
	bool ModityData(int,const char *,const char *); 

/** 
	以普通方式读一字符串数据
	@param 索引名
	@param 数据名
	@return 目标字符串
*/
	char *ReadValue(const char *,const char *);	
/** 
	在指定的行读一整数，同样是为了成批的读取，而且可以不关心具体的名字
	@param index:索引号
	@param lines:行号
*/
	int ReadValueIntByLine(const char *, int );	
/**
	以普通方式读一int数据
	@param index 索引号
	@param name 数据键
*/
	int ReadValueInt(const char *,const char *);	

/** 
	在指定的行读一字符串，设计这个函数是用于读出一批数据，
	而且可以不关心具体的名字，可以用循环的方式读
	@param 索引号
	@param 行号
	@return 数据值
*/
	char *ReadValueByLine(const char *, int);	
	/** 
	在指定的行读一数据键，目的还是为了批量获取、
	@param index 索引号
	@param lines 行号
	@return 数据键
*/
	char *ReadKeyByLine(const char *, int);	
/** 
	以普通方式写一整数
	@param index 索引号
	@param name 数据键
	@param string 数据值（int）
*/
	bool WriteValueInt(const char *,const char *, int);	
	/** 
	以普通方式写一字符串数据。索引，名字，内容
	@param index 索引号
	@param name 数据键
	@param string 数据值
*/
	bool WriteValue(const char *,const char *, char *);	

	/** 
	返回连续的行数，就是标题(索引)这个段共有多少行，一般是用来循环记数的
	@param index 索引名
*/
	int GetCountByIndex(char *);			//返回连续的行数（从INDEX到第一个空行）


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
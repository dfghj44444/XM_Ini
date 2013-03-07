/***********************************************************************
@Copy Right: 王大宝 ( Double One)
@File Name:PublicLib.h
@Describe:
@Create Date:2012-12-25 17:22:30
@Version:1.1
@Author：Double One
@Blog： http://onedouble.cnblogs.com   http://cn.linkedin.com/pub/xingmin-wang/37/5b7/b98
@Modifier: Double One (dfghj77777@gmail.com)
@Modify Date：2012-12-25 17:22:30
@Modify Reason：delete sth to adapt "xm_ini" lib
***********************************************************************/

#ifndef _WXM_COMMON_H
#define _WXM_COMMON_H

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

/**
	取得随机数
	@param nMax:范围最大值
	@return 一个随机数值
*/
int random(int nMax);						//随机数
/**
	按下键后松开
	@param Key:键值
	@param x:按了几个键
*/
void PressKey(DWORD Key,int x=0);			
/**
	等待按键
*/
void WaitKey(DWORD Key);
/**
	延时函数
	@param time:延迟时间
*/

/**
	获取文件长度
	@param name:文件名
*/
int GetFileLength(char *name);		
/**
	判断文件是否存在
	@param name:文件名
*/
bool CheckFile(char *name);			

/**
	弹出调试信息
*/
void ShowMessage(char *msg,...);	
/**
	输出调试信息
*/
void PutDebugString(char *msg);		

/**
	char to wchar
*/
WCHAR* WChar(char *string);			
/**
	wchar to char
*/
char* Char(WCHAR *string);			

/**
	从文件名含路径分离出文件名不含路径
	@param filename:含路径的文件名
	@return 不含路径的文件名
*/
char* GetFileName(char* filename);	
/**
	由文件名获取文件路径
	@param filename:文件名
	@return 文件路径
*/
char *GetFilePath(char *filename);	


#define ERROR_DATA -99999999
#define _RELEASE(A)			try{ if( (A) != NULL ) { (A) -> Release(); (A) = NULL; } } catch(...) { PutDebugString("error ->Release!"); }
#define _DELETE(X)			try{ if( (X) != NULL ) { delete (X); (X) = NULL; } } catch(...) { PutDebugString("error delete!"); }
#define _DELETE_ARRAY(X)	try{ if( (X) != NULL ) { delete[] (X); (X) = NULL; } } catch(...) { PutDebugString("error delete[]!"); }
#define _FREE(X)			try{ if( (X) != NULL ) { free(X); (X)=NULL; } } catch(...) { PutDebugString("error free()!"); }
#define KEYDOWN(name,key) (name[key] & 0x80)

#define IN
#define OUT
#define For(i , n) for(int i = 0 ; i < (n) ; ++i)
#define SZ(x)  (int)((x).size())
#define LEN(x) (int)((x).strlen())

#ifdef DEBUG
#define _ASSERT(exp) (void)((exp)||(_assert(#exp,__FILE__,__LINE__),0))
#else 
#define _ASSERT(exp)
#endif

#ifdef INIFILE_EXPORT
#define DLL_EXPORT  __declspec(dllexport)
#else
#define DLL_EXPORT  __declspec(dllimport)
#endif

#endif

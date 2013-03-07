/***********************************************************************
@Copy Right: ���� ( Double One)
@File Name:PublicLib.h
@Describe:
@Create Date:2012-12-25 17:22:30
@Version:1.1
@Author��Double One
@Blog�� http://onedouble.cnblogs.com   http://cn.linkedin.com/pub/xingmin-wang/37/5b7/b98
@Modifier: Double One (dfghj77777@gmail.com)
@Modify Date��2012-12-25 17:22:30
@Modify Reason��delete sth to adapt "xm_ini" lib
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
	ȡ�������
	@param nMax:��Χ���ֵ
	@return һ�������ֵ
*/
int random(int nMax);						//�����
/**
	���¼����ɿ�
	@param Key:��ֵ
	@param x:���˼�����
*/
void PressKey(DWORD Key,int x=0);			
/**
	�ȴ�����
*/
void WaitKey(DWORD Key);
/**
	��ʱ����
	@param time:�ӳ�ʱ��
*/

/**
	��ȡ�ļ�����
	@param name:�ļ���
*/
int GetFileLength(char *name);		
/**
	�ж��ļ��Ƿ����
	@param name:�ļ���
*/
bool CheckFile(char *name);			

/**
	����������Ϣ
*/
void ShowMessage(char *msg,...);	
/**
	���������Ϣ
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
	���ļ�����·��������ļ�������·��
	@param filename:��·�����ļ���
	@return ����·�����ļ���
*/
char* GetFileName(char* filename);	
/**
	���ļ�����ȡ�ļ�·��
	@param filename:�ļ���
	@return �ļ�·��
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

//********************************************
//	@Fielname:Wxm_IniFile.cpp
//  @Descript:ini文件操作类
//  @Create 2010.12.7
//  @Author:OneDouble
//********************************************

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <io.h>
#include <fcntl.h>
#include  <mmsystem.h>
#include "Wxm_IniFile.h"
#include "../PublicLib/PublicLib.h"

#pragma comment(lib,"winmm.lib");

//无参构造
Wxm_IniFile::Wxm_IniFile()
{
	m_DataLen=0;
	m_pData=NULL;
	m_IndexNum=0;
	m_pIndexList=NULL;
}

/**
	有参构造
	@param filename 文件名
*/
Wxm_IniFile::Wxm_IniFile(char *filename)
{
	m_DataLen=0;
	m_pData=NULL;
	m_IndexNum=0;
	m_pIndexList=NULL;
	Open(filename);
}

//析构
Wxm_IniFile::~Wxm_IniFile()
{
	if( m_DataLen != 0 && m_pData != NULL )
	{
		_DELETE( m_pData );
	}

	if( m_IndexNum != 0 && m_pIndexList != NULL )
	{
		_DELETE( m_pIndexList );
	}
}

/**
	读入文件
	@param filename 被读入的文件名
*/
bool Wxm_IniFile::Open(char *filename)
{
	strcpy(m_FileName, filename);

	_DELETE( m_pData );

	m_DataLen=GetFileLength(filename);	//获取文件长度
	if( m_DataLen > 0 )		//文件存在
	{
		m_pData=new char[m_DataLen];

		FILE *fp;
		fp=fopen(filename, "rb");
		fread(m_pData, m_DataLen, 1, fp);		//读数据
		fclose(fp);

		//初始化索引
		InitIndex();
	}
	else
	{
		m_DataLen=1;
		m_pData=new char[m_DataLen];
		memset(m_pData, 0, 1);
		InitIndex();
	}

	return false;
}

/** 
	保存文件
	@param filename:文件名
*/
bool Wxm_IniFile::Save(char *filename)
{
	if( filename==NULL )
		filename=m_FileName;

	FILE *fp;
	fp=fopen(filename, "wb");
	if( fp==NULL )
	{
		ShowMessage("Can't save %s ",filename);
		return false;
	}

	fwrite(m_pData, m_DataLen, 1, fp);
	fclose(fp);

	return true;
}

/** 
	计算出所有的索引位置并保存至m_pIndexList
*/
void Wxm_IniFile::InitIndex()
{
	m_IndexNum=0;

	for(int i=0; i<m_DataLen; i++)
	{
		//找到,要以[开头，而且段与段之间要用回车分开
		if( m_pData[i]=='[' && (i==0||(i>0&&m_pData[i-1]=='\n')  ) )
		{
			m_IndexNum++;
		}
	}

	//申请内存
	_DELETE( m_pIndexList );
	if( m_IndexNum>0 )
		m_pIndexList=new int[m_IndexNum];

	int n=0;
	//初始化
	for(int i=0; i<m_DataLen; i++)
	{
		if( m_pData[i]=='[' && ((i>0&&m_pData[i-1]=='\n' )|| i==0) )
		{
			m_pIndexList[n]=i+1;		//保存到索引表的位置是 [ 的下一个字符的位置
			n++;
		}
	}
}

/**
	返回指定标题位置
	@param string 值
	@return 索引所在的位置
*/
int Wxm_IniFile::FindIndex(char *string)
{
	for(int i=0; i<m_IndexNum; i++)
	{
		char *str=ReadData( m_pIndexList[i] );		//遍历索引表，读所以的索引
		if( strcmp(string, str) == 0 )			//比较，如果向同就返回标题的位置
		{
			_FREE( str );
			return m_pIndexList[i];				//索引所在的位置，是 [ 的字符的下一个位置
		}
		_FREE( str );
	}
	return -1;
}

/** 
	返回指定数据的内容的位置
	@param index 索引号
	@param string 数据键
	@return 键的位置
*/
int Wxm_IniFile::FindKey(int index, char *string)
{
	int p=index;	//指针

	while(1)
	{									//跳过标题(索引)所在的行
		p=GotoNextLine(p);				//下一行的位置
		char *name=ReadKey(p);		//读一个名字，注意p是按地址的方式传参的
		if( strcmp(string, name)==0 )	//相同就返回位置，数据内容开始的位置
		{
			_FREE( name );
			return p;
		}

		_FREE( name );
		if( p>=m_DataLen ) return -1;		//没有找到
	}
	return -1;
}

/** 
	走到下一行
	@param p 位置
	@return 下一行索引
*/
int Wxm_IniFile::GotoNextLine(int p)
{
	int i;
	for(i=p; i<m_DataLen; i++)
	{
		if( m_pData[i]=='\n' )				//回车换行
			return i+1;

	}
	return i;
}

/**
	在指定位置读一数据名称
	@param p 位置引用，用完走到value位置
	@return 指定位置数据键
*/
char *Wxm_IniFile::ReadKey(int &p)			//引用参数，将通过p返回，而其值是
{											//数据的内容的位置
	char chr;
	char *Ret;
	int m=0;

	Ret=(char *)malloc(256);
	memset(Ret, 0, 256);

	for(int i=p; i<m_DataLen; i++)
	{
		chr=m_pData[i];

		//结束
		if( chr == '\r' )
		{
			p=i+1;			
			return Ret;
		}
		
		//结束
		if( chr == '=' || chr == ';' )
		{
			p=i+1;						//指向数据的内容，跳过了分隔符
			return Ret;
		}
		
		Ret[m]=chr;
		m++;
	}
	return Ret;
}

/** 
	在指定位置读一行字符串
	@param p  行索引号
*/
char *Wxm_IniFile::ReadData(int p)
{
	char chr;
	char *Ret;
	int n=p, m=0;

	int EndLine=GotoNextLine(p);		//下一行的位置，只读一行的内容
	Ret=(char *)malloc(EndLine-p+1);	//分配内存
	memset(Ret, 0, EndLine-p+1);		//清0

	for(int i=0; i<m_DataLen-p; i++)
	{
		chr=m_pData[n];

		//结束			用分号，回车，TAB或者是]结束
		if( chr == ';' || chr == '\r' || chr == '\t' || chr == ']' )
		{
			return Ret;
		}
		
		Ret[m]=chr;		//读入到缓冲区
		m++;
		n++;
	}
	return Ret;
}

/** 
	以普通方式读一字符串数据
	@param 索引名
	@param 数据名
	@return 目标字符串
*/
char *Wxm_IniFile::ReadValue(char *index, char *name)
{
	int n=FindIndex(index);				//通过索引名字找到位置
	if( n == -1 )
	{
		ShowMessage("Can't find [%s] in file '%s'!", index, m_FileName);
		return "";
	}

	int m=FindKey(n, name);			//通过索引的位置和数据名字找到数据内容的位置
	if( m==-1 )
	{
		ShowMessage("Can't find [%s]-'%s' in file '%s'!", index, name, m_FileName);
		return "";
	}

	return ReadData(m);					//读出该内容
}
	
/** 
	在指定的行读一字符串，设计这个函数是用于读出一批数据，
	而且可以不关心具体的名字，可以用循环的方式读
	@param 索引号
	@param 行号
*/
char *Wxm_IniFile::ReadValue(char *index, int lines)
{
	int n=FindIndex(index);				//通过索引名字找到位置
	if( n == -1 )
	{
		ShowMessage("Can't find [%s] in file '%s'!", index, m_FileName);
		return "";
	}

	//跳到指定行数
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_DataLen )
			n=GotoNextLine(n);
	}

	//读数据
	while( n<=m_DataLen )
	{
		if( m_pData[n] == '=' )		//找到分隔符 =
		{
			n++;					//移动到=的下一个字符
			return ReadData(n);		//读出内容
		}
		if( m_pData[n] == '\r' )
		{
			return "";				//没有找到
		}
		n++;
	}

	return "";
}

/**
	以普通方式读一int数据
	@param index 索引号
	@param name 数据键
*/
int Wxm_IniFile::ReadValueInt(char *index, char *name)
{
	int n=FindIndex(index);			
	if( n == -1 )
	{
		ShowMessage("Can't find [%s] in file <%s>",index, m_FileName);
		return ERROR_DATA;
	}

	int m=FindKey(n, name);		//看看上面的注释
	if( m==-1 )
	{
		ShowMessage("Can't find [%s] '%s' in file <%s>",index, name, m_FileName);
		return ERROR_DATA;
	}

	char *str=ReadData(m);			
	int ret=atoi(str);				//转化成整数
	_FREE(str);
	return ret;
}

/** 
	在指定的行读一整数，同样是为了成批的读取，而且可以不关心具体的名字
	@param index:索引号
	@param lines:行号
*/
int Wxm_IniFile::ReadValueInt(char *index, int lines)
{
	int n=FindIndex(index);			//通过索引名字找到位置
	if( n == -1 )
	{
		ShowMessage("Can't find [%s] in file <%s>",index, m_FileName);
		return ERROR_DATA;
	}

	//跳到指定行数
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_DataLen )
			n=GotoNextLine(n);
	}

	//读数据
	while( n<m_DataLen )
	{
		if( m_pData[n] == '=' )		
		{
			n++;
			char *str=ReadData(n);	
			int ret=atoi(str);
			_FREE(str);
			return ret;
		}
		if( m_pData[n] == '\r' )
		{
			return ERROR_DATA;
		}
		n++;
	}

	return ERROR_DATA;
}

/** 
	在指定的行读一数据名称，目的还是为了批量获取、
	@param index 索引号
	@param lines 行号
*/
char *Wxm_IniFile::ReadKey(char *index, int lines)
{
	int n=FindIndex(index);			
	if( n == -1 )
	{
		ShowMessage("Can't find [%s] in file <%s>",index, m_FileName);
		return NULL;
	}

	//跳到指定行数
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_DataLen )
			n=GotoNextLine(n);
	}

	return ReadKey(n);			//读出名字返回
}

/** 
	以普通方式写一字符串数据。索引，名字，内容
	@param index 索引号
	@param name 数据键
	@param string 数据值
*/
bool Wxm_IniFile::WriteValue(char *index, char *name, char *string)
{
	int n=FindIndex(index);			
	if( n == -1 )	//新建索引
	{
		AddIndex(index);			//加入一个一个索引
		n=FindIndex(index);			//找到其位置
		AddData(n, name, string);	//在当前位置n加一个数据
		return true;
	}

	//存在索引
	int m=FindKey(n, name);
	if( m==-1 )		//新建数据
	{
		AddData(n, name, string);	//在当前位置n加一个数据
		return true;
	}

	//存在数据
	ModityData(n, name, string);	//修改一个数据

	return true;
}

/** 
	以普通方式写一整数
	@param index 索引号
	@param name 数据键
	@param string 数据值（int）
*/
bool Wxm_IniFile::WriteValueInt(char *index, char *name, int num)
{
	char string[32];
	sprintf(string, "%d", num);

	int n=FindIndex(index);			
	if( n == -1 )	//新建索引
	{
		AddIndex(index);			//看上面的注释
		n=FindIndex(index);
		AddData(n, name, string);	//在当前位置n加一个数据
		return true;
	}

	//存在索引
	int m=FindKey(n, name);
	if( m==-1 )		//新建数据
	{
		AddData(n, name, string);	//在当前位置n加一个数据
		return true;
	}

	//存在数据
	ModityData(n, name, string);	//修改一个数据

	return true;
}



/** 
	添加一个索引
*/
bool Wxm_IniFile::AddIndex(char *index)
{
	char str[256];
	memset(str, 0, 256);
	int n=FindIndex(index);			

	if( n == -1 )	//新建索引
	{
		sprintf(str,"\r\n[%s]\r\n",index);		//注意格式，索引要分开
		m_pData=(char *)realloc(m_pData, m_DataLen+strlen(str));	//重新分配内存
		sprintf(&m_pData[m_DataLen], "%s", str);		//接在后面
		m_DataLen+=strlen(str);					//更新长度

		InitIndex();							//重新建立索引表
		return true;
	}
	
	return false;	//已经存在
}

/** 
	在当前位置加入一个数据
	@param p 位置
	@param name 数据键
	@param string 数据值
*/
bool Wxm_IniFile::AddData(int p, char *name, char *string)
{
	char *str;
	int len=strlen(string);
	str=new char[len+256];						//留出空间
	memset(str, 0, len+256);
	sprintf(str,"%s=%s\r\n",name,string);		//化成tiamo=1的格式
	len=strlen(str);							//重新算长度

	p=GotoNextLine(p);	//提行
	m_pData=(char *)realloc(m_pData, m_DataLen+len);	//重新分配内存

	char *temp=new char[m_DataLen-p];				//后面的内容放到temp里面
	memcpy(temp, &m_pData[p], m_DataLen-p);
	memcpy(&m_pData[p+len], temp, m_DataLen-p);		//把后面的搬到末尾
	memcpy(&m_pData[p], str, len);					//插入新加的内容
	m_DataLen+=len;								//更新数据长度

	_DELETE( temp );
	_DELETE( str );
	return true;
}

/** 
	在当前位置修改一个数据的值
	@param p 位置
	@param name 数据键
	@param string 数据值
*/
bool Wxm_IniFile::ModityData(int p, char *name, char *string)
{
	int n=FindKey(p, name);					//找到数据。注意：返回的n是内容的位置

	char *t=ReadData(n);						//读出数据的内容
	p=n+strlen(t);								//移动到数据的末尾的下一个位置
	_FREE(t);

	int newlen=strlen(string);					//新的数据长度
	int oldlen=p-n;								//t的长度，也就是原来的数据长度

	m_pData=(char *)realloc(m_pData, m_DataLen+newlen-oldlen);	//重新分配内存

	char *temp=new char[m_DataLen-p];						//相同的方法
	memcpy(temp, &m_pData[p], m_DataLen-p);
	memcpy(&m_pData[n+newlen], temp, m_DataLen-p);			//把后面的搬到末尾
	memcpy(&m_pData[n], string, newlen);
	m_DataLen+=newlen-oldlen;								//更新数据长度

	_DELETE( temp );
	return true;
}

/** 
	返回全部文件内容
*/
char *Wxm_IniFile::GetData()
{
	return m_pData;
}

/** 
	获得文件的行数
	@param cur 得给函数一个最大搜索值
*/
int Wxm_IniFile::GetLines(int cur)
{
	int n=1;
	for(int i=0; i<cur; i++)
	{
		if( m_pData[i]=='\n' )					//找到\n，就是新的一行
			n++;
	}
	return n;
}

/** 
	返回连续的行数，就是标题(索引)这个段共有多少行，一般是用来循环记数的
	@param index 索引名
*/
int Wxm_IniFile::GetContinueDataNum(char *index)
{
	int num=0;
	int n=FindIndex(index);				//恩…………
	n=GotoNextLine(n);					//跳过标题(索引)
	while(1)
	{									//要是一行的开头就是下面的字符的话，就结束
		if( m_pData[n] == '\r' || m_pData[n] == -3 || m_pData[n] == EOF 
			|| m_pData[n] == ' ' || m_pData[n] == '/' || m_pData[n] == '\t' || m_pData[n] == '\n' )
		{
			return num;
		}
		else
		{
			num++;						//加一
			n=GotoNextLine(n);			//到下一行的开头
			if( n >= m_DataLen )	
				return num;			//文件结束
		}
	}
}

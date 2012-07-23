#ifdef INIFILE_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

#ifndef _WXM_INI_H
#define _WXM_INI_H

class  DLL_EXPORT  Wxm_IniFile{
private:
	char m_FileName[MAX_PATH];	//文件名
	int m_DataLen;			//文件长度
	char *m_pData;				//文件内容

	int m_IndexNum;			//索引数目（[]的数目）
	int *m_pIndexList;			//索引点位置列表

	int m_Point;				//当前指针
	int m_Line, m_Word;			//当前行，当前列

public:
	Wxm_IniFile();
/**
	有参构造
	@param filename 被打开的文件名
*/
	Wxm_IniFile(char *);		
	~Wxm_IniFile();				

	char *GetData();		//返回文件内容
	int GetLines(int);			//返回文件的行数
/**
	读入文件
	@param filename 被读入的文件名
*/
	bool Open(char *);		
/** 
	保存文件
	@param filename:文件名
*/
	bool Save(char *filename=NULL);		

private:
	
/** 
	计算出所有的索引位置并保存至m_pIndexList
*/
	void InitIndex();			
/**
	返回指定标题位置
	@param string 值
	@return 索引所在的位置
*/
	int FindIndex(char *);		
/** 
	返回指定数据的内容的位置
	@param index 索引号
	@param string 数据值
	@return 目标位置
*/
	int FindKey(int, char *);	
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
	char *ReadKey(int &);	
/** 
	在指定位置读一行字符串
	@param p  行索引号
*/
	char *ReadData(int);

/** 
	添加一个索引
*/
	bool AddIndex(char *);		
/** 
	在当前位置加入一个数据
	@param p 位置
	@param name 数据键
	@param string 数据值
*/
	bool AddData(int, char *, char *);	
/** 
	在当前位置修改一个数据的值
	@param p 位置
	@param name 数据键
	@param string 数据值

*/
	bool ModityData(int, char *, char *); 

public:


/** 
	以普通方式读一字符串数据
	@param 索引名
	@param 数据名
	@return 目标字符串
*/
	char *ReadValue(char *, char *);	
/** 
	在指定的行读一整数，同样是为了成批的读取，而且可以不关心具体的名字
	@param index:索引号
	@param lines:行号
*/
	int ReadValueInt(char *, int );	
/**
	以普通方式读一int数据
	@param index 索引号
	@param name 数据键
*/
	int ReadValueInt(char *, char *);	

/** 
	在指定的行读一字符串，设计这个函数是用于读出一批数据，
	而且可以不关心具体的名字，可以用循环的方式读
	@param 索引号
	@param 行号
	@return 数据值
*/
	char *ReadValue(char *, int);	
	/** 
	在指定的行读一数据键，目的还是为了批量获取、
	@param index 索引号
	@param lines 行号
	@return 数据键
*/
	char *ReadKey(char *, int);	
/** 
	以普通方式写一整数
	@param index 索引号
	@param name 数据键
	@param string 数据值（int）
*/
	bool WriteValueInt(char *, char *, int);	
	/** 
	以普通方式写一字符串数据。索引，名字，内容
	@param index 索引号
	@param name 数据键
	@param string 数据值
*/
	bool WriteValue(char *, char *, char *);	

	/** 
	返回连续的行数，就是标题(索引)这个段共有多少行，一般是用来循环记数的
	@param index 索引名
*/
	int GetContinueDataNum(char *);			//返回连续的行数（从INDEX到第一个空行）
};

#endif
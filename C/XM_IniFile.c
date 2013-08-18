/***********************************************************************
Copyright:2010-2013 Double One<dfghj77777@gmail.com>

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
@File Name:Wxm_IniFile.cc
@Describe:Ini W/S lib ,Now Support for WINDOWS,LINUX,MAC,single thread
@Create Date:2010-12-7 14:01:29
@Version:1.0
@Author：Double One
@Blog： http://onedouble.cnblogs.com
@Modifier: Double One (dfghj77777@gmail.com)
@Modify Date：2013-08-02 14:01:30
@Modify Reason：
***********************************************************************/
#include "XM_IniFile.h"
#include <assert.h>

char g_FileName[MAX_PATH];	//文件名

char *g_pData;				//文件全文内容
int  g_DataLen;			//文件长度

int g_IndexCount;			//索引数目（[]的数目）
int *g_pIndexList;			//索引点位置列表（一级菜单）

int g_Point;				//当前指针
int g_CurLine, g_CurWord;			//当前行列
int g_FileType = _DEFAULT_FILE_TYPE;//用于做行尾结束符判断
char* g_NewLine[] = {"\r\n","\n","\r","\n\r"};
/**
	判断文件换行符类型，找不到就用操作系统类型，
*/
int GetFileType(){
	for(int i = 0; i < g_DataLen; i++) {
		if( g_pData[i] == '\r' ){
			if (g_pData[i+1] == '\n')//crlf,must be windows
				return eWINDOWS;
			else//only \r
				return eMAC;
		}
		else if (g_pData[i] == '\n' ){
			if (g_pData[i+1] == '\r')//lfcr,a cold type
				return eWINDOWS;
			else//only \n
				return eLINUX;
		}
	}
	return _DEFAULT_FILE_TYPE;//不太可能进入这个环节，除非文件没有换行。
}
//指定位置前面否是换行符
bool IsAfterNewLine(int p){
	switch (g_FileType)	{
		case eWINDOWS:
			if (p > 2)
				if (g_pData[p-2] == '\r' && g_pData[p-1] == '\n' )
					return true;
		case eLINUX:
			if (p > 1)
				if ( g_pData[p-1] == '\n')
					return true;
		case eMAC:
			if (p > 1)
				if ( g_pData[p-1] == '\r')
					return true;
		case eLFCR:
			if (p > 2)
				if (g_pData[p-2] == '\n' && g_pData[p-1] == '\r' )
					return true;
		default:
			break;
	}
	return false;
}
/**
获取文件长度
*/
int GetFileLength(char *name) {
	int fh, nbytes;

	fh = _open( name, _O_RDONLY );	//只读模式
	if( fh == -1 ) {
		return -1;
	}

	nbytes = _filelength(fh);
	_close(fh);

	return nbytes;
}


/**
判断文件是否存在
@param name:文件名
*/
bool CheckFile(char *name) {
	//查找文件
	WIN32_FIND_DATA fdt;
	HANDLE hFile = FindFirstFile(name, &fdt);

	if( hFile == INVALID_HANDLE_VALUE )
		return false;
	else {
		FindClose(hFile);
		return true;
	}
}

/**
弹出调试信息
*/
void ShowMessage(char *msg, ...) {
	va_list va;
	char str[256];

	va_start(va, msg);
	vsprintf(str, msg, va);
	va_end(va);

	//here wait for add message box

}

/**
输出调试信息
*/
void PutDebugString(char *msg) {
	static int num = 0;
	char t[16];

	FILE *fp;
	if( (fp = fopen("debug.txt", "a+")) == NULL ) {
		ShowMessage("Can't open Debug.txt (%s)", msg);
		return;
	}

	num++;
	sprintf(t, "\n%d:", num);
	fseek(fp, 0, SEEK_END);
	fwrite(t, strlen(t), 1, fp);
	fwrite(msg, strlen(msg), 1, fp);
	fclose(fp);
}
/**
	有参构造
	@param filename 文件名
*/
void Clear() {
    g_DataLen = 0;
    _FREE( g_pData );
    g_IndexCount = 0;
    _FREE( g_pIndexList );
	g_Point = g_CurLine = g_CurWord = 0;
	g_FileType = _DEFAULT_FILE_TYPE;
}

/**
	读入文件
	@param filename 被读入的文件名
*/
bool Open(char *filename) {
    int nSrcLen = strlen(filename);
    if ( MAX_PATH <= nSrcLen)
        strcpy_s(g_FileName, nSrcLen, filename);
    else
        return false;//输入过长

	Clear();//先清理了

    g_DataLen = GetFileLength(g_FileName);
    //有内容存在
    if( g_DataLen > 0 )	{
        g_pData = _MALLOC(char,g_DataLen + 1);//多个"\0"
        FILE *fp = fopen(g_FileName, "rb");
        fread(g_pData, g_DataLen, 1, fp);		//读数据
        fclose(fp);
		g_FileType= GetFileType();//判断文件类型是dos还是unix
    } else {
		PutDebugString("a empty file");
        g_DataLen = 1;
        g_pData = _MALLOC(char,g_DataLen);
        memset(g_pData, 0, 1);
		g_FileType = _DEFAULT_FILE_TYPE;
    }

	//初始化索引
	InitIndex();
	return true;
}

/**
	保存文件
	@param filename:文件名
*/
bool Save(char *filename) {
    if( filename == NULL )
        filename = g_FileName;

    FILE *fp = fopen(filename, "wb");
    if( fp == NULL )	{
        ShowMessage("Can't save %s ", filename);
        return false;
    }

    fwrite(g_pData, g_DataLen, 1, fp);
    fclose(fp);

    return true;
}

/**
	计算出所有的索引位置并保存至m_pIndexList
*/
void InitIndex() {
    g_IndexCount = 0;

    for(int i = 0; i < g_DataLen; i++)	{
        //找到,要以[开头，而且前一个字符就是回车
        if( g_pData[i] == '[' && (i == 0 || IsAfterNewLine(i) ) ) {
            g_IndexCount++;
        }
    }

    //申请内存
    _FREE( g_pIndexList );
    if( g_IndexCount > 0 )
        g_pIndexList = _MALLOC(int,g_IndexCount);

    int n = 0;
    //初始化
    for(int i = 0; i < g_DataLen; i++){
        if( g_pData[i] == '[' && ( i == 0 || IsAfterNewLine(i) ) ) {
            g_pIndexList[n] = i + 1;		//保存到索引表的位置是 [ 的下一个字符的位置
            n++;
        }
    }
}


/**
	返回指定标题位置
	@param string 值
	@return 索引所在的位置
*/
int FindIndex(const char *string) {
    for(int i = 0; i < g_IndexCount; i++) {
        char *str = ReadKey( g_pIndexList[i] );		//遍历索引表，读所以的索引
        if( strcmp(string, str) == 0 )	{		//比较，如果向同就返回标题的位置
            _FREE( str );
            return g_pIndexList[i];				//索引所在的位置，是 [ 的字符的下一个位置
        }
        _FREE( str );
    }
    return -1;
}

/**
返回连续的行数，就是标题(索引)这个段共有多少行，一般是用来循环记数的
@param index 索引名
*/
int GetCountByIndex(const char *index) {
	int num = 0;
	int n = FindIndex(index);				//找到索引位置
	n = GotoNextLine(n);					//跳过标题(索引)
	while(1) {
		//要是一行的开头就是下面的字符的话，就结束
		if( g_pData[n] == '\r' || g_pData[n] == -3 || g_pData[n] == EOF
			|| g_pData[n] == ' ' || g_pData[n] == '/' || g_pData[n] == '\t' || g_pData[n] == '\n' ) {
				return num;
		} else {
			num++;
			n = GotoNextLine(n);			//到下一行的开头
			if( n >= g_DataLen )
				return num;			//文件结束
		}
	}
}

/**
	返回指定数据的内容的位置
	@param index 索引号
	@param string 数据键
	@return 键的位置
*/
int FindKey(int index,const char *string) {
    int p = index;	//指针

    while(true) {
        //跳过标题(索引)所在的行
        p = GotoNextLine(p);				//下一行的位置
        char *name = ReadKey(p);		//读一个名字，注意p是按地址的方式传参的
        if( strcmp(string, name) == 0 ) {	//相同就返回位置，数据内容开始的位置
            _FREE( name );
            return p;
        }

        _FREE( name );
        if( p >= g_DataLen ) return -1;		//没有找到
    }
    return -1;
}

/**
	在指定位置读一数据名称
	@param p 位置引用，用完走到value位置
	@return 指定位置数据键名
*/
char* ReadKey(OUT int &p) {		//引用参数，将通过p返回，而其值是
    char* Ret = _MALLOC( char ,MAX_PATH );
    memset(Ret, 0, MAX_PATH);
    //数据的内容的位置
    int m = 0;
	char chr;
    for(int i = p; i < g_DataLen; i++) {
        chr = g_pData[i];
		
		if (g_FileType == eWINDOWS)
		{
			if ( '\r' == chr && '\n' == g_pData[i + 1] ){//windows \r\n
				p = i + 2;						
				break;
			}
		}
		else if (g_FileType == eLFCR)
		{
			if ( '\n' == chr && '\r' == g_pData[i + 1] ){//lfcr \r\n
				p = i + 2;
				break;
			}
		}


        //遇到结束符，走人
        if( chr == '=' || chr == ';' || chr == '\n' || chr == '\r' ) {
            p = i + 1;						//指向数据的内容，跳过了分隔符
            break;
        }

        Ret[m] = chr;//逐字节读入
        m++;
    }
    return Ret;
}

/**
在指定的行读一数据名称，目的还是为了批量获取、
@param index 索引号
@param lines 行号
*/
char* ReadKeyByLine(const char *index, int lines) {
	int n = FindIndex(index);
	if( n == -1 ) {
		ShowMessage("Can't find [%s] in file <%s>", index, g_FileName);
		return NULL;
	}

	//跳到指定行数
	n = GotoNextLine(n);
	for(int i = 0; i < lines; i++) {
		if( n < g_DataLen )
			n = GotoNextLine(n);
	}

	return ReadKey(n);			//读出名字返回
}

/**
	在指定位置读一行字符串
	@param p  行索引号
*/
char* ReadLine(int p) {
    char chr;
    char *Ret;
    int n = p, m = 0;

    int EndLine = GotoNextLine(p);		//下一行的位置，只读一行的内容
    Ret = (char *)malloc(EndLine - p + 1);	//分配内存
    memset(Ret, 0, EndLine - p + 1);		//清0

    for(int i = 0; i < g_DataLen - p; i++) {
        chr = g_pData[n];

        //结束			用分号，回车，TAB或者是]结束
        if( chr == ';' || chr == '\n' ||chr == '\r' || chr == '\t' || chr == ']' ) {
            return Ret;
        }

        Ret[m] = chr;		//读入到缓冲区
        m++;
        n++;
    }
    return Ret;
}

/**
	以普通方式读一字符串数据
	@param 索引名
	@param 键名
	@return 目标字符串
*/
char* ReadValue(const char *index,const char *name) {
    int n = FindIndex(index);				//通过索引名字找到位置
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file '%s'!", index, g_FileName);
        return "";
    }

    int m = FindKey(n, name);			//通过索引的位置和键名字找到数据内容的位置
    if( m == -1 ) {
        ShowMessage("Can't find [%s]-'%s' in file '%s'!", index, name, g_FileName);
        return "";
    }

    return ReadKey(m);					//读出该内容
}

/**
	在指定的行读一字符串，设计这个函数是用按序号于读出一批数据，
	而且可以不关心具体的名字，可以用循环的方式读
	@param index 索引号
	@param lines 行号，从索引所在行开始计算
*/
char* ReadValueByLine(const char *index, int lines) {
    int n = FindIndex(index);				//通过索引名字找到位置
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file '%s'!", index, g_FileName);
        return "";
    }

    //跳到指定行数
    n = GotoNextLine(n);
    for(int i = 0; i < lines; i++) {
        if( n < g_DataLen )
            n = GotoNextLine(n);
    }

    //读数据
    while( n <= g_DataLen ) {
        if( g_pData[n] == '=' ) {	//找到分隔符 =
            n++;					//移动到=的下一个字符
            return ReadKey(n);		//读出内容
        }
        if( g_pData[n] == '\r' || g_pData[n] == '\n') {
            return "";				//没有找到分隔符就看到换行符了
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
int ReadValueInt(const char *index,const char *name) {
    int n = FindIndex(index);
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file <%s>", index, g_FileName);
        return ERROR_DATA;
    }

    int m = FindKey(n, name);		//看看上面的注释
    if( m == -1 ) {
        ShowMessage("Can't find [%s] '%s' in file <%s>", index, name, g_FileName);
        return ERROR_DATA;
    }

    char *str = ReadKey(m);
    int ret = atoi(str);				//转化成整数
    _FREE(str);
    return ret;
}

/**
	在指定的行读一整数，同样是为了按序号成批的读取，而且可以不关心具体的名字
	@param index:索引号
	@param lines:行号
*/
int ReadValueIntByLine(const char *index, int lines) {
    int n = FindIndex(index);			//通过索引名字找到位置
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file <%s>", index, g_FileName);
        return ERROR_DATA;
    }

    //跳到指定行数
    n = GotoNextLine(n);
    for(int i = 0; i < lines; i++) {
        if( n < g_DataLen )
            n = GotoNextLine(n);
    }

    //读数据
    while( n < g_DataLen ) {
        if( g_pData[n] == '=' ) {
            n++;
            char *str = ReadKey(n);
            int ret = atoi(str);
            _FREE(str);
            return ret;
        }
        if( g_pData[n] == '\r' && g_pData[n] == '\n') {//没有找到分隔符就看到换行符了
            return ERROR_DATA;
        }
        n++;
    }

    return ERROR_DATA;
}

/**
走到指定位置的下一行,实际是找到下一个"\n"
@param p 位置
@return 下一行索引
*/
int GotoNextLine(int p) {
	for(; p < g_DataLen; p++) {
		if (g_FileType == eWINDOWS){
			if(g_pData[p] == '\r') {
				if (p < g_DataLen ){
					if(g_pData[p+1] == '\n'){
						return p+2;
					}
				}
			}
		}
		else if (g_FileType == eLINUX){
			if(g_pData[p] == '\n') 
				return p + 1;
		}
		else if (g_FileType == eMAC){
			if(g_pData[p] == '\r') 
				return p + 1;
		}
		else if (g_FileType == eLFCR){
			if(g_pData[p] == '\n') {
				if (p < g_DataLen -1){
					if(g_pData[p+1] == '\r'){
						return p+2;
					}
				}
			}
		}
		else{
			ShowMessage("Unkown File Type@GetLineCount");
		}
	}
	return p;
}

/**
获得文件的行数
@param cur 得给函数一个最大搜索值
*/
int GetLineCount(int cur) {
	int n = 1;
	for(int i = 0; i < cur; i++){
		i = GotoNextLine(i);
		if( i - 1 <= cur )
			n++;
	}
	return n;
}

/**
返回全部文件内容,慎用！
*/
char* GetData() {
	return g_pData;
}

/**
在当前位置修改一个数据的值
@param p 位置
@param name 数据键
@param string 数据值
*/
bool ModityData(int p,const char *name,const char *string) {
	int n = FindKey(p, name);					//找到数据。注意：返回的n是内容的位置

	char *t = ReadKey(n);						//读出数据的内容
	p = n + strlen(t);								//移动到数据的末尾的下一个位置
	_FREE(t);

	int newlen = strlen(string);					//新的数据长度
	int oldlen = p - n;								//t的长度，也就是原来的数据长度

	g_pData = (char *)realloc(g_pData, g_DataLen + newlen - oldlen);	//重新分配内存

	char *temp = _MALLOC(char,g_DataLen - p);						//相同的方法
	memcpy(temp, &g_pData[p], g_DataLen - p);					//取出后段
	memcpy(&g_pData[n + newlen], temp, g_DataLen - p);			//把后面的搬到末尾
	memcpy(&g_pData[n], string, newlen);						//覆写新段
	g_DataLen += newlen - oldlen;								//更新数据长度

	_FREE( temp );
	return true;
}

/**
以普通方式写一字符串数据。索引，名字，内容
@param index 索引号
@param name 数据键
@param string 数据值
*/
bool WriteValue(const char *index, const char *name,const char *string) {
	int n = FindIndex(index);
	if( n == -1 ) {	//新建索引
		AddIndex(index);			//加入一个一个索引
		n = FindIndex(index);			//找到其位置
		AddData(n, name, string);	//在当前位置n加一个数据
		return true;
	}

	//存在索引
	int m = FindKey(n, name);
	if( m == -1 ) {	//新建数据
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
bool WriteValueInt(const char *index,const char *name, int num) {
	char string[32];
	sprintf(string, "%d", num);

	int n = FindIndex(index);
	if( n == -1 ) {	//新建索引
		AddIndex(index);			//看上面的注释
		n = FindIndex(index);
		AddData(n, name, string);	//在当前位置n加一个数据
		return true;
	}

	//存在索引
	int m = FindKey(n, name);
	if( m == -1 ) {	//新建数据
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
bool AddIndex(const char *index) {
	char str[MAX_PATH];
	memset(str, 0, MAX_PATH);
	int n = FindIndex(index);

	if( n == -1 ) {	//新建索引
		_ASSERT(g_FileType < eOHTER);
		sprintf(str, "%s[%s]%s", index , g_NewLine[g_FileType]);		//注意格式，索引要分开
		g_pData = (char *)realloc(g_pData, g_DataLen + strlen(str));	//重新分配内存
		sprintf(&g_pData[g_DataLen], "%s", str);		//接在后面
		g_DataLen += strlen(str);					//更新长度

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
bool AddData(int p,const char *key, const char *value) {
	int len=strlen(value);
	char str[MAX_PATH];						//留出空间
	memset(str, 0, MAX_PATH);
	_ASSERT(g_FileType < eOHTER);
	sprintf(str,"%s=%s%s",key,value,g_NewLine[g_FileType]);		//化成tiamo=1的格式
	len=strlen(str);							//重新算长度

	p=GotoNextLine(p);	//提行
	g_pData=(char *)realloc(g_pData, g_DataLen+len);	//重新分配内存

	char* temp = _MALLOC(char,g_DataLen-p);				//后面的内容放到temp里面
	memcpy(temp, &g_pData[p], g_DataLen-p);
	memcpy(&g_pData[p+len], temp, g_DataLen-p);		//把后面的搬到末尾
	memcpy(&g_pData[p], str, len);					//插入新加的内容
	g_DataLen+=len;								//更新数据长度

	_FREE(temp);

	return true;
}

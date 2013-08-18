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
@Author��Double One
@Blog�� http://onedouble.cnblogs.com
@Modifier: Double One (dfghj77777@gmail.com)
@Modify Date��2013-08-02 14:01:30
@Modify Reason��
***********************************************************************/
#include "XM_IniFile.h"
#include <assert.h>

char g_FileName[MAX_PATH];	//�ļ���

char *g_pData;				//�ļ�ȫ������
int  g_DataLen;			//�ļ�����

int g_IndexCount;			//������Ŀ��[]����Ŀ��
int *g_pIndexList;			//������λ���б�һ���˵���

int g_Point;				//��ǰָ��
int g_CurLine, g_CurWord;			//��ǰ����
int g_FileType = _DEFAULT_FILE_TYPE;//��������β�������ж�
char* g_NewLine[] = {"\r\n","\n","\r","\n\r"};
/**
	�ж��ļ����з����ͣ��Ҳ������ò���ϵͳ���ͣ�
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
	return _DEFAULT_FILE_TYPE;//��̫���ܽ���������ڣ������ļ�û�л��С�
}
//ָ��λ��ǰ����ǻ��з�
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
��ȡ�ļ�����
*/
int GetFileLength(char *name) {
	int fh, nbytes;

	fh = _open( name, _O_RDONLY );	//ֻ��ģʽ
	if( fh == -1 ) {
		return -1;
	}

	nbytes = _filelength(fh);
	_close(fh);

	return nbytes;
}


/**
�ж��ļ��Ƿ����
@param name:�ļ���
*/
bool CheckFile(char *name) {
	//�����ļ�
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
����������Ϣ
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
���������Ϣ
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
	�вι���
	@param filename �ļ���
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
	�����ļ�
	@param filename ��������ļ���
*/
bool Open(char *filename) {
    int nSrcLen = strlen(filename);
    if ( MAX_PATH <= nSrcLen)
        strcpy_s(g_FileName, nSrcLen, filename);
    else
        return false;//�������

	Clear();//��������

    g_DataLen = GetFileLength(g_FileName);
    //�����ݴ���
    if( g_DataLen > 0 )	{
        g_pData = _MALLOC(char,g_DataLen + 1);//���"\0"
        FILE *fp = fopen(g_FileName, "rb");
        fread(g_pData, g_DataLen, 1, fp);		//������
        fclose(fp);
		g_FileType= GetFileType();//�ж��ļ�������dos����unix
    } else {
		PutDebugString("a empty file");
        g_DataLen = 1;
        g_pData = _MALLOC(char,g_DataLen);
        memset(g_pData, 0, 1);
		g_FileType = _DEFAULT_FILE_TYPE;
    }

	//��ʼ������
	InitIndex();
	return true;
}

/**
	�����ļ�
	@param filename:�ļ���
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
	��������е�����λ�ò�������m_pIndexList
*/
void InitIndex() {
    g_IndexCount = 0;

    for(int i = 0; i < g_DataLen; i++)	{
        //�ҵ�,Ҫ��[��ͷ������ǰһ���ַ����ǻس�
        if( g_pData[i] == '[' && (i == 0 || IsAfterNewLine(i) ) ) {
            g_IndexCount++;
        }
    }

    //�����ڴ�
    _FREE( g_pIndexList );
    if( g_IndexCount > 0 )
        g_pIndexList = _MALLOC(int,g_IndexCount);

    int n = 0;
    //��ʼ��
    for(int i = 0; i < g_DataLen; i++){
        if( g_pData[i] == '[' && ( i == 0 || IsAfterNewLine(i) ) ) {
            g_pIndexList[n] = i + 1;		//���浽�������λ���� [ ����һ���ַ���λ��
            n++;
        }
    }
}


/**
	����ָ������λ��
	@param string ֵ
	@return �������ڵ�λ��
*/
int FindIndex(const char *string) {
    for(int i = 0; i < g_IndexCount; i++) {
        char *str = ReadKey( g_pIndexList[i] );		//���������������Ե�����
        if( strcmp(string, str) == 0 )	{		//�Ƚϣ������ͬ�ͷ��ر����λ��
            _FREE( str );
            return g_pIndexList[i];				//�������ڵ�λ�ã��� [ ���ַ�����һ��λ��
        }
        _FREE( str );
    }
    return -1;
}

/**
�������������������Ǳ���(����)����ι��ж����У�һ��������ѭ��������
@param index ������
*/
int GetCountByIndex(const char *index) {
	int num = 0;
	int n = FindIndex(index);				//�ҵ�����λ��
	n = GotoNextLine(n);					//��������(����)
	while(1) {
		//Ҫ��һ�еĿ�ͷ����������ַ��Ļ����ͽ���
		if( g_pData[n] == '\r' || g_pData[n] == -3 || g_pData[n] == EOF
			|| g_pData[n] == ' ' || g_pData[n] == '/' || g_pData[n] == '\t' || g_pData[n] == '\n' ) {
				return num;
		} else {
			num++;
			n = GotoNextLine(n);			//����һ�еĿ�ͷ
			if( n >= g_DataLen )
				return num;			//�ļ�����
		}
	}
}

/**
	����ָ�����ݵ����ݵ�λ��
	@param index ������
	@param string ���ݼ�
	@return ����λ��
*/
int FindKey(int index,const char *string) {
    int p = index;	//ָ��

    while(true) {
        //��������(����)���ڵ���
        p = GotoNextLine(p);				//��һ�е�λ��
        char *name = ReadKey(p);		//��һ�����֣�ע��p�ǰ���ַ�ķ�ʽ���ε�
        if( strcmp(string, name) == 0 ) {	//��ͬ�ͷ���λ�ã��������ݿ�ʼ��λ��
            _FREE( name );
            return p;
        }

        _FREE( name );
        if( p >= g_DataLen ) return -1;		//û���ҵ�
    }
    return -1;
}

/**
	��ָ��λ�ö�һ��������
	@param p λ�����ã������ߵ�valueλ��
	@return ָ��λ�����ݼ���
*/
char* ReadKey(OUT int &p) {		//���ò�������ͨ��p���أ�����ֵ��
    char* Ret = _MALLOC( char ,MAX_PATH );
    memset(Ret, 0, MAX_PATH);
    //���ݵ����ݵ�λ��
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


        //����������������
        if( chr == '=' || chr == ';' || chr == '\n' || chr == '\r' ) {
            p = i + 1;						//ָ�����ݵ����ݣ������˷ָ���
            break;
        }

        Ret[m] = chr;//���ֽڶ���
        m++;
    }
    return Ret;
}

/**
��ָ�����ж�һ�������ƣ�Ŀ�Ļ���Ϊ��������ȡ��
@param index ������
@param lines �к�
*/
char* ReadKeyByLine(const char *index, int lines) {
	int n = FindIndex(index);
	if( n == -1 ) {
		ShowMessage("Can't find [%s] in file <%s>", index, g_FileName);
		return NULL;
	}

	//����ָ������
	n = GotoNextLine(n);
	for(int i = 0; i < lines; i++) {
		if( n < g_DataLen )
			n = GotoNextLine(n);
	}

	return ReadKey(n);			//�������ַ���
}

/**
	��ָ��λ�ö�һ���ַ���
	@param p  ��������
*/
char* ReadLine(int p) {
    char chr;
    char *Ret;
    int n = p, m = 0;

    int EndLine = GotoNextLine(p);		//��һ�е�λ�ã�ֻ��һ�е�����
    Ret = (char *)malloc(EndLine - p + 1);	//�����ڴ�
    memset(Ret, 0, EndLine - p + 1);		//��0

    for(int i = 0; i < g_DataLen - p; i++) {
        chr = g_pData[n];

        //����			�÷ֺţ��س���TAB������]����
        if( chr == ';' || chr == '\n' ||chr == '\r' || chr == '\t' || chr == ']' ) {
            return Ret;
        }

        Ret[m] = chr;		//���뵽������
        m++;
        n++;
    }
    return Ret;
}

/**
	����ͨ��ʽ��һ�ַ�������
	@param ������
	@param ����
	@return Ŀ���ַ���
*/
char* ReadValue(const char *index,const char *name) {
    int n = FindIndex(index);				//ͨ�����������ҵ�λ��
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file '%s'!", index, g_FileName);
        return "";
    }

    int m = FindKey(n, name);			//ͨ��������λ�úͼ������ҵ��������ݵ�λ��
    if( m == -1 ) {
        ShowMessage("Can't find [%s]-'%s' in file '%s'!", index, name, g_FileName);
        return "";
    }

    return ReadKey(m);					//����������
}

/**
	��ָ�����ж�һ�ַ������������������ð�����ڶ���һ�����ݣ�
	���ҿ��Բ����ľ�������֣�������ѭ���ķ�ʽ��
	@param index ������
	@param lines �кţ������������п�ʼ����
*/
char* ReadValueByLine(const char *index, int lines) {
    int n = FindIndex(index);				//ͨ�����������ҵ�λ��
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file '%s'!", index, g_FileName);
        return "";
    }

    //����ָ������
    n = GotoNextLine(n);
    for(int i = 0; i < lines; i++) {
        if( n < g_DataLen )
            n = GotoNextLine(n);
    }

    //������
    while( n <= g_DataLen ) {
        if( g_pData[n] == '=' ) {	//�ҵ��ָ��� =
            n++;					//�ƶ���=����һ���ַ�
            return ReadKey(n);		//��������
        }
        if( g_pData[n] == '\r' || g_pData[n] == '\n') {
            return "";				//û���ҵ��ָ����Ϳ������з���
        }
        n++;
    }

    return "";
}

/**
	����ͨ��ʽ��һint����
	@param index ������
	@param name ���ݼ�
*/
int ReadValueInt(const char *index,const char *name) {
    int n = FindIndex(index);
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file <%s>", index, g_FileName);
        return ERROR_DATA;
    }

    int m = FindKey(n, name);		//���������ע��
    if( m == -1 ) {
        ShowMessage("Can't find [%s] '%s' in file <%s>", index, name, g_FileName);
        return ERROR_DATA;
    }

    char *str = ReadKey(m);
    int ret = atoi(str);				//ת��������
    _FREE(str);
    return ret;
}

/**
	��ָ�����ж�һ������ͬ����Ϊ�˰���ų����Ķ�ȡ�����ҿ��Բ����ľ��������
	@param index:������
	@param lines:�к�
*/
int ReadValueIntByLine(const char *index, int lines) {
    int n = FindIndex(index);			//ͨ�����������ҵ�λ��
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file <%s>", index, g_FileName);
        return ERROR_DATA;
    }

    //����ָ������
    n = GotoNextLine(n);
    for(int i = 0; i < lines; i++) {
        if( n < g_DataLen )
            n = GotoNextLine(n);
    }

    //������
    while( n < g_DataLen ) {
        if( g_pData[n] == '=' ) {
            n++;
            char *str = ReadKey(n);
            int ret = atoi(str);
            _FREE(str);
            return ret;
        }
        if( g_pData[n] == '\r' && g_pData[n] == '\n') {//û���ҵ��ָ����Ϳ������з���
            return ERROR_DATA;
        }
        n++;
    }

    return ERROR_DATA;
}

/**
�ߵ�ָ��λ�õ���һ��,ʵ�����ҵ���һ��"\n"
@param p λ��
@return ��һ������
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
����ļ�������
@param cur �ø�����һ���������ֵ
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
����ȫ���ļ�����,���ã�
*/
char* GetData() {
	return g_pData;
}

/**
�ڵ�ǰλ���޸�һ�����ݵ�ֵ
@param p λ��
@param name ���ݼ�
@param string ����ֵ
*/
bool ModityData(int p,const char *name,const char *string) {
	int n = FindKey(p, name);					//�ҵ����ݡ�ע�⣺���ص�n�����ݵ�λ��

	char *t = ReadKey(n);						//�������ݵ�����
	p = n + strlen(t);								//�ƶ������ݵ�ĩβ����һ��λ��
	_FREE(t);

	int newlen = strlen(string);					//�µ����ݳ���
	int oldlen = p - n;								//t�ĳ��ȣ�Ҳ����ԭ�������ݳ���

	g_pData = (char *)realloc(g_pData, g_DataLen + newlen - oldlen);	//���·����ڴ�

	char *temp = _MALLOC(char,g_DataLen - p);						//��ͬ�ķ���
	memcpy(temp, &g_pData[p], g_DataLen - p);					//ȡ�����
	memcpy(&g_pData[n + newlen], temp, g_DataLen - p);			//�Ѻ���İᵽĩβ
	memcpy(&g_pData[n], string, newlen);						//��д�¶�
	g_DataLen += newlen - oldlen;								//�������ݳ���

	_FREE( temp );
	return true;
}

/**
����ͨ��ʽдһ�ַ������ݡ����������֣�����
@param index ������
@param name ���ݼ�
@param string ����ֵ
*/
bool WriteValue(const char *index, const char *name,const char *string) {
	int n = FindIndex(index);
	if( n == -1 ) {	//�½�����
		AddIndex(index);			//����һ��һ������
		n = FindIndex(index);			//�ҵ���λ��
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	int m = FindKey(n, name);
	if( m == -1 ) {	//�½�����
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	ModityData(n, name, string);	//�޸�һ������

	return true;
}

/**
����ͨ��ʽдһ����
@param index ������
@param name ���ݼ�
@param string ����ֵ��int��
*/
bool WriteValueInt(const char *index,const char *name, int num) {
	char string[32];
	sprintf(string, "%d", num);

	int n = FindIndex(index);
	if( n == -1 ) {	//�½�����
		AddIndex(index);			//�������ע��
		n = FindIndex(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	int m = FindKey(n, name);
	if( m == -1 ) {	//�½�����
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	ModityData(n, name, string);	//�޸�һ������

	return true;
}

/**
���һ������
*/
bool AddIndex(const char *index) {
	char str[MAX_PATH];
	memset(str, 0, MAX_PATH);
	int n = FindIndex(index);

	if( n == -1 ) {	//�½�����
		_ASSERT(g_FileType < eOHTER);
		sprintf(str, "%s[%s]%s", index , g_NewLine[g_FileType]);		//ע���ʽ������Ҫ�ֿ�
		g_pData = (char *)realloc(g_pData, g_DataLen + strlen(str));	//���·����ڴ�
		sprintf(&g_pData[g_DataLen], "%s", str);		//���ں���
		g_DataLen += strlen(str);					//���³���

		InitIndex();							//���½���������
		return true;
	}

	return false;	//�Ѿ�����
}

/**
�ڵ�ǰλ�ü���һ������
@param p λ��
@param name ���ݼ�
@param string ����ֵ
*/
bool AddData(int p,const char *key, const char *value) {
	int len=strlen(value);
	char str[MAX_PATH];						//�����ռ�
	memset(str, 0, MAX_PATH);
	_ASSERT(g_FileType < eOHTER);
	sprintf(str,"%s=%s%s",key,value,g_NewLine[g_FileType]);		//����tiamo=1�ĸ�ʽ
	len=strlen(str);							//�����㳤��

	p=GotoNextLine(p);	//����
	g_pData=(char *)realloc(g_pData, g_DataLen+len);	//���·����ڴ�

	char* temp = _MALLOC(char,g_DataLen-p);				//��������ݷŵ�temp����
	memcpy(temp, &g_pData[p], g_DataLen-p);
	memcpy(&g_pData[p+len], temp, g_DataLen-p);		//�Ѻ���İᵽĩβ
	memcpy(&g_pData[p], str, len);					//�����¼ӵ�����
	g_DataLen+=len;								//�������ݳ���

	_FREE(temp);

	return true;
}

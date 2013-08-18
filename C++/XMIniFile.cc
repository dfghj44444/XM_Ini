
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
@File Name:Wxm_IniFile.cc
@Describe:
@Create Date:2010-12-7 14:01:29
@Version:1.0
@Author��Double One
@Blog�� http://onedouble.cnblogs.com
@Modifier: Double One (dfghj77777@gmail.com)
@Modify Date��2012-12-27 14:01:30
@Modify Reason��
***********************************************************************/


#include "XM_IniFile.h"
#include "Common.h"


//�޲ι���
XM_IniFile::XM_IniFile() {
    m_DataLen = 0;
    m_pData = NULL;
    m_IndexNum = 0;
    m_pIndexList = NULL;
}

/**
	�вι���
	@param filename �ļ���
*/
XM_IniFile::XM_IniFile(char *filename) {
    m_DataLen = 0;
    m_pData = NULL;
    m_IndexNum = 0;
    m_pIndexList = NULL;
    Open(filename);
}

//����
XM_IniFile::~XM_IniFile() {
    if( m_DataLen != 0 && m_pData != NULL )	{
        _DELETE( m_pData );
    }

    if( m_IndexNum != 0 && m_pIndexList != NULL )	{
        _DELETE( m_pIndexList );
    }
}

/**
	�����ļ�
	@param filename ��������ļ���
*/
bool XM_IniFile::Open(char *filename) {

    int nSrcLen = strlen(filename);
    if ( MAX_PATH <= nSrcLen)
        strcpy_s(m_FileName, nSrcLen, filename);
    else
        return false;//�������

    _DELETE( m_pData );

    m_DataLen = GetFileLength(m_FileName);
    //�ļ�����
    if( m_DataLen > 0 )	{
        m_pData = new char[m_DataLen];

        FILE *fp;
        fp = fopen(m_FileName, "rb");
        fread(m_pData, m_DataLen, 1, fp);		//������
        fclose(fp);

        //��ʼ������
        InitIndex();
    } else {
        m_DataLen = 1;
        m_pData = new char[m_DataLen];
        memset(m_pData, 0, 1);
        InitIndex();
    }

    return false;
}

/**
	�����ļ�
	@param filename:�ļ���
*/
bool XM_IniFile::Save(char *filename) {
    if( filename == NULL )
        filename = m_FileName;

    FILE *fp;
    fp = fopen(filename, "wb");
    if( fp == NULL )	{
        ShowMessage("Can't save %s ", filename);
        return false;
    }

    fwrite(m_pData, m_DataLen, 1, fp);
    fclose(fp);

    return true;
}

/**
	��������е�����λ�ò�������m_pIndexList
*/
void XM_IniFile::InitIndex() {
    m_IndexNum = 0;

    for(int i = 0; i < m_DataLen; i++)	{
        //�ҵ�,Ҫ��[��ͷ�����Ҷ����֮��Ҫ�ûس��ֿ�
        if( m_pData[i] == '[' && (i == 0 || (i > 0 && m_pData[i - 1] == '\n')  ) ) {
            m_IndexNum++;
        }
    }

    //�����ڴ�
    _DELETE( m_pIndexList );
    if( m_IndexNum > 0 )
        m_pIndexList = new int[m_IndexNum];

    int n = 0;
    //��ʼ��
    for(int i = 0; i < m_DataLen; i++)	{
        if( m_pData[i] == '[' && ((i > 0 && m_pData[i - 1] == '\n' ) || i == 0) )		{
            m_pIndexList[n] = i + 1;		//���浽�������λ���� [ ����һ���ַ���λ��
            n++;
        }
    }
}

/**
	����ָ������λ��
	@param string ֵ
	@return �������ڵ�λ��
*/
int XM_IniFile::FindIndex(char *string) {
    for(int i = 0; i < m_IndexNum; i++) {
        char *str = ReadData( m_pIndexList[i] );		//���������������Ե�����
        if( strcmp(string, str) == 0 )	{		//�Ƚϣ������ͬ�ͷ��ر����λ��
            _FREE( str );
            return m_pIndexList[i];				//�������ڵ�λ�ã��� [ ���ַ�����һ��λ��
        }
        _FREE( str );
    }
    return -1;
}

/**
	����ָ�����ݵ����ݵ�λ��
	@param index ������
	@param string ���ݼ�
	@return ����λ��
*/
int XM_IniFile::FindKey(int index, char *string) {
    int p = index;	//ָ��

    while(1) {
        //��������(����)���ڵ���
        p = GotoNextLine(p);				//��һ�е�λ��
        char *name = ReadKey(p);		//��һ�����֣�ע��p�ǰ���ַ�ķ�ʽ���ε�
        if( strcmp(string, name) == 0 ) {	//��ͬ�ͷ���λ�ã��������ݿ�ʼ��λ��
            _FREE( name );
            return p;
        }

        _FREE( name );
        if( p >= m_DataLen ) return -1;		//û���ҵ�
    }
    return -1;
}

/**
	�ߵ���һ��
	@param p λ��
	@return ��һ������
*/
int XM_IniFile::GotoNextLine(int p) {
    int i;
    for(i = p; i < m_DataLen; i++) {
        if( m_pData[i] == '\n' )				//�س�����
            return i + 1;

    }
    return i;
}

/**
	��ָ��λ�ö�һ��������
	@param p λ�����ã������ߵ�valueλ��
	@return ָ��λ�����ݼ�
*/
char *XM_IniFile::ReadKey(int &p) {		//���ò�������ͨ��p���أ�����ֵ��
    //���ݵ����ݵ�λ��
    char chr;
    char *Ret;
    int m = 0;

    Ret = (char *)malloc(256);
    memset(Ret, 0, 256);

    for(int i = p; i < m_DataLen; i++) {
        chr = m_pData[i];

        //����
        if( chr == '\r' ) {
            p = i + 1;
            return Ret;
        }

        //����
        if( chr == '=' || chr == ';' ) {
            p = i + 1;						//ָ�����ݵ����ݣ������˷ָ���
            return Ret;
        }

        Ret[m] = chr;
        m++;
    }
    return Ret;
}

/**
	��ָ��λ�ö�һ���ַ���
	@param p  ��������
*/
char *XM_IniFile::ReadData(int p) {
    char chr;
    char *Ret;
    int n = p, m = 0;

    int EndLine = GotoNextLine(p);		//��һ�е�λ�ã�ֻ��һ�е�����
    Ret = (char *)malloc(EndLine - p + 1);	//�����ڴ�
    memset(Ret, 0, EndLine - p + 1);		//��0

    for(int i = 0; i < m_DataLen - p; i++) {
        chr = m_pData[n];

        //����			�÷ֺţ��س���TAB������]����
        if( chr == ';' || chr == '\r' || chr == '\t' || chr == ']' ) {
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
	@param ������
	@return Ŀ���ַ���
*/
char *XM_IniFile::ReadValue(char *index, char *name) {
    int n = FindIndex(index);				//ͨ�����������ҵ�λ��
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file '%s'!", index, m_FileName);
        return "";
    }

    int m = FindKey(n, name);			//ͨ��������λ�ú����������ҵ��������ݵ�λ��
    if( m == -1 ) {
        ShowMessage("Can't find [%s]-'%s' in file '%s'!", index, name, m_FileName);
        return "";
    }

    return ReadData(m);					//����������
}

/**
	��ָ�����ж�һ�ַ��������������������ڶ���һ�����ݣ�
	���ҿ��Բ����ľ�������֣�������ѭ���ķ�ʽ��
	@param ������
	@param �к�
*/
char *XM_IniFile::ReadValue(char *index, int lines) {
    int n = FindIndex(index);				//ͨ�����������ҵ�λ��
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file '%s'!", index, m_FileName);
        return "";
    }

    //����ָ������
    n = GotoNextLine(n);
    for(int i = 0; i < lines; i++) {
        if( n < m_DataLen )
            n = GotoNextLine(n);
    }

    //������
    while( n <= m_DataLen ) {
        if( m_pData[n] == '=' ) {	//�ҵ��ָ��� =
            n++;					//�ƶ���=����һ���ַ�
            return ReadData(n);		//��������
        }
        if( m_pData[n] == '\r' ) {
            return "";				//û���ҵ�
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
int XM_IniFile::ReadValueInt(char *index, char *name) {
    int n = FindIndex(index);
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file <%s>", index, m_FileName);
        return ERROR_DATA;
    }

    int m = FindKey(n, name);		//���������ע��
    if( m == -1 ) {
        ShowMessage("Can't find [%s] '%s' in file <%s>", index, name, m_FileName);
        return ERROR_DATA;
    }

    char *str = ReadData(m);
    int ret = atoi(str);				//ת��������
    _FREE(str);
    return ret;
}

/**
	��ָ�����ж�һ������ͬ����Ϊ�˳����Ķ�ȡ�����ҿ��Բ����ľ��������
	@param index:������
	@param lines:�к�
*/
int XM_IniFile::ReadValueInt(char *index, int lines) {
    int n = FindIndex(index);			//ͨ�����������ҵ�λ��
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file <%s>", index, m_FileName);
        return ERROR_DATA;
    }

    //����ָ������
    n = GotoNextLine(n);
    for(int i = 0; i < lines; i++) {
        if( n < m_DataLen )
            n = GotoNextLine(n);
    }

    //������
    while( n < m_DataLen ) {
        if( m_pData[n] == '=' ) {
            n++;
            char *str = ReadData(n);
            int ret = atoi(str);
            _FREE(str);
            return ret;
        }
        if( m_pData[n] == '\r' ) {
            return ERROR_DATA;
        }
        n++;
    }

    return ERROR_DATA;
}

/**
	��ָ�����ж�һ�������ƣ�Ŀ�Ļ���Ϊ��������ȡ��
	@param index ������
	@param lines �к�
*/
char *XM_IniFile::ReadKey(char *index, int lines) {
    int n = FindIndex(index);
    if( n == -1 ) {
        ShowMessage("Can't find [%s] in file <%s>", index, m_FileName);
        return NULL;
    }

    //����ָ������
    n = GotoNextLine(n);
    for(int i = 0; i < lines; i++) {
        if( n < m_DataLen )
            n = GotoNextLine(n);
    }

    return ReadKey(n);			//�������ַ���
}

/**
	����ͨ��ʽдһ�ַ������ݡ����������֣�����
	@param index ������
	@param name ���ݼ�
	@param string ����ֵ
*/
bool XM_IniFile::WriteValue(char *index, char *name, char *string) {
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
bool XM_IniFile::WriteValueInt(char *index, char *name, int num) {
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
bool XM_IniFile::AddIndex(char *index) {
    char str[256];
    memset(str, 0, 256);
    int n = FindIndex(index);

    if( n == -1 ) {	//�½�����
        sprintf(str, "\r\n[%s]\r\n", index);		//ע���ʽ������Ҫ�ֿ�
        m_pData = (char *)realloc(m_pData, m_DataLen + strlen(str));	//���·����ڴ�
        sprintf(&m_pData[m_DataLen], "%s", str);		//���ں���
        m_DataLen += strlen(str);					//���³���

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
bool XM_IniFile::AddData(int p, char *key, char *value) {
	char *str;
	int len=strlen(value);
	str=new char[len+256];						//�����ռ�
	memset(str, 0, len+256);
	sprintf(str,"%s=%s\r\n",key,value);		//����tiamo=1�ĸ�ʽ
	len=strlen(str);							//�����㳤��

	p=GotoNextLine(p);	//����
	m_pData=(char *)realloc(m_pData, m_DataLen+len);	//���·����ڴ�

	char *temp=new char[m_DataLen-p];				//��������ݷŵ�temp����
	memcpy(temp, &m_pData[p], m_DataLen-p);
	memcpy(&m_pData[p+len], temp, m_DataLen-p);		//�Ѻ���İᵽĩβ
	memcpy(&m_pData[p], str, len);					//�����¼ӵ�����
	m_DataLen+=len;								//�������ݳ���

	_DELETE( temp );
	_DELETE( str );

    return true;
}

/**
	�ڵ�ǰλ���޸�һ�����ݵ�ֵ
	@param p λ��
	@param name ���ݼ�
	@param string ����ֵ
*/
bool XM_IniFile::ModityData(int p, char *name, char *string) {
    int n = FindKey(p, name);					//�ҵ����ݡ�ע�⣺���ص�n�����ݵ�λ��

    char *t = ReadData(n);						//�������ݵ�����
    p = n + strlen(t);								//�ƶ������ݵ�ĩβ����һ��λ��
    _FREE(t);

    int newlen = strlen(string);					//�µ����ݳ���
    int oldlen = p - n;								//t�ĳ��ȣ�Ҳ����ԭ�������ݳ���

    m_pData = (char *)realloc(m_pData, m_DataLen + newlen - oldlen);	//���·����ڴ�

    char *temp = new char[m_DataLen - p];						//��ͬ�ķ���
    memcpy(temp, &m_pData[p], m_DataLen - p);
    memcpy(&m_pData[n + newlen], temp, m_DataLen - p);			//�Ѻ���İᵽĩβ
    memcpy(&m_pData[n], string, newlen);
    m_DataLen += newlen - oldlen;								//�������ݳ���

    _DELETE( temp );
    return true;
}

/**
	����ȫ���ļ�����
*/
char *XM_IniFile::GetData() {
    return m_pData;
}

/**
	����ļ�������
	@param cur �ø�����һ���������ֵ
*/
int XM_IniFile::GetLines(int cur) {
    int n = 1;
    for(int i = 0; i < cur; i++) {
        if( m_pData[i] == '\n' )					//�ҵ�\n�������µ�һ��
            n++;
    }
    return n;
}

/**
	�������������������Ǳ���(����)����ι��ж����У�һ��������ѭ��������
	@param index ������
*/
int XM_IniFile::GetContinueDataNum(char *index) {
    int num = 0;
    int n = FindIndex(index);				//����������
    n = GotoNextLine(n);					//��������(����)
    while(1) {
        //Ҫ��һ�еĿ�ͷ����������ַ��Ļ����ͽ���
        if( m_pData[n] == '\r' || m_pData[n] == -3 || m_pData[n] == EOF
                || m_pData[n] == ' ' || m_pData[n] == '/' || m_pData[n] == '\t' || m_pData[n] == '\n' ) {
            return num;
        } else {
            num++;						//��һ
            n = GotoNextLine(n);			//����һ�еĿ�ͷ
            if( n >= m_DataLen )
                return num;			//�ļ�����
        }
    }
}

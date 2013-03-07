
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
@File Name:Common.cc
@Describe:
@Create Date:2012-12-8
@Version:1.0
@Author��Double One
@Blog�� http://onedouble.cnblogs.com
@Modifier: Double One (dfghj77777@gmail.com)
@Modify Date��2012-12-27 18:00:39
@Modify Reason��
***********************************************************************/
#include "Common.h"


/**
*	���þ���
*/
RECT GetRect(int x1, int y1, int x2, int y2) {
    RECT TempRect = {x1, y1, x2, y2};
    return TempRect;
}

/**
	ȡ�������
	@param nMax:��Χ���ֵ
	@return һ�������ֵ
*/
int random(int nMax) {
    return rand() * nMax / RAND_MAX;
}

/**
	���¼����ɿ�
	@param Key:��ֵ
	@param x:
*/
void PressKey(DWORD Key, int x) {
    if( x == 0 )
        while( !GetAsyncKeyState(Key) )
        {}
    while( GetAsyncKeyState(Key) )
    {}
}

/**
	�ȴ�����
*/
void WaitKey(DWORD Key) {
    while( GetAsyncKeyState(Key) )	{}
    while( !GetAsyncKeyState(Key) )	{}
}

///**
//	��ʱ����
//	@param time:�ӳ�ʱ��
//*/
//void Delay(long time)
//{
//	static long old_clock, new_clock; //��ʱ����
//	new_clock=old_clock=timeGetTime();
//
//	MSG msg;
//	while( new_clock < old_clock + time )
//	{
//		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
//		{
//            if (!GetMessage(&msg, NULL, 0, 0)) return;
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//		}
//		else if(bActive)
//		{
//			new_clock=timeGetTime();
//			if( GetAsyncKeyState(VK_F4) )		//F4����
//			{
//				return;
//			}
//		}
//		else WaitMessage();	//�ȴ���Ϣ
//	}
//}
//��Ҫ�õ�������

/**
	�ٶȲ���
	@param nFrames:֡��
	@return ֡��
*/
int CountSpeed(long &nFrames) {
    static long old_clock, new_clock; //��ʱ����
    static long ret;

    new_clock = timeGetTime();
    if( new_clock > old_clock + 1000 ) {
        old_clock = new_clock;
        ret = nFrames;
        nFrames = 0;
    }
    return ret;
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

    MessageBox(NULL, str, "Message", MB_OK);
}

/**
	���������Ϣ
*/
void PutDebugString(char *msg) {
    static int num = 0;
    char t[16];
    \

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
	���ļ�����ȡ�ļ�·��
	@param filename:�ļ���
	@return �ļ�·��
*/
char *GetFilePath(char *filename) {
    char *Ret;
    int Len = strlen(filename);
    Ret = (char *)malloc(Len + 1);
    strcpy(Ret, filename);

    for(int i = Len - 1; i >= 0; i--) {
        if( Ret[i] == '\\' )			//�Ӻ���ǰ��'\'
            break;
        Ret[i] = 0;						//���óɽ�����
    }

    return Ret;
}
/**
	���ļ�����·��������ļ�������·��
	@param filename:��·�����ļ���
	@return ����·�����ļ���
*/
char *GetFileName(char *filename) {
    char *szTemp = strrchr(filename, '\\');		//�����һ��'\'
    szTemp++;									//����'\'
    char *Ret = (char *)malloc(strlen(szTemp) + 1);
    strcpy(Ret, szTemp);
    return Ret;
}
/**
	char to wchar
*/
WCHAR *WChar(char *string) {
    WCHAR *wchr;
    //���������������ó�NULL��0
    //�õ���Ҫ�Ļ���������
    int wsize = MultiByteToWideChar(CP_ACP, NULL, string, strlen(string), NULL, 0);

    wchr = new WCHAR[wsize + 1];		//���һ����0
    MultiByteToWideChar(CP_ACP, NULL, string, strlen(string), wchr, wsize);
    wchr[wsize] = 0;
    return wchr;
    //int   nLen   =   strlen(string)   +   1;
    //int   nwLen   =   MultiByteToWideChar(CP_ACP,   0,   string,   nLen,   NULL,   0);
    //WCHAR   lpszFile[nwLen+1];
    //MultiByteToWideChar(CP_ACP,   0,   string,   nLen,   lpszFile,   nwLen);
    //return lpszFile;

}

/**
	wchar to char
*/
char *Char(WCHAR *string) {					//����intelCPU�ķ��÷���
    //����һ��L"tiamo"
    char *chr;									//��ôӦ���������ķ�ʽ��ÿ���ַ�ռ16λ
    chr = new char[wcslen(string) + 1];				//����Ϊ��
    memset(chr, 0, wcslen(string) + 1);			//0x0074,0x0069,0x0061,0x006d,0x006f,0x0000
    for(unsigned int i = 0; i < wcslen(string); i++) { //��intel��CPU�������ŵ�
        //7400|6900|6100|6d00|6f00|0000
        chr[i] = (unsigned char)string[i];		//���ˣ���������ǿ������ת������
    }
    return chr;
}

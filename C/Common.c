
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
@File Name:Common.cc
@Describe:
@Create Date:2012-12-8
@Version:1.0
@Author��Double One
@Blog�� http://onedouble.cnblogs.com
@Modifier: Double One (dfghj77777@gmail.com)
@Modify Date��2013-8-1 15:44:00
@Modify Reason��
***********************************************************************/
#include "Common.h"

/**
	ȡ�������
	@param nMax:��Χ���ֵ
	@return һ�������ֵ
*/
int random(int nMax) {
    return rand() * nMax / RAND_MAX;
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

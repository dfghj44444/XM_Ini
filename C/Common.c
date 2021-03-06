
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
@Author：Double One
@Blog： http://onedouble.cnblogs.com
@Modifier: Double One (dfghj77777@gmail.com)
@Modify Date：2013-8-1 15:44:00
@Modify Reason：
***********************************************************************/
#include "Common.h"

/**
	取得随机数
	@param nMax:范围最大值
	@return 一个随机数值
*/
int random(int nMax) {
    return rand() * nMax / RAND_MAX;
}




/**
	由文件名获取文件路径
	@param filename:文件名
	@return 文件路径
*/
char *GetFilePath(char *filename) {
    char *Ret;
    int Len = strlen(filename);
    Ret = (char *)malloc(Len + 1);
    strcpy(Ret, filename);

    for(int i = Len - 1; i >= 0; i--) {
        if( Ret[i] == '\\' )			//从后向前找'\'
            break;
        Ret[i] = 0;						//设置成结束点
    }

    return Ret;
}
/**
	从文件名含路径分离出文件名不含路径
	@param filename:含路径的文件名
	@return 不含路径的文件名
*/
char *GetFileName(char *filename) {
    char *szTemp = strrchr(filename, '\\');		//找最后一个'\'
    szTemp++;									//跳过'\'
    char *Ret = (char *)malloc(strlen(szTemp) + 1);
    strcpy(Ret, szTemp);
    return Ret;
}
/**
	char to wchar
*/
WCHAR *WChar(char *string) {
    WCHAR *wchr;
    //将后两个参数设置成NULL和0
    //得到需要的缓冲区长度
    int wsize = MultiByteToWideChar(CP_ACP, NULL, string, strlen(string), NULL, 0);

    wchr = new WCHAR[wsize + 1];		//多出一个放0
    MultiByteToWideChar(CP_ACP, NULL, string, strlen(string), wchr, wsize);
    wchr[wsize] = 0;
    return wchr;
}

/**
	wchar to char
*/
char *Char(WCHAR *string) {					//想想intelCPU的放置方法
    //比如一个L"tiamo"
    char *chr;									//那么应该是这样的方式：每个字符占16位
    chr = new char[wcslen(string) + 1];				//依次为：
    memset(chr, 0, wcslen(string) + 1);			//0x0074,0x0069,0x0061,0x006d,0x006f,0x0000
    for(unsigned int i = 0; i < wcslen(string); i++) { //而intel的CPU是这样放的
        //7400|6900|6100|6d00|6f00|0000
        chr[i] = (unsigned char)string[i];		//好了，这下来个强制类型转换……
    }
    return chr;
}

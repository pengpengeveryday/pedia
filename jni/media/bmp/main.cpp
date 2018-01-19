#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma pack(1)
enum
{
  HV41 = 0, //default value
  HV22,
  HV11,
  HV21,
  HV12,
  NV12
};

typedef struct tagCMDParm
{
    char filename[256]; //source filename
    int help;
    int w;
    int h;
    int fsz;
    int type;
}CMDParam;
CMDParam Param; //global configuration

void InitParam(CMDParam* param)
{
    memset(param , 0x00 , sizeof(CMDParam));
}
void ParseInputCommand(char* InputCommand, CMDParam* param)
{
    char   *InputCommandTmp[256];
    int i;


    InputCommandTmp[0] = strtok(InputCommand, " \n");

    for (i=1; InputCommandTmp[i-1] != NULL; i++)
    {
        InputCommandTmp[i] = strtok(NULL, " \n");
    }


    for (i=0; InputCommandTmp[i] != NULL; i++)
    {
        if (!strncmp(InputCommandTmp[i], "-help", 5))
        {
            param->help = 1;
            continue;
        }
        if (i == 0)//get file name
        {
            strcpy(&param->filename[0], InputCommandTmp[i]);
            continue;
        }

        if (!strncmp(InputCommandTmp[i], "-w=", 3))
        {
            param->w = atoi(&InputCommandTmp[i][3]);
            continue;
        }
        
        if (!strncmp(InputCommandTmp[i], "-h=", 3))
        {
            param->h = atoi(&InputCommandTmp[i][3]);
            continue;
        }
        
        if (!strncmp(InputCommandTmp[i], "-HV41", 5))
        {
            param->type = HV41;
            continue;
        }
        if (!strncmp(InputCommandTmp[i], "-HV22", 5))
        {
            param->type = HV22;
            continue;
        }
         if (!strncmp(InputCommandTmp[i], "-HV11", 5))
        {
            param->type = HV11;
            continue;
        }
         if (!strncmp(InputCommandTmp[i], "-HV12", 5))
        {
            param->type = HV12;
            continue;
        }
         if (!strncmp(InputCommandTmp[i], "-HV21", 5))
        {
            param->type = HV21;
            continue;
        }
         if (!strncmp(InputCommandTmp[i], "-NV12", 5))
        {
            param->type = NV12;
            continue;
        }
    }
}

void HelpCMD()
{
    printf("--------------------help-----------------------\n");
    printf("              filename -w=xx -h=xx colorformat\n");
    printf("colorformat can be '-HV11/-HV41/-HV22/-HV12/-HV21/-NV12'\n");
    printf("-----------------------------------------------\n");
}

typedef unsigned char   BYTE;  /*一个字节的整型*/
typedef unsigned short int  WORD;  /*两个字节的整型*/
typedef unsigned long   DWORD;  /*四个字节的整型*/


/*文件头结构*/
typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;            /*文件类型，BM为BMP*/
    DWORD bfSize;           /*文件的大小(字节)*/
    WORD bfReserved1;       /*保留*/
    WORD bfReserved2;       /*保留*/
    DWORD bfOffBits;        /*本结构开始处至图形阵列的偏移字节数*/
}BITMAPFILEHEADER;

/*文件信息头结构*/
typedef struct tagBITMAPINFOHEADER
{
  DWORD biSize;           /*本结构的字节数*/
  DWORD biWidth;          /*以像素数表示图形的宽度*/
  DWORD biHeight;         /*以像素数表示图形的高度*/
  WORD  biPlanes;         /*色彩平面数，必须为1*/
  WORD  biBitCount;       /*一个像素所占的位数，可以是1、4、8、24*/
  DWORD biCompression;    /*图形压缩形式，0为不压缩*/
  DWORD biSizeImage;      /*图形的字节数*/
  DWORD biXPelsperMeter;  /*1米内的水平像素数*/
  DWORD biYPelsPerMeter;  /*1米内的垂直像素数*/
  DWORD biClrUsed;        /*图形中实际用到的像素数*/
  DWORD biClrlmportant;   /*图形中的重要的颜色数*/
}BITMAPINFOHEADER;

/*颜色*/
typedef struct tagrgb
{
  BYTE B;
  BYTE G;
  BYTE R;
}rgbDATA;

void localgets(char* buf, int len)
{
  char ch;
  for(int i = 0; i < len && (ch = getchar()) != '\n'; i++)
  {
    buf[i]   =   ch;
  }
}

void YUV2RGB(BYTE* pYUV, BYTE* pRGB)
{
  double Y, U, V;
  double R, G, B;
  
  Y = pYUV[0];
  U = pYUV[1];
  V = pYUV[2];
  
  R = Y + 1.402*(V-128);
  G = Y - 0.34414*(U-128) - 0.71414*(V-128);
  B = Y + 1.772*(U-128);

  /*R = Y + 1.402*V;
  G = Y - 0.34414*U - 0.71414*V;
  B = Y + 1.772*U;*/
 
  if (R > 255)R = 255;
  if (G > 255)G = 255;
  if (B > 255)B = 255;
  if (R < 0)R = 0;
  if (G < 0)G = 0;
  if (B < 0)B = 0; 
  pRGB[0] = (int)R;
  pRGB[1] = (int)G;
  pRGB[2] = (int)B;
}


bool RestoreDataFromYUV(BYTE* pData, int len, const char* chFileName, int pos)
{
  FILE* fp;
  int readLen;
  int offset = pos;
  
  fp = fopen(chFileName, "rb");
  if (fp == NULL)
  {
    printf("%s open failed\n", chFileName);
    return false;
  }
  fseek(fp, offset, SEEK_SET);
  readLen = fread(pData, 1, len, fp);
  if (readLen != len)
  {
    printf("RestoreDataFromYUV() read size from %d not pair, %d | %d\n", offset, readLen, len);
    return false;
  }
  fclose(fp);
  return true;
}

int GetYUVFrameLen(int format, int w, int h)
{
  int len = 0;
  switch (format)
  {
    case HV11:
      len = w*h*3;
      break;
    case HV41:
      len = w*h*3/2;
      break;
    case HV22:
      len = w*h*3/2;
      break;
    case HV12:
    case HV21:
      len = w*h*2;
      break;
    case NV12:
      len = w*h*3/2;
      break;
  }
  return len;
}

//YYYYYYYYY
//UUUUUUUUU
//VVVVVVVVV
void HV11ToRGB888(BYTE* pYUV, int w, int h, BYTE* pRGB)
{
  BYTE *pY, *pU, *pV;
  int YLEN, ULEN, VLEN;
  int i = 0;
  BYTE YUV[3] = {0};
  
  YLEN = ULEN = VLEN = w*h;
  pY = pYUV;
  pU = pY + YLEN;
  pV = pU + ULEN;
  
  while (i < w*h*3)
  {
    YUV[0] = *pY;
    YUV[1] = *pU;
    YUV[2] = *pV;
    YUV2RGB(&YUV[0], &pRGB[i]);
    pY++;
    pU++;
    pV++;
    i += 3;
  }
}

//YUV420 Planar
void HV11ToHV22(BYTE* pSrcHV11, BYTE* pDstHV22, int w, int h)
{
  int SRC_FRAME_LEN = w*h;
  int DST_Y_LEN = w*h;
  int DST_U_LEN = DST_Y_LEN/4;
  int DST_V_LEN = DST_U_LEN;
  BYTE* pSY = pSrcHV11;
  BYTE* pSU = pSY + SRC_FRAME_LEN;
  BYTE* pSV = pSU + SRC_FRAME_LEN;
  BYTE* pDY = pDstHV22;
  BYTE* pDU = pDY + DST_Y_LEN;
  BYTE* pDV = pDU + DST_U_LEN;
  int i = 0, j = 0;
  
  for (j = 0; j < h; j+=2)
  {
    for (i = 0; i < w/2; i++)
    {
      pDU[i] = pSU[i*2];
      pDV[i] = pSV[i*2];
      
    }
    pDU += w/2;
    pDV += w/2;
    pSU += w*2;
    pSV += w*2;
  }
  memcpy(pDY, pSY, DST_Y_LEN);
}


void HV41ToHV11(BYTE* pSrcHV22, BYTE* pDstHV11, int w, int h)
{
    int DST_FRAME_LEN = w*h;
	  int SRC_Y_LEN = w*h;
	  int SRC_U_LEN = SRC_Y_LEN/4;
	  int SRC_V_LEN = SRC_U_LEN;
	  BYTE* pDY = pDstHV11;
	  BYTE* pDU = pDY + DST_FRAME_LEN;
	  BYTE* pDV = pDU + DST_FRAME_LEN;
	  BYTE* pSY = pSrcHV22;
	  BYTE* pSU = pSY + SRC_Y_LEN;
	  BYTE* pSV = pSU + SRC_U_LEN;
	  int i = 0, j = 0;

	  for (j = 0; j < h; j++)
	  {
	      for (i = 0; i < w/4; i++)
	      {
	          pDU[i*4] = pSU[i];
	          pDU[i*4+1] = pSU[i];
	          pDU[i*4+2] = pSU[i];
	          pDU[i*4+3] = pSU[i];
	          pDV[i*4] = pSV[i];
	          pDV[i*4+1] = pSV[i];
	          pDV[i*4+2] = pSV[i];
	          pDV[i*4+3] = pSV[i];
	      }
	      pDU += w;
	      pDV += w;
	      pSU += w/4;
	      pSV += w/4;
	  }
	  memcpy(pDY, pSY, SRC_Y_LEN);
}


//YUV420 Planar to YUV444
void HV22ToHV11(BYTE* pSrcHV22, BYTE* pDstHV11, int w, int h)
{
  int DST_FRAME_LEN = w*h;
  int SRC_Y_LEN = w*h;
  int SRC_U_LEN = SRC_Y_LEN/4;
  int SRC_V_LEN = SRC_U_LEN;
  BYTE* pDY = pDstHV11;
  BYTE* pDU = pDY + DST_FRAME_LEN;
  BYTE* pDV = pDU + DST_FRAME_LEN;
  BYTE* pSY = pSrcHV22;
  BYTE* pSU = pSY + SRC_Y_LEN;
  BYTE* pSV = pSU + SRC_U_LEN;
  int i = 0, j = 0;
  
  for (j = 0; j < h; j+=2)
  {
    for (i = 0; i < w/2; i++)
    {
      pDU[i*2] = pDU[i*2+1] = pSU[i];
      pDV[i*2] = pDV[i*2+1] = pSV[i];
      pDU[i*2 + w] = pDU[i*2+1 + w] = pSU[i];
      pDV[i*2 + w] = pDV[i*2+1 + w] = pSV[i];
      
    }
    pSU += w/2;
    pSV += w/2;
    pDU += w*2;
    pDV += w*2;
  }
  memcpy(pDY, pSY, SRC_Y_LEN);
}


//NV12 to YUV444
void NV12ToHV11(BYTE* pSrcHV22, BYTE* pDstHV11, int w, int h)
{
  int DST_FRAME_LEN = w*h;
  int SRC_Y_LEN = w*h;
  int SRC_U_LEN = SRC_Y_LEN/4;
  int SRC_V_LEN = SRC_U_LEN;
  BYTE* pDY = pDstHV11;
  BYTE* pDU = pDY + DST_FRAME_LEN;
  BYTE* pDV = pDU + DST_FRAME_LEN;
  BYTE* pSY = pSrcHV22;
  BYTE* pSU = pSY + SRC_Y_LEN;
  BYTE* pSV = pSU + SRC_U_LEN;
  int i = 0, j = 0;
  
  for (j = 0; j < h; j+=2)
  {
    for (i = 0; i < w/2; i++)
    {
      pDU[i*2] = pDU[i*2+1] = pSU[i*2];
      pDV[i*2] = pDV[i*2+1] = pSU[i*2+1];
      pDU[i*2 + w] = pDU[i*2+1 + w] = pSU[i*2];
      pDV[i*2 + w] = pDV[i*2+1 + w] = pSU[i*2+1];
      
    }
    pSU += w;
 //pSV += w/2;
    pDU += w*2;
    pDV += w*2;
  }
  memcpy(pDY, pSY, SRC_Y_LEN);
}


void MakeRGBData(BYTE* pYUV , int w, int h, BYTE* pRGB, int type)
{
  BYTE* pYUV11 = NULL;
  switch (type)
  {
    case HV11:
      HV11ToRGB888(pYUV, w, h, pRGB);
      break;
    case HV41:
      pYUV11 = new BYTE[w*h*3];
      HV41ToHV11(pYUV, pYUV11, w, h);
      HV11ToRGB888(pYUV11, w, h, pRGB);
      break;
    case HV22:
      pYUV11 = new BYTE[w*h*3];
      HV22ToHV11(pYUV, pYUV11, w, h);
      HV11ToRGB888(pYUV11, w, h, pRGB);
      break;
    case NV12:
      pYUV11 = new BYTE[w*h*3];
      NV12ToHV11(pYUV, pYUV11, w, h);
      HV11ToRGB888(pYUV11, w, h, pRGB);
      break;
  }
  if (pYUV11 != NULL) delete []pYUV11;
}

bool SaveRGBData2File(BYTE* pData, int w, int h, const char* chFileName)
{
  DWORD BytePerLine,FillZeroNum,PixelN;
  BITMAPFILEHEADER fileheader;
  BITMAPINFOHEADER infoheader;
  const char *BMtype="BM"; 
  char Zeros[5]="\0\0\0\0";
  FILE *fpout;
  
  fpout=fopen(chFileName,"wb");
  if (fpout == NULL)
  {
      printf("can't create file:%s\n", chFileName);
      return false;
  }
  
  /****生成BMP文件头*/  
  BytePerLine = (3*w + 3)&0xFFFFFFFC;  /*每行多少个字节,包括补零*/
  FillZeroNum = (3*w)&0x03;    /*补零的个数*/
  
  fileheader.bfType = *((WORD *)(BMtype));
  fileheader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + h*BytePerLine;
  fileheader.bfReserved1 = 0;
  fileheader.bfReserved2 = 0;
  fileheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
  infoheader.biSize = sizeof(BITMAPINFOHEADER);
  infoheader.biWidth=w;
  infoheader.biHeight=h;
  infoheader.biPlanes=1;
  infoheader.biBitCount=24;
  infoheader.biCompression=0;
  infoheader.biSizeImage=h*BytePerLine;
  infoheader.biXPelsperMeter=(WORD)(300*39.37007874);
  infoheader.biYPelsPerMeter=(WORD)(300*39.37007874);
  infoheader.biXPelsperMeter=0;
  infoheader.biYPelsPerMeter=0;
  infoheader.biClrUsed=0;
  infoheader.biClrlmportant=0;
  fwrite(&fileheader, sizeof(fileheader),1,fpout);
  fwrite(&infoheader, sizeof(infoheader),1,fpout);

  fwrite(pData, w*h*3, 1, fpout);
  if (FillZeroNum)
  {
    fwrite(Zeros,sizeof(BYTE),4-FillZeroNum,fpout);
  }
  fclose(fpout);
  return true;
}

int main(void)
{
  InitParam(&Param);
  char command[256] = {0};
  printf("Please input command:\n");
  localgets(&command[0], 255);
  ParseInputCommand(command, &Param);
  if (Param.help == 1)
    HelpCMD();
  
  int w, h;
  int format;
  int frameLen;
  BYTE* pData;
  BYTE* pYUV;
  FILE* fp;
  int readBytes = 0;
  bool canLoop;
  
  w = Param.w;
  h = Param.h;
  format = Param.type;
  frameLen = GetYUVFrameLen(format, w, h);
  pYUV = new BYTE[frameLen];
  pData = new BYTE[w*h*3];
  char newFilename[256] = {0};
  int index = 1;

while (1){
	  canLoop = RestoreDataFromYUV(pYUV, frameLen, Param.filename, readBytes);
	   if (!canLoop)
	   	goto EXIT;
	  readBytes += frameLen;
	  MakeRGBData(pYUV, w, h, pData, format);
	  
	  strcpy(newFilename, Param.filename);
	  sprintf(newFilename, "%s_%d.bmp", Param.filename, index);
	  SaveRGBData2File(pData, w, h, newFilename);
	  index++;
  }
 EXIT:
  delete []pYUV;
  delete []pData;
  
  printf("%s created (w:%d h:%d)\n", newFilename, w, h);
  return 0;
}


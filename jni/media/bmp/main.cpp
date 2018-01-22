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

void RGB2YUV(BYTE* pRGB, BYTE* pYUV)
{
  double Y, U, V;
  BYTE R, G, B;
  
  R = pRGB[1];
  G = pRGB[2];
  B = pRGB[3];
  Y = 0.299*R + 0.587*G + 0.114*B;
  U = -0.1687*R - 0.3313*G + 0.5*B + 128;
  V = 0.5*R - 0.4187*G - 0.0813*B + 128;
  //U = 0.564*(B - Y);
  //V = 0.713*(R - Y);
//Y  =  (0.257 * R) + (0.504 * G) + (0.098 * B) + 16;
//U = -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128;
//V =  (0.439 * R) - (0.368 * G) - (0.071 * B) + 128;
  
  //printf("Y:%d U:%d V:%d  ", (int)Y, (int)U, (int)V);
  pYUV[0] = (int)Y > 235?235:(int)Y;
  pYUV[1] = (int)U > 235?235:(int)U;
  pYUV[2] = (int)V > 240?240:(int)V;
  pYUV[0] = (int)Y<16?16:(int)Y;
  pYUV[1] = (int)U<16?16:(int)U;    
  pYUV[2] = (int)V<16?16:(int)V;
  pYUV[0] = Y;
  pYUV[1] = U;                                                                                                                                                                             
  pYUV[2] = V;
}


bool RestoreDataFromBMP(BYTE* pData, int w, int h, const char* chFileName)
{
  BITMAPFILEHEADER fileheader;
  BITMAPINFOHEADER infoheader;
  FILE* fp;
  int readLen;
  int offset;
  
  fp = fopen(chFileName, "rb");
  if (fp == NULL)
  {
    printf("%s open failed\n", chFileName);
    return false;
  }
  offset = sizeof(fileheader) + sizeof(infoheader);
  fseek(fp, 0, SEEK_END);
  int fsz = ftell(fp);
  offset = ftell(fp) -w*h*4 ;
  fseek(fp, offset, SEEK_SET);
  readLen = fread(pData, 1, w*h*4, fp);
  if (readLen != w*h*4 || fsz != offset + readLen)
  {
    printf("RestoreDataFromBMP() read size from %d not pair, %d | %d fsz:%d\n", offset, readLen, w*h*4, fsz);
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
//UUUUUUUU
//VVVVVVVVV
void MakeYUV444Data(BYTE* pRGB, int w, int h, BYTE* pYUV)
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
   // printf("begin trans...\n");
    RGB2YUV(&pRGB[i], &YUV[0]);
    *pY = YUV[0];
    *pU = YUV[1];
    *pV = YUV[2];
    pY++;
    pU++;
    pV++;
    i += 3;
  }
}

void MakeYUV411HV41Data(BYTE* pRGB, int w, int h, BYTE* pYUV)
{
	BYTE* pSrcRGB;
	BYTE *pY, *pU, *pV;
	int YLEN, ULEN, VLEN;
	int total;
	int i = 0;
	BYTE RGB[4][3];
	BYTE YUV[3];

	pSrcRGB = pRGB;
	YLEN = w*h;
	ULEN = VLEN = YLEN/4;

	pY = pYUV;
	pU = pY + YLEN;
	pV = pU + ULEN;
  total = w*h*3; //input data RGB888

  while (i < total)
  {
      for (int j = 0; j < 4; j++)
      {
		      RGB2YUV(&pRGB[i], &YUV[0]);
		      *pY = YUV[0];		     
		      pY++;
		      if (j == 0)
		      {
		          *pU = YUV[1];
		          *pV = YUV[2];
				      pU++;
				      pV++;				      
		      }
		      i += 3;
      }
  }
	
	
}


void MakeYUV420HV22Data(BYTE* pRGB, int w, int h, BYTE* pYUV)
{
	BYTE *pY, *pU, *pV;
	int YLEN, ULEN, VLEN;
	int total;
	int i = 0, j = 0;
	BYTE YUV[3];
	YLEN = w*h;
	ULEN = VLEN = YLEN/4;

	pY = pYUV;
	pU = pY + YLEN;
	pV = pU + ULEN;
        //total = w*h*3; //input data RGB888

        for (j = 0; j < h; j++) {
            for (i = 0; i< w; i++) {
                RGB2YUV(&pRGB[(j*w + i)*4], &YUV[0]);
                pY[j*w + i] = YUV[0];
            }
        }

        for (j = 0; j < h; j+=2) {
            for (i = 0; i< w; i+=2) {
                RGB2YUV(&pRGB[(j*w + i)*4], &YUV[0]);
                *pU = YUV[1];
	        *pV = YUV[2];
	        pU++;
                pV++;
            }
        }

	/*for (j = 0; j < h; j+=2)
	{
		for (i = 0; i < w; i+=2)
		{
			 RGB2YUV(&pRGB[(j*w + i)*3], &YUV[0]);
			  *pY = YUV[0];
			  *pU = YUV[1];
			  *pV = YUV[2];
			  *(pY+1) = *pY;
			  *(pY+w) = *pY;
			  *(pY+w+1) = *pY;
			  pY+=2;
			  pU+=1;
			  pV+=1;
		}
		pY += w;
	}*/
}

void MakeNV12Data(BYTE* pRGB, int w, int h, BYTE* pYUV)
{
	BYTE* pSrcRGB;
	BYTE *pY, *pU, *pV;
	int YLEN, ULEN, VLEN;
	int total;
	int i = 0, j = 0;
	BYTE RGB[4][3];
	BYTE YUV[3];

	pSrcRGB = pRGB;
	YLEN = w*h;
	ULEN = VLEN = YLEN/4;

	pY = pYUV;
	pU = pY + YLEN;
	pV = pU + ULEN;
        total = w*h*3; //input data RGB888

	for (j = 0; j < h; j+=2)
	{
		for (i = 0; i < w; i+=2)
		{
				RGB2YUV(&pRGB[(j*w + i)*3], &YUV[0]);
			  *pY = YUV[0];
			  *pU = YUV[1];
			  pU++;
			  *pU = YUV[2];
			  *(pY+1) = *pY;
			  *(pY+w) = *pY;
			  *(pY+w+1) = *pY;
			  pY+=2;
			  pU+=1;
		}
		pY += w;
	}
}


void MakeYUVData(BYTE* pRGB, int w, int h, BYTE* pYUV, int type)
{
  switch (type)
  {
    case HV11:
      MakeYUV444Data(pRGB, w, h, pYUV);
      break;
    case HV41:
      MakeYUV411HV41Data(pRGB, w, h, pYUV);
      break;
    case HV22:
      MakeYUV420HV22Data(pRGB, w, h, pYUV);
      break;
    case NV12:
      MakeNV12Data(pRGB, w, h, pYUV);
      break;
  }
}

void SaveYUV2File(BYTE* pData, int len, FILE* fp)
{
  fwrite(pData, len ,1, fp);
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
  
  w = Param.w;
  h = Param.h;
  format = Param.type;
  frameLen = GetYUVFrameLen(format, w, h);
  pYUV = new BYTE[frameLen];
  pData = new BYTE[w*h*4];

  bool loop = true;

  int i = 0;
  int maxLoop = 10;
  char filename[256] = {0};
  sprintf(filename, "%s", Param.filename);

  char newFilename[256] = {0};
  strcpy(newFilename, filename);
  int nameLen = strlen(newFilename);
  newFilename[nameLen-3] = 'y';
  newFilename[nameLen-2] = 'u';
  newFilename[nameLen-1] = 'v';
  FILE* fp = fopen(newFilename, "wb");
  if (fp == NULL)
  {
    printf("error open %s\n", newFilename);
    return 0;
  }
while (1)
{
  i++;
  loop = RestoreDataFromBMP(pData, w, h, filename);
  if (!loop ||  i >= maxLoop) break;
  printf("RestoreDataFromBMP finished\n");
  MakeYUVData(pData, w, h, pYUV, format);
  printf("MakeYUVData[%d] finished\n", i);
  SaveYUV2File(pYUV, frameLen, fp);
  
  printf("%s created (w:%d h:%d)\n", newFilename, w, h);
  //break;
}
  fclose(fp);
  return 0;
}


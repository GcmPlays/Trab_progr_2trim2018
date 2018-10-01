/**
@Author: Victor Guimar�es
@Author: Gabriel Machado
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <time.h>

typedef unsigned short WORD; // 2 Bytes
typedef unsigned int   DWORD;// 4 Bytes

struct bmpheader {
	WORD	bfType;         //Assinatura do arquivo (BM)
	DWORD	bfSize;         //Tamanho do arquivo
	WORD	bfReserved1;    //Reservado
	WORD	bfReserved2;    //Reservado
	DWORD	bfOffBits;      //Numero de bytes do cabe�alho at� o come�o do arquivo
    };

struct bmpinfoheader{
    DWORD biSize;           //Tamanho do cabe�alho, em bytes
    DWORD biWidth;          //Largura em pixels
    DWORD biHeight;         //Altura em pixels
    WORD  biPlanes;         //N� de planos da imagem, deve ser 1
    WORD  biBitCount;       //N�meros de bits por pixel
    DWORD biCompression;    //Compress�o usada
    DWORD biSizeImage;      //Tamanho de dados da imagem
    DWORD biXPelsPerMeter;  //Resolu��o horizontal pixel/m
    DWORD biYPelsPerMeter;  //Resolu��o vertical pixel/m
    DWORD biClrUsed;        //N� de cores usadas
    DWORD biClrImportant;   //N� de cores importantes
    };

void menu(int *escolha);
int headerreader(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo);
int buscacor(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo);

int main()
{
    system("color 0E");//Troca a cor do console para uma totalmente superior
    setlocale(LC_ALL, "Portuguese");//Aplicando l�ngua como Portugu�s

    int option = 0,
        flag   = 0;

    char nome[20];

    struct bmpheader *ptrheader;
    struct bmpinfoheader *ptrinfo;

    //Alocando mem�ria para os ponteiros
    ptrheader = (struct bmpheader*) malloc(sizeof(struct bmpheader));
    ptrinfo   = (struct bmpinfoheader*) malloc(sizeof(struct bmpinfoheader));

    puts("Por favor insira o nome do arquivo");
    gets(nome);
    strcat(nome, ".bmp");
    puts(nome);

    FILE *filePtr;
    filePtr = fopen(nome,"r+b");

    // Caso o programa n�o conseguir abrir a imagem:
     if (filePtr == 0)
     {
        puts("Deu ruim");
        return 0;
    }

    printf("\n");
    printf("Selecione a op��o:\n");
    printf("1. Abrir imagem e exibir informa��es       \n");
    printf("2. Criar imagens com figuras individuais   \n");
    printf("3. Encontrar uma figura colorida           \n");
    printf("4. Converter a imagem para escala de cinza \n");
    printf("5. Encerrar o programa                     \n");

    do
    {
        menu(&option);
        switch(option)
        {
            case 1:
                flag = 1;
                printf("\n");
                headerreader(filePtr,ptrheader,ptrinfo);
                break;
            case 2:
                if(flag != 1) break;
                buscacor(filePtr,ptrheader,ptrinfo);
                break;
            case 3:
                if(flag != 1) break;
                break;
            case 4:
                if(flag != 1) break;
                break;
            case 5:
                flag = 2;
                break;
        }
    }
    while(flag != 2);
    printf("\n");
    fclose(filePtr);
    return 0;
}

void menu(int *escolha)
{
    printf("\nOp��o: ");
    *escolha = ( getche()-'0' );
    if(*escolha < 1 || *escolha > 5)
    {
        printf("\nColoca um valor de 1 a 5 caralho\n"); //Hoje eu to pro crime
        *escolha = 0;
    }
}

int headerreader(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo)
{
    /// bmpheader
    fseek(adr, 0, SEEK_SET);                                //Garantindo que come�a do come�o
    fread(&ptrheader->bfType, sizeof(WORD), 1, adr);        //Lendo assinatura do arquivo
    if(ptrheader->bfType != 0x4d42)                         //Checa assinatura
    {
        puts("O arquivo n�o � .bmp!");
        return 0;
    }
    fread(&ptrheader->bfSize, sizeof(DWORD), 1, adr);       //Lendo Qtd de bytes do cabe�alho
    fread(&ptrheader->bfReserved1, sizeof(WORD), 1, adr);   //Lendo Byte Reservado 1
    fread(&ptrheader->bfReserved2, sizeof(WORD), 1, adr);   //Lendo Byte Reservado 2
    fread(&ptrheader->bfOffBits, sizeof(DWORD), 1, adr);    //Lendo BfOffSetBits
    /// bmpinfoheader
    fread(&ptrinfo->biSize, sizeof(DWORD), 1, adr);         //Lendo Ttamanho do arquivo
    fread(&ptrinfo->biWidth, sizeof(DWORD), 1, adr);        //Lendo Largura
    fread(&ptrinfo->biHeight, sizeof(DWORD), 1, adr);       //Lendo Altura
    fread(&ptrinfo->biPlanes, sizeof(WORD), 1, adr);        //Lendo N� de planos da imagem
    fread(&ptrinfo->biBitCount, sizeof(WORD), 1, adr);      //Lendo Quantidade de bits por pixel
    fread(&ptrinfo->biCompression, sizeof(DWORD), 1, adr);  //Lendo Compress�o usada
    fread(&ptrinfo->biSizeImage, sizeof(DWORD), 1, adr);    //Lendo Tamanho de dados da imagem
    fread(&ptrinfo->biXPelsPerMeter, sizeof(DWORD), 1, adr);//Lendo Resolu��o horizontal pixel/m
    fread(&ptrinfo->biYPelsPerMeter, sizeof(DWORD), 1, adr);//Lendo Resolu��o vertical pixel/m
    fread(&ptrinfo->biClrUsed, sizeof(DWORD), 1, adr);      //Lendo N� de cores usadas
    fread(&ptrinfo->biClrImportant, sizeof(DWORD), 1, adr); //Lendo N� de cores importantes


    printf("\nAssinatura: %c%c",ptrheader->bfType%0x100,ptrheader->bfType/0x100);
    printf("\nO tamanho do arquivo � %x Bytes",ptrheader->bfSize);
    printf("\nO deslocamento do cabe�alho at� o in�cio do arquivo � %d Bytes",ptrheader->bfOffBits );

    printf("\nO tamanho do cabe�alho � %x Bytes",ptrinfo->biSize);
    printf("\nA largura do arquivo � %d pixels",ptrinfo->biWidth);
    printf("\nA altura do arquivo � %d pixels",ptrinfo->biHeight);
    printf("\nO arquivo possui %d bits por pixel",ptrinfo->biBitCount);

    return 0;
}

int buscacor(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo)
{
    int cor = sizeof(char);
    int tamanho = (ptrinfo->biHeight * ptrinfo->biWidth);
    int i;
    FILE *red;
    red = fopen("TesteBmp_R.bmp","w+b");

    fseek(adr, 0, SEEK_SET); //Garantindo que come�a do come�o
    fwrite(ptrheader, sizeof(*ptrheader), 1, red);
    fwrite(ptrinfo, sizeof(*ptrinfo), 1, red);

    fseek(red, ptrheader->bfOffBits, SEEK_SET);

    printf("\n%d %x", cor, cor);

    /*for(i = 0; i < tamanho; i++)
    {
        fread(&cor, 3, 1, adr);

        if(cor == 0xffffff || cor != 0xff0000)
        {
            fwrite(0xff, 3, 1, red);
        }
        else
        {
            fwrite(&cor, 3, 1, red);
        }
    }*/

    return 0;
}

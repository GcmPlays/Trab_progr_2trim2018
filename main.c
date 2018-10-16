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
 WORD bfType;         //Assinatura do arquivo (BM)
 DWORD bfSize;         //Tamanho do arquivo
 WORD bfReserved1;    //Reservado
 WORD bfReserved2;    //Reservado
 DWORD bfOffBits;      //Numero de bytes do cabe�alho at� o come�o do arquivo
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
int grayscale();

int main()
{
    system("color 0E");//Troca a cor do console para uma totalmente superior
    // Atributos de cor s�o especificados por DOIS d�gitos hexadecimais. O primeiro
	//corresponde � cor de plano de fundo; o segundo � cor de primeiro plano. Cada
	//d�gito pode ter apenas um dos seguintes valores:
	
	// 0 = preto            8 = cinza
	// 1 = azul            	9 = azul claro
	// 2 = verde            A = verde claro
	// 3 = aqua             B = aqua claro
	// 4 = vermelho         C = vermelho claro
	// 5 = roxo             D = lil�s
	// 6 = amarelo          E = amarelo claro
	// 7 = branco           F = branco brilhante
	
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
    strcat(nome, ".bmp");//acrescenta bmp ao nome do arquivo digitado
    puts(nome);
    FILE *filePtr;
    filePtr = fopen(nome,"rb");
    
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
        switch(option)//menu de sele��o das op��es principais do programa
        {
            case 1:
                flag = 1;
                printf("\n");
                headerreader(filePtr,ptrheader,ptrinfo);
                break;
                                printf("\n");

            case 2:
                if(flag != 1) break;
                buscacor(filePtr,ptrheader,ptrinfo);
                break;
                                printf("\n");

            case 3:
                if(flag != 1) break;
                break;
                                printf("\n");

            case 4:
            	if(flag != 1) break;
                grayscale ();
                break;
                                printf("\n");

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
        printf("\nO valor digitado n�o est� nas op��es. \nDigite um valor v�lido: \n"); //Hoje eu to pro crime
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
    printf("\nO arquivo possui %d bits por pixel\n",ptrinfo->biBitCount);
    return 0;
}
int buscacor(FILE *adr, struct bmpheader *ptrheader,struct bmpinfoheader *ptrinfo)
{
    int cor = 0, i;
    int tamanho = (ptrinfo->biHeight * ptrinfo->biWidth);
    FILE *redptr;
    FILE *greenptr;
    FILE *blueptr;
    redptr = fopen("TesteBmp_R.bmp","w+b");
    greenptr = fopen("TesteBmp_G.bmp","w+b");
    blueptr= fopen("TesteBmp_B.bmp","w+b");
    
    //Passando cabe�alho para novo arquivo
    fwrite(&ptrheader->bfType, sizeof(WORD), 1, redptr);
    fwrite(&ptrheader->bfSize, sizeof(*ptrheader)-4, 1, redptr);
    fwrite(ptrinfo, sizeof(*ptrinfo), 1, redptr);
    fseek(redptr, ptrheader->bfOffBits, SEEK_SET);
    fseek(adr, ptrheader->bfOffBits, SEEK_SET);
    DWORD white = 0xffffff;
    DWORD red   = 0x0000ff;
    DWORD blue  = 0xff0000;
    DWORD green = 0x00ff00;
    int nulo = 0;

    puts(" ");
    for(i = 0; i <= ptrheader->bfSize; i++ )
    {
        fread(&cor, 3, 1, adr);
        //printf("%x\t\n", cor);

  if(cor==white)
  {
   fwrite(&nulo, 3, 1, redptr);
  }
  else
  {
   cor&=0x00ff00;
   fwrite(&cor, 3, 1, redptr);
  }
   }
   
    
}
    int grayscale()
	{
	int *header1, *tamanho, *header2;
	unsigned char *mapa;
	unsigned char *gs;
	int ctrl=0, i;
    unsigned char cor[3];
    FILE *open;
    open=fopen("TesteBmp.bmp","rb"); //abre arquivo
    header1=malloc(34); //aloca memoria
    tamanho=malloc(4);
    header2=malloc(16);
    
    fread(header1,34,1,open); //le dados
    fread(tamanho,4,1,open);
    fread(header2,16,1,open);
    mapa=malloc(*tamanho); //aloca memoria mapa
    gs=malloc(*tamanho); //aloca memoria grayscale
    fread(mapa,*tamanho,1,open); //le mapa de bits
    fclose(open);
    
    while(ctrl<*tamanho)
	{
        for(i=0;i<2;i++)//limita o n�mero de cores para 3(vermelho,verde e azul)
		{
            cor[i]=*mapa;
            mapa++;
            ctrl++;
        }

        for(i=0;i<3;i++){ //limita o n�mero de cores para 3(vermelho,verde e azul)
            *gs=((cor[0]*0.3)+(cor[1]*0.59)+(cor[2]*0.11)); //define a quantidade de cada cor
            gs++; //avanca bit
        }
    }
    gs=gs-*tamanho; //posiciona no inicio do endere�o
    open=fopen("TesteBmp_gs.bmp","wb");
    fwrite(header1,34,1,open); //escreve arquivo
    fwrite(tamanho,4,1,open);
    fwrite(header2,16,1,open);
    fwrite(gs,*tamanho,1,open);
    fclose(open);


    //Liberando espa�o de mem�ria
    free(mapa);
    free(gs);
    free(header1);
    free(tamanho);
    free(header2);

    //Exibe confirma��o na tela
    printf("\nImagem gerada com �xito com o nome de Bmp_gs.bmp!\n");
	}


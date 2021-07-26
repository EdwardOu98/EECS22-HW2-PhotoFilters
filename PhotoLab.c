/*********************************************************************/
/* PhotoLab.c: assignment 2 for EECS 22, Fall 2018                   */
/*                                                                   */
/* modifications: Mihnea Chirila                         	     */
/* 09/27/17 RD	adjusted for lecture usage                           */
/* 10/1/18 MC	 adjusted for F18                          	     */
/*********************************************************************/
/* Enter your name, last name and UCInet ID below:
 *
 * *****************************
 * First Name : Edward
 * Last Name: Ou
 * UCInet ID: 15561434
 *
 */

/*** header files ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*** global definitions ***/

#define SLEN 80 		/* maximum length of file names and string*/
#define SUCCESS 0		/* return code for success */
#define EXIT 7			/* menu item number for EXIT */
#define MAX_PIXEL 255		/* max pixel value */
#define MIN_PIXEL 0		/* min pixel value */
#define SHUFF_HEIGHT_DIV 4      /* Height division for shuffleing */
#define SHUFF_WIDTH_DIV 4       /* Width division for shuffleing */

const int WIDTH  = 600;	/* image width */
const int HEIGHT = 400;	/* image height */

/*** function declarations ***/

void PrintMenu();

/* read image from a file */
int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT],
              unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* save a processed image */
int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT],
              unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/* edge detection */
void Edge(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
          unsigned char B[WIDTH][HEIGHT]);

/* change color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
                 unsigned char B[WIDTH][HEIGHT]);

/* sharpen the image */
void Shuffle(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]);

/* Test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT],
              unsigned char B[WIDTH][HEIGHT]);

/*** main program ***/

int main(void)
{
    unsigned char R[WIDTH][HEIGHT];
    unsigned char G[WIDTH][HEIGHT];
    unsigned char B[WIDTH][HEIGHT];
    int i;
    int j = 0;
    char f[SLEN];
    char s[SLEN]; 

    do{
        do{
            PrintMenu();
            scanf("%d", &i);
            
            if(i > 0 && i <= 7){
                j = 0;
            }
            else{
                j = 1;
                printf("Invalid input! Please enter a number according to the instruction!\n");
            }
        }while(j == 1);
        
        if(i == 1){
            printf("Please input the file name to load: ");
            scanf("%s", f);
            LoadImage(f, R, G, B);
        }
        else if(i == 2){
            printf("Please input the file name to save: ");
            scanf("%s", s);
            SaveImage(s, R, G, B);
        }
        else if(i == 3){
            BlackNWhite(R, G, B);
        }
        else if(i == 4){
            Edge(R, G, B);
        }
        else if(i == 5){
            Shuffle(R, G ,B);
        }
        else if(i == 6){
            AutoTest(R, G, B);
        }
        else if(i == EXIT){
            printf("Thank you for trying out this program! Hope to see you again soon!\n");
            break;
        }
    }while(1);

    return 0;
} /* end of main */


/*** function definitions ***/

/* read a photo from the specified file into the specified */
/* RGB data structure; returns 0 for success, >0 for error */


int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
        FILE           *File;
        char            Type[SLEN];
        int             Width, Height, MaxValue;
        int             x, y;
        char            ftype[] = ".ppm";
        char            fname_tmp[SLEN];
        strcpy(fname_tmp, fname);
        strcat(fname_tmp, ftype);

        File = fopen(fname_tmp, "r");
        if (!File) {
                printf("Cannot open file \"%s\" for reading!\n", fname);
                return 1;
                    }
        fscanf(File, "%79s", Type);
        if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
                printf("\nUnsupported file format!\n");
                return 2;
                                                                }
        fscanf(File, "%d", &Width);
        if (Width != WIDTH) {
                printf("\nUnsupported image width %d!\n", Width);
                return 3;
                             }
        fscanf(File, "%d", &Height);
        if (Height != HEIGHT) {
                 printf("\nUnsupported image height %d!\n", Height);
                 return 4;
                              }
        fscanf(File, "%d", &MaxValue);
        if (MaxValue != 255) {
                 printf("\nUnsupported image maximum value %d!\n", MaxValue);
                 return 5;
                            }
        if ('\n' != fgetc(File)) {
                 printf("\nCarriage return expected!\n");
                 return 6;
                                }
        for (y = 0; y < HEIGHT; y++)
             for (x = 0; x < WIDTH; x++) {
                         R[x][y] = fgetc(File);
                         G[x][y] = fgetc(File);
                         B[x][y] = fgetc(File);
                                         }
        if (ferror(File)) {
             printf("\nFile error while reading from file!\n");
             return 7;
                           }
        printf("%s was read.\n", fname_tmp);
        fclose(File);
        return 0;
}

int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
        FILE    *File;
        int x, y;
        char    SysCmd[SLEN * 5];
        char    ftype[] = ".ppm";
        char    fname_tmp[SLEN];  /*avoid to modify on the original pointer, 11/10/10, X.Han*/
        char    fname_tmp2[SLEN];

        strcpy(fname_tmp, fname);
        strcpy(fname_tmp2, fname);
        strcat(fname_tmp2, ftype);

        File = fopen(fname_tmp2, "w");
        if (!File) {
                printf("Cannot open file \"%s\" for writing!\n", fname);
                return 1;
                   }
        fprintf(File, "P6\n");
        fprintf(File, "%d %d\n", WIDTH, HEIGHT);
        fprintf(File, "255\n");
        for (y = 0; y < HEIGHT; y++)
                 for (x = 0; x < WIDTH; x++) {
                               fputc(R[x][y], File);
                               fputc(G[x][y], File);
                               fputc(B[x][y], File);
                                              }
         if (ferror(File)) {
                  printf("\nFile error while writing to file!\n");
                  return 2;
                            }
         fclose(File);
         printf("%s was saved. \n", fname_tmp2);
   /*
     *      * rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
     *           * and make it world readable
                                   *                */
        sprintf(SysCmd, "/users/grad2/doemer/eecs22/bin/pnmtojpeg_hw2.tcsh %s", fname_tmp2);
        if (system(SysCmd) != 0) {
        printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
        return 3;
                                  }
        printf("%s.jpg was stored.\n", fname_tmp);
        return 0;
}

/*Print Menu Function*/
void PrintMenu(){
    printf("--------------------------------\n");
    printf("1: Load a PPM image\n");
    printf("2: Save an image in PPM and JPEG format\n");
    printf("3: Change a color image to Black & White\n");
    printf("4: Sketch the edge of an image\n");
    printf("5: Shuffle an image\n");
    printf("6: Test all functions\n");
    printf("7: Exit\n");
    printf("Please make your choice: ");
}


/*BlackNWhite Function
 * This function will change a color picture to black and white*/
void BlackNWhite(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]){
    int x, y;
    unsigned char tmp;

    for(y = 0; y < HEIGHT; y++)
    {   for(x = 0; x < WIDTH; x++)
        {
            tmp = (R[x][y] + G[x][y] + B[x][y]) / 3;
            R[x][y] = tmp;
            G[x][y] = tmp;
            B[x][y] = tmp;
        }
    }

    printf("The image have been modified to black and white! \n");
}

/*Edge Function
 * This function will find the edge of an image*/
void Edge(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]){
    int x, y;
    int new_E;
    int tmp, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
    int tR1[WIDTH][HEIGHT], tG1[WIDTH][HEIGHT], tB1[WIDTH][HEIGHT];
    
    for(y = 1; y < HEIGHT - 1; y++)
    {
        for(x = 1; x < WIDTH - 1; x++)
        {
            tmp = (R[x][y]+G[x][y]+B[x][y])/3;
            tmp1 = (R[x-1][y-1]+G[x-1][y-1]+B[x-1][y-1])/3;
            tmp2 = (R[x][y-1]+G[x][y-1]+B[x][y-1])/3;
            tmp3 = (R[x+1][y-1]+G[x+1][y-1]+B[x+1][y-1])/3;
            tmp4 = (R[x-1][y]+G[x-1][y]+B[x-1][y])/3;
            tmp5 = (R[x+1][y]+G[x+1][y]+B[x+1][y])/3;
            tmp6 = (R[x-1][y+1]+G[x-1][y+1]+B[x-1][y+1])/3;
            tmp7 = (R[x][y+1]+G[x][y+1]+B[x][y+1])/3;
            tmp8 = (R[x+1][y+1]+G[x+1][y+1]+B[x+1][y+1])/3;

            new_E = 8*tmp - tmp1 - tmp2 - tmp3 - tmp4 - tmp5 - tmp6 - tmp7 - tmp8;

            tR1[x][y] = new_E;
            tG1[x][y] = new_E;
            tB1[x][y] = new_E;
        }
    }

    for(y = 1; y < HEIGHT - 1; y++)
    {
        for(x = 1; x < WIDTH - 1; x++)
        {
            if(tR1[x][y] < 75)
            {
                R[x][y] = MIN_PIXEL;
                G[x][y] = MIN_PIXEL;
                B[x][y] = MIN_PIXEL;
            }
            else if(tR1[x][y] > 75 && tR1[x][y] < 130)
            {
                R[x][y] = tR1[x][y];
                G[x][y] = tG1[x][y];
                B[x][y] = tB1[x][y];
            }
            else
            {
               R[x][y] = MAX_PIXEL;
               G[x][y] = MAX_PIXEL;
               B[x][y] = MAX_PIXEL;
            }
        }
    }

    for(x = 0; x < WIDTH; x++)
    {
        R[x][0] = MIN_PIXEL;
        R[x][399] = MIN_PIXEL;
        G[x][0] = MIN_PIXEL;
        G[x][399] = MIN_PIXEL;
        B[x][0] = MIN_PIXEL;
        B[x][399] = MIN_PIXEL;
    }

    for(y = 1; y < HEIGHT - 1; y++)
    {
        R[0][y] = MIN_PIXEL;
        R[599][y] = MIN_PIXEL;
        G[0][y] = MIN_PIXEL;
        G[599][y] = MIN_PIXEL;
        B[0][y] = MIN_PIXEL;
        B[599][y] = MIN_PIXEL;
    }

    printf("Edge detection finished!\n");
}

/*Shuffle function
 * This function randomly swap parts of the image*/
void Shuffle(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    unsigned char tmp;
    int x, y, i, j;
    int a, s, w, q, m;
    int counter = 0;
    int rx, ry, ri, rj;

    srand(time(0));
    
    do{
        rx = rand() % 4;
        w = WIDTH / SHUFF_WIDTH_DIV * rx;
        a = w + 149;

        ry = rand() % 4;
        q = HEIGHT / SHUFF_HEIGHT_DIV * ry;
        s = q + 99;

        ri  = rand() % 4;
        m = WIDTH / SHUFF_WIDTH_DIV * ri;

        rj = rand() % 4;
        j = HEIGHT / SHUFF_HEIGHT_DIV * rj;

        for(y = q; y <= s; y++)
        {
            for(x = w, i = m; x <= a; x++)
            {
                tmp = R[x][y];
                R[x][y] = R[i][j];
                R[i][j] = tmp;

                tmp = G[x][y];
                G[x][y] = G[i][j];
                G[i][j] = tmp;

                tmp = B[x][y];
                B[x][y] = B[i][j];
                B[i][j] = tmp;

                i++;
            }
            j++;
        }

        counter++;

    }while(counter < 8);
    
}

/*AutoTest Function
 * This function automatically test all the filters in this program*/
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    LoadImage("HSSOE", R, G, B);
    BlackNWhite(R, G, B);
    SaveImage("bw", R, G, B);
    printf("Black & White Tested! \n");

    LoadImage("HSSOE", R, G, B);
    Edge(R, G, B);
    SaveImage("edge", R, G, B);
    printf("Edge Detection Tested! \n");

    LoadImage("HSSOE", R, G, B);
    Shuffle(R, G, B);
    SaveImage("shuffle", R, G, B);
    printf("Shuffle Tested! \n");
    
    printf("\nAll DIP have been tested! \n");
}

/*


        .
        .
        .
        .
        .



*/




/* EOF */

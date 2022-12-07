#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Picture{
    int res;
    float** image;
}Picture;

float dsigmoid(float x)
{
    float e = exp(-x);
    return e/((1+e)*(1+e));
}

float sigmoid(float x)
{
    return 1./(1+exp(-x));
}

float** picture;
int layerSizes2[10] = {0,0,0,0,0,0,784,1000,1000,10};
float* layers2[10] = {0};
float* errors2[10] = {0};
float*  weights2[10] = {0};

int activation = 1; // Sigmoid
                    // INITIALIZATION

void save_weight()
{
    FILE *ptr = fopen("weight.txt", "w");
    if (ptr == NULL)
    {
        printf("not enough memory");
        return;
    }
    fclose(ptr);
}


void initNet(){
    // ALOCATE MEMORY
    layers2[0] = (float*)malloc((layerSizes2[0]+1) * sizeof(float));
    errors2[0] = (float*)malloc(layerSizes2[0] * sizeof(float));
    for (int i=1;i<10;i++){
        layers2[i] = (float*)malloc((layerSizes2[i]+1) * sizeof(float));
        errors2[i] = (float*)malloc(layerSizes2[i] * sizeof(float));
        weights2[i] = (float*)malloc(layerSizes2[i] * (layerSizes2[i-1]+1) * sizeof(float));
    }
    // RANDOMIZE WEIGHTS AND BIAS
    float scale;
    for (int i=0;i<10;i++) layers2[i][layerSizes2[i]]=1.0;
    for (int j=1;j<10;j++){
        // XAVIER-HE INITIALIZATION
        scale = 2.0 * sqrt(6.0/(layerSizes2[j] + layerSizes2[j-1]));
        if (j!=9 && activation==1) 
            scale = scale * 1.41; // Sigmoid
        for (int i=0;i<layerSizes2[j] * (layerSizes2[j-1]+1);i++)
            weights2[j][i] = scale * ( (float)rand()/RAND_MAX - 0.5 );
        for (int i=0;i<layerSizes2[j];i++) // BIASES
            weights2[j][(layerSizes2[j-1]+1)*(i+1)-1] = 0.0;
    }
}

void decrypt_picture(char* file, float*** input)
{
    SDL_Surface* picture = IMG_Load(file);
    if (picture  == NULL)
        errx(EXIT_FAILURE, "pls enter an existing file");
    SDL_LockSurface(picture);
    Uint32* pixels = picture -> pixels;
    SDL_PixelFormat* format = picture -> format;
    Uint8 r,g,b;
    for (int i=0;i<28;i++)
        for (int j =0;j<28;j++)
        {
            SDL_GetRGB(pixels[i*28+j],format,&r,&g,&b);
            *(*(*(input)+i)+j) = ((r+g+b)/3);
        }        
    SDL_FreeSurface(picture);
}

int numLayers = 4;
// FORWARD PROPAGATION
int forwardProp(){
    int i,j,k,imax;
    float sum, esum, max;
    // INPUT LAYER - RECEIVES 28X28 IMAGES
    for (i=0;i<784;i++) layers2[10-numLayers][i] = picture[i/28][i%28];
    // HIDDEN LAYERS - Sigmoid ACTIVATION
    for (k=11-numLayers;k<9;k++)
        for (i=0;i<layerSizes2[k];i++){
            sum = 0.0;
            for (j=0;j<layerSizes2[k-1]+1;j++)
                sum += layers2[k-1][j]*weights2[k][i*(layerSizes2[k-1]+1)+j];
            layers2[k][i] = sigmoid(sum);
        }
    // OUTPUT LAYER - SOFTMAX ACTIVATION
    esum = 0.0;
    for (i=0;i<layerSizes2[9];i++){
        sum = 0.0;
        for (j=0;j<layerSizes2[8]+1;j++)
            sum += layers2[8][j]*weights2[9][i*(layerSizes2[8]+1)+j];
        if (sum>30) return -1; //GRADIENT EXPLODED
        layers2[9][i] = exp(sum);
        esum += layers2[9][i];
    }
    // SOFTMAX FUNCTION
    max = layers2[9][0]; imax=0;
    for (i=0;i<layerSizes2[9];i++){
        if (layers2[9][i]>max){
            max = layers2[9][i];
            imax = i;
        }
        layers2[9][i] = layers2[9][i] / esum;
        printf("%d = %f\n",i,layers2[9][i]);
    }
    return imax;
}

float learn = 0.01;
float decay = 0.95;
// BACKPROPAGATION
int backProp(int x, int epoch, float *ent){
    int i, j, k, r = 0;
    // FORWARD PROP FIRST
    int p = forwardProp();
    if (p==-1) return -1; // GRADIENT EXPLODED
                          // CORRECT PREDICTION?
    if (p==x) r=1;
    // OUTPUT LAYER - CALCULATE ERRORS
    for (i=0;i<layerSizes2[9];i++){
        errors2[9][i] = learn * (0-layers2[9][i]) * pow(decay,epoch);
        if (i==x){
            errors2[9][i] = learn * (1-layers2[9][i]) * pow(decay,epoch);
            if (layers2[9][i]==0) return -1; // GRADIENT VANISHED
            *ent = -log(layers2[9][i]);
        }
    }
    // HIDDEN LAYERS - CALCULATE ERRORS
    for (k=8;k>10-numLayers;k--)
        for (i=0;i<layerSizes2[k];i++){
            errors2[k][i] = 0;
            if (layers2[k][i]>0) // Sigmoid DERIVATIVE
                for (j=0;j<layerSizes2[k+1];j++)
                    errors2[k][i] += errors2[k+1][j]*dsigmoid(weights2[k+1][j*(layerSizes2[k]+1)+i]);
        }
    // UPDATE WEIGHTS - GRADIENT DESCENT
    for (k=11-numLayers;k<10;k++)
        for (i=0;i<layerSizes2[k];i++)
            for (j=0;j<layerSizes2[k-1]+1;j++)
                weights2[k][i*(layerSizes2[k-1]+1)+j] += errors2[k][i]*layers2[k-1][j];
    return r;
}

void init(float*** picture)
{
    *picture = malloc(28*sizeof(float*));
    for(int i= 0; i<28; i++)
        *(*picture+i) = malloc(28*sizeof(float));
}

int main()
{
    char* file;
    init(&picture);
    initNet();
    float res;
    for (int i=0;i<100;i++)
    {
        printf("----------------epoch: %d----------------\n",i+1);
        for (size_t k=0;k<=9;k++)
        {
            int x = ((int)rand())%306;
            char* nb;
            asprintf(&nb,"%d",x);
            asprintf(&file,"%d-%s%s.png",k,strlen(nb)==1 ? "000"
                    : strlen(nb)==2 ? "00" : "0",nb);
            printf("%s",file);
            decrypt_picture(file,&picture);
            backProp(k+1,i,&res);
        }
    }
    save_weight();
    return EXIT_SUCCESS;
}

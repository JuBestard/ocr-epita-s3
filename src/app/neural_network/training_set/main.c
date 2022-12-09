#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

double ReLU(double x)
{
    return x > 0 ? x : 0;
}

double dsigmoid(double x)
{
    double e = exp(-x);
    return e/((1+e)*(1+e));
}

double sigmoid(double x)
{
    return 1./(1+exp(-x));
}

double** picture;
int layerSizes[10] = {0,0,0,0,0,0,784,1000,1000,10};
double* layers[10] = {0};
double* errors[10] = {0};
double*  weights[10] = {0};

int activation = 2; // TanH
                    // INITIALIZATION

int test[1] = {0};

void save_weight()
{
    FILE *ptr = fopen("weight.txt", "w");
    if (ptr == NULL)
    {
        printf("Not enough memory\n");
        return;
    }
    for(int i=7;i<10;i++)
        for(int j=0;j<layerSizes[i]*layerSizes[i-1]+1;j++)
        {
            fprintf(ptr,"%f\n",weights[i][j]);
        }
    fclose(ptr);
}

int load_weight()
{
    FILE *ptr = fopen("weight.txt", "r");
    if (ptr == NULL)
    {
        printf("Not enough memory\n");
        return 0;
    }
    for(int i=7;i<10;i++)
        for(int j=0;j<layerSizes[i]*layerSizes[i-1]+1;j++)
        {
            fscanf(ptr,"%lf\n",&weights[i][j]);
        }
    fclose(ptr);
    return 1;
}



// INITIALIZATION
void initNet(){
    // ALOCATE MEMORY
    int i,j;
    layers[0] = (double*)malloc((layerSizes[0]+1) * sizeof(double));
    errors[0] = (double*)malloc(layerSizes[0] * sizeof(double));
    for (i=1;i<10;i++){
        layers[i] = (double*)malloc((layerSizes[i]+1) * sizeof(double));
        errors[i] = (double*)malloc(layerSizes[i] * sizeof(double));
        weights[i] = (double*)malloc(layerSizes[i] * (layerSizes[i-1]+1) * sizeof(double));
    }
    if (!load_weight())
    {
        // RANDOMIZE WEIGHTS AND BIAS
        double scale;
        for (i=0;i<10;i++) layers[i][layerSizes[i]]=1.0;
        for (j=1;j<10;j++){
            // XAVIER-HE INITIALIZATION
            scale = 2.0 * sqrt(6.0/(layerSizes[j] + layerSizes[j-1]));
            if (j!=9 && activation==1) scale = scale * 1.41; // RELU
            else if (j!=9) scale = scale * 4.0; // TANH
            for (i=0;i<layerSizes[j] * (layerSizes[j-1]+1);i++)
                weights[j][i] = scale * ( (double)rand()/RAND_MAX - 0.5 );
            for (i=0;i<layerSizes[j];i++) // BIASES
                weights[j][(layerSizes[j-1]+1)*(i+1)-1] = 0.0;
        }
    }
}

void decrypt_picture(char* file, double*** input)
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
    double sum, esum, max;
    // INPUT LAYER - RECEIVES 28X28 IMAGES
    for (i=0;i<784;i++) layers[10-numLayers][i] = picture[i/28][i%28];
    // HIDDEN LAYERS - RELU ACTIVATION
    for (k=11-numLayers;k<9;k++)
        for (i=0;i<layerSizes[k];i++){
            sum = 0.0;
            for (j=0;j<layerSizes[k-1]+1;j++)
                sum += layers[k-1][j]*weights[k][i*(layerSizes[k-1]+1)+j];
            if (activation==1) layers[k][i] = ReLU(sum);
            else if (activation==2) layers[k][i] = tanh(sum);
        }
    // OUTPUT LAYER - SOFTMAX ACTIVATION
    esum = 0.0;
    for (i=0;i<layerSizes[9];i++){
        sum = 0.0;
        for (j=0;j<layerSizes[8]+1;j++)
            sum += layers[8][j]*weights[9][i*(layerSizes[8]+1)+j];
        if (sum>30) 
        {
            printf("exploded");
            return -1; //GRADIENT EXPLODED
        }
        layers[9][i] = exp(sum);
        esum += layers[9][i];
    }
    // SOFTMAX FUNCTION
    max = layers[9][0]; imax=0;
    for (i=0;i<layerSizes[9];i++){
        if (layers[9][i]>max){
            max = layers[9][i];
            imax = i;
        }
        layers[9][i] /= esum;
        if(test[0]>47)
            printf("%d = %f\n",i,layers[9][i]);
    }
    return imax;
}

double learn = 0.01;
double decay = 0.95;
// BACKPROPAGATION
int backProp(int x, int epoch, double *ent){
    int i, j, k, r = 0;
    double der = 1.0;
    // FORWARD PROP FIRST
    int p = forwardProp();
    if (p==-1) return -1; // GRADIENT EXPLODED
                          // CORRECT PREDICTION?
    int y = x;
    if (p==y) r=1;
    // OUTPUT LAYER - CALCULATE ERRORS
    for (i=0;i<layerSizes[9];i++){
        errors[9][i] = learn * (0-layers[9][i]);// * pow(decay,epoch);
        if (i==y) {
            errors[9][i] = learn * (1-layers[9][i]);// * pow(decay,epoch);
            if (layers[9][i]==0) 
            {
                printf("vanished");
                return -1; // GRADIENT VANISHED
            }
            *ent = -log(layers[9][i]);
        }
    }
    // HIDDEN LAYERS - CALCULATE ERRORS
    for (k=8;k>10-numLayers;k--)
        for (i=0;i<layerSizes[k];i++){
            errors[k][i] = 0;
            if (activation==2) der = (layers[k][i]+1)*(1-layers[k][i]); // TanH DERIVATIVE
            if (layers[k][i]>0 || activation==2) // ReLU DERIVATIVE
                for (j=0;j<layerSizes[k+1];j++)
                    errors[k][i] += errors[k+1][j]*weights[k+1][j*(layerSizes[k]+1)+i]*der;
        }
    // UPDATE WEIGHTS - GRADIENT DESCENT
    for (k=11-numLayers;k<10;k++)
        for (i=0;i<layerSizes[k];i++)
            for (j=0;j<layerSizes[k-1]+1;j++)
                weights[k][i*(layerSizes[k-1]+1)+j] += errors[k][i]*layers[k-1][j];
    return r;
}

void init(double*** picture)
{
    *picture = malloc(28*sizeof(double*));
    for(int i= 0; i<28; i++)
        *(*picture+i) = malloc(28*sizeof(double));
}

int main()
{
    char* file;
    init(&picture);
    initNet();
    double res;
    int i = 0;
    for (;i<50;i++)
    {
        printf("----------------epoch: %d----------------\n",i+1);
        *test = i+1;
        for (size_t k=0;k<=9;k++)
        {
            int x = ((int)rand())%306;
            char* nb;
            asprintf(&nb,"%d",x);
            asprintf(&file,"%d-%s%s.png",k,strlen(nb)==1 ? "000"
                    : strlen(nb)==2 ? "00" : "0",nb);
            decrypt_picture(file,&picture);
            if (i>=47)
                printf("%s\n",file);
            backProp(k,i,&res);
        }
    }
    save_weight();
    return EXIT_SUCCESS;
}

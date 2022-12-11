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
int layerSizes[10] = {784,1000,1000,1000,1000,1000,1000,1000,1000,10};
double* layers[10] = {0};
double* errors[10] = {0};
double*  weights[10] = {0};

int activation = 2; // TanH
                    // INITIALIZATION

int numLayers = 10;
int test[1] = {0};

int epoch = 10;

void save_weight()
{
    FILE *ptr = fopen("src/neuralnetwork/nn/training_set/weight.txt", "w");
    if (ptr == NULL)
    {
        printf("save_weight: File not found\n");
        return;
    }
    for(int i=11-numLayers;i<10;i++)
        for(int j=0;j<layerSizes[i]*layerSizes[i-1]+1;j++)
        {
            fprintf(ptr,"%f\n",weights[i][j]);
        }
    fclose(ptr);
}

int load_weight()
{
    FILE *ptr = fopen("src/neuralnetwork/nn/training_set/weight.txt", "r");
    if (ptr == NULL)
    {
        printf("load_weight: File not found\n");
        return 0;
    }
    double temp;
    for(int i=11-numLayers;i<10;i++)
        for(int j=0;j<layerSizes[i]*layerSizes[i-1]+1;j++)
        {
            fscanf(ptr,"%lf\n",&temp);
            weights[i][j] = temp;
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
    if (load_weight() == 0)
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

int decrypt_picture(int k, double*** input)
{
    char* file = "";
    SDL_Surface* image;
    int res = -1;

    int x = ((int)rand())%3;
    asprintf(&file,"out/save/%d_%d.bmp",k,x);
    image = SDL_LoadBMP(file);
    if (picture  == NULL)
        errx(EXIT_FAILURE, "pls enter an existing file");
    res = x;

    printf("%s\n",file);
    SDL_LockSurface(image);
    Uint32* pixels = image -> pixels;
    SDL_PixelFormat* format = image -> format;
    Uint8 r,g,b;
    for (int i=0;i<28;i++)
        for (int j =0;j<28;j++)
        {
            SDL_GetRGB(pixels[i*28+j],format,&r,&g,&b);
            //*(*(*(input)+i)+j) = ((r+g+b)/3);
            *(*(*(input)+i)+j) = ((r+g+b)/3) >= 250 ? 0 : 255;
        }        
    SDL_FreeSurface(image);
    return res;
}

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
    }
    return imax;
}

double learn = 0.001;
double decay = 0.95;
// BACKPROPAGATION
int backProp(int x, double *ent){
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
    return 1;
}
//int main(int argc, char** argv)
//{
//    int sol[] = 
//    {
//        5,3,0,0,7,0,0,0,0,
//        6,0,0,1,9,5,0,0,0,
//        0,9,8,0,0,0,0,6,0,
//        8,0,0,0,6,0,0,0,3,
//        4,0,0,8,0,3,0,0,1,
//        7,0,0,0,2,0,0,0,6,
//        0,6,0,0,0,0,2,8,0,
//        0,0,0,4,1,9,0,0,5,
//        0,0,0,0,8,0,0,7,9,
//        0,2,0,0,0,0,6,0,9,
//        8,5,7,0,6,4,2,0,0,
//        0,9,0,0,0,1,0,0,0,
//        0,1,0,6,5,0,3,0,0,
//        0,0,8,1,0,3,5,0,0,
//        0,0,3,0,2,9,0,8,0,
//        0,0,0,4,0,0,0,6,0,
//        0,0,2,8,7,0,1,3,5,
//        1,0,6,0,0,0,0,2,0,
//        7,0,8,9,0,0,0,0,2,
//        5,1,3,0,0,2,0,0,8,
//        0,9,2,3,1,0,0,0,7,
//        0,5,0,0,3,0,9,0,0,
//        1,6,0,0,2,0,0,7,5,
//        0,0,9,0,4,0,0,6,0,
//        9,0,0,0,8,4,2,1,0,
//        2,0,0,6,0,0,7,4,9,
//        4,0,0,0,0,1,5,0,3
//    };
//    if (argc != 2)
//        errx(EXIT_FAILURE,"./neuralnet <epoch>");
//    epoch = atoi(argv[1]);
//    init(&picture);
//    initNet();
//    double res;
//    int i = 0;
//    *test = epoch;
//    for (;i<epoch;i++)
//    {
//        if ((i+1)%1==0)
//            printf("----------------epoch: %d----------------\n",i+1);
//        *test = i+1;
//        for (size_t k=0;k<81;k++)
//        {
//            int fact = decrypt_picture(k,&picture);
//            if (fact == -1)
//                backProp(k%10,&res);
//            else
//                backProp(sol[fact*81+k],&res);
//            if (i>=epoch-3)
//                printf("solution is: %d\n",sol[fact*81+k]);
//        }
//    }
//    //for (size_t k=0;k<=9;k++)
//    //{
//    //for (int k=0;k<81;k++)
//    //{
//    //    decrypt_picture(k,&picture);
//    //    for(int i=0;i<28;i++)
//    //    {
//    //        for(int j=0;j<28;j++)
//    //            printf("%s",picture[i][j]>=200 ? "█" : " ");
//    //        printf("\n");
//    //    }
//    //    printf("this picture is %d\n",forwardProp());
//    //}
//    //}
//
//    save_weight();
//    return EXIT_SUCCESS;
//}

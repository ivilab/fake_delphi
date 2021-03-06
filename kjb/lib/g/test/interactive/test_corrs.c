#include <g/g_camera_matrix.h>
#include <g/g_point_projection.h>

#define TEST_DIR "ransac_test_#/"
#define CAMERA_FILE "camera_matrix"
#define P2D_FILE "points2d"
#define P3D_FILE "points3d"
#define L2D_FILE "lines2d"
#define L3DA_FILE "lines3da"
#define L3DB_FILE "lines3db"
#define STR_SZ 28
#define IDX 12
#define NUM_TESTS 10
#define DEFAULT_POINTS_MIN 8
#define DEFAULT_POINTS_MAX 8
#define DEFAULT_LINES_MIN 12
#define DEFAULT_LINES_MAX 12

void runTest(int);
void PASS_OR_FAIL(int);
void free_corrs(void);

Matrix* p_proj=0;
Matrix* new_proj=0;

Vector_vector* p2d=0;
Vector_vector* p3d=0;
Vector_vector* l2d=0;
Vector_vector* l3da=0;
Vector_vector* l3db=0;

Matrix* l_proj=0;
Vector* plucker=0;
Vector* plot=0;

double totErr=0;
double ptErr=0;
double lnErr=0;

char* camFile;
char* point2File;
char* point3File;
char* line2File;
char* line3aFile;
char* line3bFile;
int tests_failed=0;

int numTests=NUM_TESTS;
int pMin=DEFAULT_POINTS_MIN;
int pMax=DEFAULT_POINTS_MAX;
int lMin=DEFAULT_LINES_MIN;
int lMax=DEFAULT_LINES_MAX;


int main(int argc, char** argv)
{
    int i;

    if (! is_interactive()) 
    {
        p_stderr("This test program either needs to be adjusted for batch testing or moved.\n");
        p_stderr("Forcing failure to help increase the chances this gets done.\n"); 
        return EXIT_CANNOT_TEST;
    }
    
    if (argc>1) {
        numTests=atoi(argv[1]);
    }
    if (argc>2) {
        pMin=atoi(argv[2]);
    }
    if (argc>3) {
        pMax=atoi(argv[3]);
    }
    if (argc>4) {
        lMin=atoi(argv[4]);
    }
    if (argc>5) {
        lMax=atoi(argv[5]);
    }

    camFile = (char*)malloc(sizeof(char)*STR_SZ);
    point2File = (char*)malloc(sizeof(char)*STR_SZ);
    point3File = (char*)malloc(sizeof(char)*STR_SZ);
    line2File = (char*)malloc(sizeof(char)*STR_SZ);
    line3aFile = (char*)malloc(sizeof(char)*STR_SZ);
    line3bFile = (char*)malloc(sizeof(char)*STR_SZ);

    camFile = strcpy(camFile,TEST_DIR); camFile = strcat(camFile,CAMERA_FILE);
    point2File = strcpy(point2File,TEST_DIR); point2File = strcat(point2File,P2D_FILE);
    point3File = strcpy(point3File,TEST_DIR); point3File = strcat(point3File,P3D_FILE);
    line2File = strcpy(line2File,TEST_DIR); line2File = strcat(line2File,L2D_FILE);
    line3aFile = strcpy(line3aFile,TEST_DIR); line3aFile = strcat(line3aFile,L3DA_FILE);
    line3bFile = strcpy(line3bFile,TEST_DIR); line3bFile = strcat(line3bFile,L3DB_FILE);

    get_target_vector_vector(&p2d,8);
    get_target_vector_vector(&p3d,8);
    get_target_vector_vector(&l2d,12);
    get_target_vector_vector(&l3da,12);
    get_target_vector_vector(&l3db,12);

    printf("Using data in directories \"%s\"\n\n",TEST_DIR);
    printf("-----\nNumber of tests: %d\n-----\n\n",numTests);

    for (i = 0; i < numTests; i++) {
        runTest(i);
    }

    free_corrs();
    printf("Number of average errors greater than 2.0: %d\n",tests_failed);
    
    if (tests_failed == 0) 
    {
        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_BUG;
    }

}

void runTest(int test_id) {
    
    Matrix* temp=0;
    int numPts,numLines,i,j;
    Vector_vector* p2dt=0;
    Vector_vector* p3dt=0;
    Vector_vector* l2dt=0;
    Vector_vector* l3dat=0;
    Vector_vector* l3dbt=0;

    camFile[IDX] = '0'+test_id;
    point2File[IDX] = '0'+test_id;
    point3File[IDX] = '0'+test_id;
    line2File[IDX] = '0'+test_id;
    line3aFile[IDX] = '0'+test_id;
    line3bFile[IDX] = '0'+test_id;

    read_matrix(&p_proj,camFile);
    read_matrix(&temp,point2File);
    vector_vector_from_matrix(&p2dt,temp);
    read_matrix(&temp,point3File);
    vector_vector_from_matrix(&p3dt,temp);
    read_matrix(&temp,line2File);
    vector_vector_from_matrix(&l2dt,temp);
    read_matrix(&temp,line3aFile);
    vector_vector_from_matrix(&l3dat,temp);
    read_matrix(&temp,line3bFile);
    vector_vector_from_matrix(&l3dbt,temp);

    free_matrix(temp);

    copy_vector(&(p2d->elements[0]),p2dt->elements[6]);
    copy_vector(&(p3d->elements[0]),p3dt->elements[6]);
    copy_vector(&(p2d->elements[1]),p2dt->elements[0]);
    copy_vector(&(p3d->elements[1]),p3dt->elements[0]);
    copy_vector(&(p2d->elements[2]),p2dt->elements[4]);
    copy_vector(&(p3d->elements[2]),p3dt->elements[4]);
    copy_vector(&(p2d->elements[3]),p2dt->elements[1]);
    copy_vector(&(p3d->elements[3]),p3dt->elements[1]);
    copy_vector(&(p2d->elements[4]),p2dt->elements[5]);
    copy_vector(&(p3d->elements[4]),p3dt->elements[5]);
    copy_vector(&(p2d->elements[5]),p2dt->elements[7]);
    copy_vector(&(p3d->elements[5]),p3dt->elements[7]);
    copy_vector(&(p2d->elements[6]),p2dt->elements[2]);
    copy_vector(&(p3d->elements[6]),p3dt->elements[2]);
    copy_vector(&(p2d->elements[7]),p2dt->elements[3]);
    copy_vector(&(p3d->elements[7]),p3dt->elements[3]);

    copy_vector(&(l2d->elements[0]),l2dt->elements[6]);
    copy_vector(&(l3da->elements[0]),l3dat->elements[6]);
    copy_vector(&(l3db->elements[0]),l3dbt->elements[6]);
    copy_vector(&(l2d-> elements[1]),l2dt-> elements[3]);
    copy_vector(&(l3da->elements[1]),l3dat->elements[3]);
    copy_vector(&(l3db->elements[1]),l3dbt->elements[3]);
    copy_vector(&(l2d-> elements[2]),l2dt-> elements[9]);
    copy_vector(&(l3da->elements[2]),l3dat->elements[9]);
    copy_vector(&(l3db->elements[2]),l3dbt->elements[9]);
    copy_vector(&(l2d-> elements[3]),l2dt-> elements[4]);
    copy_vector(&(l3da->elements[3]),l3dat->elements[4]);
    copy_vector(&(l3db->elements[3]),l3dbt->elements[4]);
    copy_vector(&(l2d-> elements[4]),l2dt-> elements[11]);
    copy_vector(&(l3da->elements[4]),l3dat->elements[11]);
    copy_vector(&(l3db->elements[4]),l3dbt->elements[11]);
    copy_vector(&(l2d-> elements[5]),l2dt-> elements[2]);
    copy_vector(&(l3da->elements[5]),l3dat->elements[2]);
    copy_vector(&(l3db->elements[5]),l3dbt->elements[2]);
    copy_vector(&(l2d-> elements[6]),l2dt-> elements[7]);
    copy_vector(&(l3da->elements[6]),l3dat->elements[7]);
    copy_vector(&(l3db->elements[6]),l3dbt->elements[7]);
    copy_vector(&(l2d-> elements[7]),l2dt-> elements[5]);
    copy_vector(&(l3da->elements[7]),l3dat->elements[5]);
    copy_vector(&(l3db->elements[7]),l3dbt->elements[5]);
    copy_vector(&(l2d-> elements[8]),l2dt-> elements[8]);
    copy_vector(&(l3da->elements[8]),l3dat->elements[8]);
    copy_vector(&(l3db->elements[8]),l3dbt->elements[8]);
    copy_vector(&(l2d-> elements[9]),l2dt-> elements[10]);
    copy_vector(&(l3da->elements[9]),l3dat->elements[10]);
    copy_vector(&(l3db->elements[9]),l3dbt->elements[10]);
    copy_vector(&(l2d-> elements[10]),l2dt-> elements[0]);
    copy_vector(&(l3da->elements[10]),l3dat->elements[0]);
    copy_vector(&(l3db->elements[10]),l3dbt->elements[0]);
    copy_vector(&(l2d-> elements[11]),l2dt-> elements[1]);
    copy_vector(&(l3da->elements[11]),l3dat->elements[1]);
    copy_vector(&(l3db->elements[11]),l3dbt->elements[1]);

    free_vector_vector(p2dt);
    free_vector_vector(p3dt);
    free_vector_vector(l2dt);
    free_vector_vector(l3dat);
    free_vector_vector(l3dbt);

    printf("Test %d:\n\n",test_id);

    for (numPts=pMin; numPts<=pMax; numPts++)
    {
        for (numLines=lMin; numLines<=lMax; numLines++)
        {
            if (numPts+numLines>=6)
            {

                printf("Original values--\n");
                for (i=0;i<3;i++) {
                    for (j=0;j<4;j++) {
                        printf("\t%f",p_proj->elements[i][j]);
                    }
                    printf("\n");
                }
                printf("2D Points--\n");
                for (i=0;i<p2d->length;i++) {
                    for (j=0;j<p2d->elements[i]->length;j++) {
                        printf("\t%f",p2d->elements[i]->elements[j]);
                    }
                    printf("\n");
                }
                printf("2D Lines--\n");
                for (i=0;i<l2d->length;i++) {
                    for (j=0;j<l2d->elements[i]->length;j++) {
                        printf("\t%f",l2d->elements[i]->elements[j]);
                    }
                    printf("\n");
                }

                get_camera_matrix_from_point_and_line_correspondences(&new_proj,p2d,p3d,numPts,l2d,l3da,l3db,numLines);
                
                printf("Derived values using %d points and %d lines--\n",numPts,numLines);
                for (i=0;i<3;i++) {
                    for (j=0;j<4;j++) {
                        printf("\t%f",new_proj->elements[i][j]);
                    }
                    printf("\n");
                }
                totErr = 0;
                ptErr = 0;
                lnErr = 0;
                printf("Projected 2D points from result--\n");
                for (i=0; i<numPts; i++) {
                    multiply_matrix_and_vector(&plot,new_proj,p3d->elements[i]);
                    ow_multiply_vector_by_scalar(plot,p2d->elements[i]->elements[2]/plot->elements[2]);
                    for (j=0;j<plot->length;j++) {
                        printf("\t%f",plot->elements[j]);
                    }
                    printf("\n");
                    ptErr += error_check_vectors(plot,p2d->elements[i],2);
                }
                printf("Projected 2D lines from result--\n");
                line_projection_matrix_from_point_project_matrix(&l_proj,new_proj);
                for (i=0; i<numLines; i++) {
                    plucker_line_from_points(&plucker,l3da->elements[i],l3db->elements[i]);
                    multiply_matrix_and_vector(&plot,l_proj,plucker);
                    ow_multiply_vector_by_scalar(plot,l2d->elements[i]->elements[1]/plot->elements[1]);
                    for (j=0;j<plot->length;j++) {
                        printf("\t%f",plot->elements[j]);
                    }
                    printf("\n");
                    lnErr += error_check_vectors(plot,l2d->elements[i],1);
                }
                totErr = ptErr+lnErr;
                ptErr=(numPts>0)?(ptErr/numPts):0;
                lnErr=(numLines>0)?(lnErr/numLines):0;
                printf("Total error: %f\n",totErr);
                printf("Points mean error: %f\n",ptErr);
                printf("Lines mean error: %f\n",lnErr);
                totErr/=(numPts+numLines);
                printf("Total mean error: %f\t",totErr);
                PASS_OR_FAIL(totErr<2.0);
            }
        }
    }
    printf("\n\n");
}

void free_corrs()
{
    free_vector_vector(p2d);
    free_vector_vector(p3d);
    free_vector_vector(l2d);
    free_vector_vector(l3da);
    free_vector_vector(l3db);
    free_matrix(new_proj);
    free_matrix(p_proj);
    free_vector(plucker);
    free_vector(plot);
    free_matrix(l_proj);
    free(camFile);
    free(line2File);
    free(line3aFile);
    free(line3bFile);
}

void PASS_OR_FAIL(int pof)
{
    if (pof)
    {
        printf("Pass\n");
    } else {
        printf("FAIL!\n");
        tests_failed++;
    }
}

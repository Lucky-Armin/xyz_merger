#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char *argv[])
{

  char *output_filename = "merged_trajectory.xyz";
  char *input_filename1 = argv[1];
  char *input_filename2 = argv[2];

  if (argc != 3)
  {
    printf("Usage: %s  trajectory1.xyz  trajectory2.xyz\nMerges two trajectories by appending the coordinates in the second file frame by frame to the 1st trajectory.\nWrites output to a file named \"%s\".\n", argv[0], output_filename);
    return 1;
  }

  FILE *fileptr1 = fopen(output_filename, "r");
  FILE *input_file1 = fopen(input_filename1, "r");
  FILE *input_file2 = fopen(input_filename2, "r");

  if (fileptr1 != NULL)
  {
    printf("File \"%s\" already exists!\n*******************************************\n****************  EXITING  ****************\n*******************************************\n", output_filename);
    return 1;
  }
  if (input_file1 == NULL || input_file2 == NULL)
  {
    printf("Error: unable to open input file(s) of type *.xyz\n*******************************************\n****************  EXITING  ****************\n*******************************************\n");
    return 1;
  }

  FILE *output_file = fopen(output_filename, "w");

  int i = 0;
  int j = 0;
  enum
  {
    X,
    Y,
    Z
  };

  char line1[256];
  char line2[256];
  int NATOMS1 = 0;
  int NATOMS2 = 0;
  int NATOMS_new = 0;
  char atom1[3];
  char atom2[3];
  double line_count = 1;
  int number_of_frames = 0;

  // determine the number of frames to be merged from the first input file
  fgets(line1, sizeof(line1), input_file1);
  sscanf(line1, "%d", &NATOMS1);
  while (fgets(line1, sizeof(line1), input_file1) != NULL)
  {
    line_count++;
  }

  number_of_frames = round(line_count / (NATOMS1 + 2));

  fclose(input_file1);
  input_file1 = fopen(input_filename1, "r");

  for (j = 0; j < number_of_frames; j++)
  {

    // reading of the 1st line
    fgets(line1, sizeof(line1), input_file1);
    fgets(line2, sizeof(line2), input_file2);

    sscanf(line1, "%d", &NATOMS1);
    sscanf(line2, "%d", &NATOMS2);

    NATOMS_new = NATOMS1 + NATOMS2;

    double **CRD1;   // Array for saving the coordinates of all NATOMS in X Y and Z for atoms from file 1
    double **CRD2;   // Array for saving the coordinates of all NATOMS in X Y and Z for atoms from file 2
    char **ELEMENT1; // Array for saving the element type for each line from file 1
    char **ELEMENT2; // Array for saving the element type for each line from file 2

    CRD1 = (double **)calloc(NATOMS_new, sizeof(double *));
    CRD2 = (double **)calloc(NATOMS_new, sizeof(double *));
    ELEMENT1 = (char **)calloc(NATOMS_new, sizeof(char *));
    ELEMENT2 = (char **)calloc(NATOMS_new, sizeof(char *));

    for (i = 0; i < NATOMS_new; ++i)
    {
      CRD1[i] = (double *)calloc(3, sizeof(double));
      CRD2[i] = (double *)calloc(3, sizeof(double));
      ELEMENT1[i] = (char *)calloc(3, sizeof(char));
      ELEMENT2[i] = (char *)calloc(3, sizeof(char));
    }

    // reading of the 2nd line
    fgets(line1, sizeof(line1), input_file1);
    fgets(line2, sizeof(line2), input_file2);

    // reading the coordinates line by line for each input file and storing them in the repsective 2D array
    for (i = 0; i < NATOMS1; i++)
    {
      fgets(line1, sizeof(line1), input_file1);
      sscanf(line1, "%s %lf %lf %lf", atom1, &CRD1[i][X], &CRD1[i][Y], &CRD1[i][Z]);
      strcpy(ELEMENT1[i], atom1);
    }
    for (i = 0; i < NATOMS2; i++)
    {
      fgets(line2, sizeof(line2), input_file2);
      sscanf(line2, "%s %lf %lf %lf", atom2, &CRD2[i][X], &CRD2[i][Y], &CRD2[i][Z]);
      strcpy(ELEMENT2[i], atom2);
    }

    // writing to the new merged trajectory file
    fprintf(output_file, "%d\n", NATOMS_new);
    fprintf(output_file, "Jakob Gamper is best <3\n");

    // writing the coordinates from the first input file to the output
    for (i = 0; i < NATOMS1; i++)
    {
      fprintf(output_file, "%-3s   %13.9lf   %13.9lf   %13.9lf\n", ELEMENT1[i], CRD1[i][X], CRD1[i][Y], CRD1[i][Z]);
    }
    // writing the coordinates from the second input file to the output
    for (i = 0; i < NATOMS2; i++)
    {
      fprintf(output_file, "%s   %13.9lf   %13.9lf   %13.9lf\n", ELEMENT2[i], CRD2[i][X], CRD2[i][Y], CRD2[i][Z]);
    }

    free(CRD1);
    free(CRD2);
    free(ELEMENT1);
    free(ELEMENT2);
  }

  fclose(input_file1);
  fclose(input_file2);
  fclose(output_file);
  printf("Trajectories merged successfully!\n");
  return 0;
}

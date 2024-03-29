#include "precice/SolverInterfaceC.h"

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  double  dt                 = 0.0;
  int     dimensions         = -1;
  double *vertices;
  double *readData;
  double *writeData;
  int     meshID = -1;
  int     dataID = -1;
  int *   vertexIDs;
  int     numberOfVertices = 3;
  int     writeDataID      = -1;
  int     readDataID       = -1;


  MPI_Init( NULL, NULL );

  int commRank = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

  int commSize = -1;
  MPI_Comm_size(MPI_COMM_WORLD, &commSize);

  if (argc != 3) {
    printf("Usage: ./solverdummy configFile solverName meshName\n\n");
    printf("Parameter description\n");
    printf("  configurationFile: Path and filename of preCICE configuration\n");
    printf("  solverName:        SolverDummy participant name in preCICE configuration\n");
    return 1;
  }

  const char *configFileName  = argv[1];
  const char *participantName = argv[2];

  const char *meshName = (strcmp(participantName, "SolverOne") == 0) ? "MeshOne" : "MeshTwo";

  printf("DUMMY (%d): Running solver dummy with preCICE config file \"%s\" and participant name \"%s\".\n",
        commRank, configFileName, participantName);


  const char *writeItCheckp = precicec_actionWriteIterationCheckpoint();
  const char *readItCheckp  = precicec_actionReadIterationCheckpoint();

  precicec_createSolverInterface(participantName, configFileName, commRank, commSize);

  meshID = precicec_getMeshID(meshName);

  if (strcmp(participantName, "SolverOne") == 0) {
    writeDataID = precicec_getDataID("dataOne", meshID);
    readDataID  = precicec_getDataID("dataTwo", meshID);
  }
  if (strcmp(participantName, "SolverTwo") == 0) {
    writeDataID = precicec_getDataID("dataTwo", meshID);
    readDataID  = precicec_getDataID("dataOne", meshID);
  }

  dimensions = precicec_getDimensions();
  vertices   = malloc(numberOfVertices * dimensions * sizeof(double));
  readData   = malloc(numberOfVertices * dimensions * sizeof(double));
  writeData  = malloc(numberOfVertices * dimensions * sizeof(double));
  vertexIDs  = malloc(numberOfVertices * sizeof(int));

  for (int i = 0; i < numberOfVertices; i++) {
    for (int j = 0; j < dimensions; j++) {
      vertices[j + dimensions * i]  = i;
      readData[j + dimensions * i]  = i;
      writeData[j + dimensions * i] = i;
    }
  }

  precicec_setMeshVertices(meshID, numberOfVertices, vertices, vertexIDs);

  free(vertices);

  dt = precicec_initialize();

  while (precicec_isCouplingOngoing()) {

    if (precicec_isActionRequired(writeItCheckp)) {
      printf("DUMMY (%d): Writing iteration checkpoint \n", commRank);
      precicec_markActionFulfilled(writeItCheckp);
    }

    if (precicec_isReadDataAvailable) {
      precicec_readBlockVectorData(readDataID, numberOfVertices, vertexIDs, readData);
    }

    for (int i = 0; i < numberOfVertices * dimensions; i++) {
      writeData[i] = readData[i] + 1;
    }

    if (precicec_isWriteDataRequired(dt)) {
      precicec_writeBlockVectorData(writeDataID, numberOfVertices, vertexIDs, writeData);
    }

    dt = precicec_advance(dt);

    if (precicec_isActionRequired(readItCheckp)) {
      printf("DUMMY (%d): Reading iteration checkpoint \n", commRank);
      precicec_markActionFulfilled(readItCheckp);
    } else {
      printf("DUMMY (%d): Advancing in time \n", commRank);
    }
  }

  precicec_finalize();
  free(writeData);
  free(readData);
  free(vertexIDs);
  printf("DUMMY (%d): Closing C solver dummy... \n", commRank);

  MPI_Finalize();

  return 0;
}

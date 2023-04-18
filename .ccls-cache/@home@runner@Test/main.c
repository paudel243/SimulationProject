/* Online C Compiler and Editor */
// To Do:
// Testing for bugs
// Constructing loops to simulate question 6 and 7
// TAG, Set, Word Fields main memory address (Lowest Prio)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int hitRate = 0;
int missRate = 0;
int numofdataaccesses=0;
#define STRINGLENGTH 99

struct DataToProcess 
{
  char dataName[STRINGLENGTH];
};
struct CacheBlock 
{
  char dataInBlock[STRINGLENGTH];
  int timeStamp; // this will be used to track the age of the process and at
                 // what time a block was used
};
struct LoopStructure 
{
  int startOfLoop;
  int endOfLoop;
  int amountOfLoops;
};

int CacheFull(struct CacheBlock blockArray[], int totalBlocks) 
{
  int check = -1; // initialize to -1
  for (int i = 0; i < totalBlocks; i++) 
  {
    if (strlen(blockArray[i].dataInBlock) == 0) 
    {
      check = i; // if the function finds the first empty block
      break;
    }
  }
  return check; // if everything is full then check will still be -1 otherwise
                // it will return the first empty block it finds
}

int ReplaceAlgo(struct CacheBlock blockArray[], int totalBlocks) 
{
  int blockToReplace, isCacheFull; // added the oldestBlock variable for LRU
  isCacheFull = CacheFull(blockArray, totalBlocks); // Check if Cache is full
  if (isCacheFull == -1) 
  {                          // if cache is full run LRU
    int lruBlock = 0;                               // for LRU
    for (int i = 1; i < totalBlocks; i++) 
    {
      if (blockArray[i].timeStamp < blockArray[lruBlock].timeStamp) 
      {
        lruBlock = i;
      }
    }
    blockToReplace = lruBlock;
  } 
  else 
  { // IF CACHE ISNT FULL
    blockToReplace =isCacheFull; // Since CacheFull already contains the first empty
  }
  return blockToReplace; // returns the block that can be replaced
}

int CheckCache(struct CacheBlock blockArray[], char dataName[99],int numOfBlocks) 
{
  int blockNum = -1; // Initialize to -1 until a hit is identified if it was
                     // never identified in the cache then -1 means miss
                     // for LRU
  for (int i = 0; i < numOfBlocks; i++) 
  {
    // New for LRU
    if (strcmp(blockArray[i].dataInBlock, dataName) == 0) 
    {
      blockNum = i;
      break; // IF It's a hit
    }
  }
  return blockNum; // returns if it's a hit or not
}

void FullAssociative(struct CacheBlock blockArray[],struct DataToProcess dataArray[], int numOfBlocks,int start, int numOfDataToProcess) 
{
  int isDataInCache, blockToReplace;
  for (int i = start; i < numOfDataToProcess;i++) // Loop for datasequence to process
  {
    isDataInCache = 0;
    char processData[STRINGLENGTH];
    strcpy(processData, dataArray[i].dataName);
    isDataInCache = CheckCache(blockArray, processData,numOfBlocks); // checks if the data is in the cache
    if (isDataInCache >= 0)      // if it's a hit
    {
      hitRate += 1;                            // hitRate ++
      blockArray[isDataInCache].timeStamp = numofdataaccesses; // update timeStamp
    } 
    else if (isDataInCache == -1)            // if it's a miss
    {
      blockToReplace = ReplaceAlgo(blockArray, numOfBlocks); // Replacement Algo Function returns the
                                    // block that will be replaced in the cache
      strcpy(blockArray[blockToReplace].dataInBlock,dataArray[i].dataName); // placing the data to process in the cache
      blockArray[blockToReplace].timeStamp = numofdataaccesses; // update timestamp
      missRate += 1;                            // miss rate + 1
    }
    numofdataaccesses+=1;
  }
}

void LoopFullAssociative(struct CacheBlock cache[], struct DataToProcess dataBlocks[],int numOfCB, struct LoopStructure loopS[],int current)
{
	for(int i=0;i<loopS[current].amountOfLoops;i++)
    {
    	FullAssociative(cache, dataBlocks, numOfCB, loopS[current].startOfLoop,loopS[current].endOfLoop);
	}

}

void NestedLoops(struct CacheBlock blockArray[],struct DataToProcess blocksToProcess[],struct LoopStructure loops[], int numOfDataToProcess,int numOfLoops,int numOfCacheBlocks,int currentLoop) 
{
  int programDone = 0;
  if(currentLoop==0&&loops[currentLoop].startOfLoop>0)
  {
    FullAssociative(blockArray, blocksToProcess, numOfCacheBlocks, 0, loops[currentLoop].startOfLoop); // PROCESSES THE DATA BEFORE ANY OF THE LOOPS START
    //programDone=1;
  }
  while (programDone == 0) 
  {
    if(currentLoop==numOfLoops-1)// if there are no more loops inside of the current loop
    {
      LoopFullAssociative(blockArray,blocksToProcess,numOfCacheBlocks,loops,currentLoop);
      programDone=1;
    }
    else if(currentLoop < numOfLoops-1) //if theres still a loop inside of the current loop
    {
    	for(int i = 0; i<loops[currentLoop].amountOfLoops;i++)
		{
			FullAssociative(blockArray,blocksToProcess,numOfCacheBlocks,loops[currentLoop].startOfLoop,loops[currentLoop+1].startOfLoop);
			LoopFullAssociative(blockArray,blocksToProcess,numOfCacheBlocks,loops,currentLoop+1);
			FullAssociative(blockArray,blocksToProcess,numOfCacheBlocks,loops[currentLoop+1].endOfLoop,loops[currentLoop].endOfLoop);
		}
		programDone=1;
    }     
  }
  if(currentLoop==0&&loops[0].endOfLoop < numOfDataToProcess)
  {
    FullAssociative(blockArray, blocksToProcess, numOfCacheBlocks, loops[0].endOfLoop, numOfDataToProcess); //PROCESSES THE REST OF THE DATA OUTSIDE THE LOOPS
  }
  
}

void InputDataSequence(struct DataToProcess block[], int size,int sequenceType) 
{
  if(sequenceType == 0)
  {
    for (int i = 0; i < size; i++) 
    {
      printf("Input Hex Address \n");
      scanf("%s", block[i].dataName);
    }
  }
  if (sequenceType == 1) 
  {
    
    for (int i = 0; i < size; i++) 
    {
      printf("Input Data Block \n");
      scanf("%s", block[i].dataName);
    }
  } 
  else if (sequenceType == 2) 
  {
    for (int i = 0; i < size; i++) 
    {
      memset(block[i].dataName,0,STRINGLENGTH);
      sprintf(block[i].dataName, "DB:%d",i); // placing the blockNames into an array for processing
      printf("%s\n",block[i].dataName);
    }
  }
}

int SequenceTypeInput() 
{
  int input, i = 0;
  while (i == 0) 
  {
    printf("What types of inputs will be accepted\n");
    printf("(0): Addresses\n");
    printf("(1): Blocks\n");
    printf("(2): Words\n");
    scanf("%d", &input);
    if (input == 0)
    {
      	i = 1;
      	printf("Cache Memory will be in Bytes\n");
      	printf("Main Memory should be 0\n");
      	printf("Block Size will be in Bytes\n");
      	printf("Number of Blocks in the cache will be calculated based on Cache Memory and Block Size \n");
    }
    if (input == 1) 
    {
		i = 1;
      	printf("Cache Memory will be calculated based on total number of blocks in the cache * block size\n");
      	printf("Main Memory Size will be accepted as blocks\n");
      	printf("Block Size will be in words\n");
      	printf("Number of Blocks will be asked \n");
    }
	if (input == 2) 
    {
      	i = 1;
      	printf("Cache Memory will be asked as words\n");
      	printf("Main Memory Size will be accepted as words\n");
      	printf("Block Size will be in words\n");
      	printf("Number of Blocks will be calculated based on CacheMemory/BlockSize \n");
    }  
    else if(i==0)
    {
      printf("Invalid Input Please Try Again \n");
    }
  }
  return input;
}

int InputProgramLoop() {
  int input, i = 0;
  while (i == 0) {
    printf("Does the program have any loops Yes(1) or No(0) \n");
    scanf("%d", &input);
    if (input == 0) {
      i = 1;
    }
    if (input == 1) {
      i = 1;
    } else {
      printf("Invalid Input Please Try Again \n");
    }
  }
  return input;
}



void LoopInputs(struct LoopStructure loopStructures[], int blockMemorySize,int numOfLoops,int blockSize)
{
  for (int i = 0; i < numOfLoops; i++) 
  {
    printf("Where does Loop # %d start\n", i);
    scanf("%d", &loopStructures[i].startOfLoop);
    loopStructures[i].startOfLoop =(loopStructures[i].startOfLoop + 1) / blockSize; // convert the words being looped into blocks
    printf("Test Start Loop: %d\n",loopStructures[i].startOfLoop);
    printf("Where does Loop # %d end\n", i);
    scanf("%d", &loopStructures[i].endOfLoop);
    loopStructures[i].endOfLoop = (loopStructures[i].endOfLoop + 1) /  blockSize; // convert the words being looped into blocks
    printf("Test End Loop: %d\n", loopStructures[i].endOfLoop);
    printf("How many times will this loop loop?\n");
    scanf("%d", &loopStructures[i].amountOfLoops);
  }
}




int returnBits(int convert)
{
	int bits = (int) log2(convert);
	return bits;
}
// Print Output//
void outputProgram(int numOfData, int blockSize,int cacheAccessTime,int memAccessTime,int numOfBlocks,int memSize,struct CacheBlock cacheBlocks[],const char *file)
{
	int tagBits=0,blockBits=0,wordBits=0,memBits=0;
	float hitRatePercentage, missRatePercentage, missPenalty, aveAccessTime,totalAccessTime;
	printf("%d\n",numofdataaccesses);
	hitRatePercentage = (float)hitRate / (float)numofdataaccesses;
	printf("Cache hits: %d\n", hitRate);
	printf("Hit Rate Percentage: %f\n",hitRatePercentage);
	missRatePercentage = (float)missRate / (float)numofdataaccesses;
	printf("Cache misses: %d\n", missRate);
  	printf("Miss Rate Percentage: %f\n",missRatePercentage);
	missPenalty =cacheAccessTime+ blockSize * memAccessTime + cacheAccessTime;
	printf("Miss penalty: %f\n", missPenalty);
	aveAccessTime = hitRatePercentage*cacheAccessTime + missRatePercentage*missPenalty;
	printf("Average memory access time: %.2f\n", aveAccessTime);
	totalAccessTime = hitRate*blockSize*cacheAccessTime + missRate*blockSize*(memAccessTime + 1)+ missRate*cacheAccessTime;
	printf("Total memory access time: %.2f\n", totalAccessTime);
	
	memBits=returnBits(memSize);
	if(memBits>0)
	{
		blockBits=returnBits(numOfBlocks);
		wordBits=returnBits(blockSize);
		printf("Total Num of Memory Bits: %d\n",memBits);
		printf("TAG bits on FA: %d\n", memBits - wordBits);
		printf("TAG bits including blockbits: %d\n", memBits-blockBits-wordBits);
		printf("Block Bits (disregard if not needed): %d\n", blockBits);
		printf("Word Bits: %d\n",wordBits);	
	}
	printf("\n");
	for(int i =0; i< numOfBlocks; i++)
    {
      printf("Block %d",i);
      printf(" %s\n",cacheBlocks[i].dataInBlock);
	}
	int writeToFile;
	printf("Write to text file: output.txt? Yes(1) or No(0) \n");
	scanf("%d",&writeToFile);
	if(writeToFile==1)
	{
		FILE *f = fopen(file,"w");
		if(f==NULL)
		{
			printf("Error: Could not open file\n");
			
		}
		fprintf(f,"%s %d\n","Total Data Accessed:",numofdataaccesses);
		fprintf(f,"%s %d\n","Cache hits:",hitRate);
		fprintf(f,"%s %f\n","Hit Rate Percentage:",hitRatePercentage);
		fprintf(f,"%s %d\n","Cache misses:",missRate);
		fprintf(f,"%s %f\n","Miss Rate Percentage:",missRatePercentage);
		fprintf(f,"%s %f\n","Miss penalty: ", missPenalty);
		fprintf(f,"%s %.2f\n","Average Memory Access Time: ", aveAccessTime);
		fprintf(f,"%s %.2f\n","Total Memory Access Time: ", totalAccessTime);
		for(int i =0; i< numOfBlocks; i++)
    	{
      		fprintf(f,"%s %d, %s\n","Block: ",i, cacheBlocks[i].dataInBlock);
		}
	}
    
}

void InitializeBlocks(struct CacheBlock blocks[],int numOfBlocks)
{
  for(int i = 0;i<numOfBlocks;i++)
    {
      memset(blocks[i].dataInBlock,0,STRINGLENGTH); //blank block
      blocks[i].timeStamp=0; // just initializing to avoid garbage values
    }
}



void InitializeDataBlocks(struct DataToProcess db[],int sType,int total)
{

	if(sType==0)
	{
    for (int i = 0; i < total; i++) 
    {
      	printf("Input Hex Address: \n");
      	scanf("%s", db[i].dataName);
    }
	}
	else if(sType==1)
	{
		for (int i = 0; i < total; i++) 
    	{
      		printf("Input Data Block \n");
      		scanf("%s", db[i].dataName);
    	}
	}
	else if (sType==2)
	{
		for (int i = 0; i < total; i++) 
   		{	
   			memset(db[i].dataName,0,STRINGLENGTH);
			sprintf(db[i].dataName, "DB:%d",i); // placing the blockNames into an array for processing
    	}
	}
}
int InputWordAmount(int blockSize) 
{
	int input;
  	printf("Input amount of words to process \n");
  	printf("Note: it is assumed that the size of main memory, cache memory and block memory size are in the unit words\n");
  	scanf("%d", &input);
  	printf("Words that will be processed are from locations 0 to %d\n",(input-1));
  	input = input / blockSize; // convert the amount of data to process in blocks
  	printf("Number of Data Blocks: %d \n", input);                           // since we copy from main memory block per block.
  	return input;
}


int main() 
{
	int cacheMemorySize, mainMemorySize, blockMemorySize, data, sequenceType,cacheAccessTime,memAccessTime,doesProgramLoop=0, numOfLoops,numberOfBlocks;
  	sequenceType = SequenceTypeInput();
  	printf("Input Cache Access Time: \n");
  	scanf("%d", &cacheAccessTime);
  	printf("Input Memory Access Time: \n");
  	scanf("%d", &memAccessTime);
	printf("Cache Memory Size \n");
  	scanf("%d", &cacheMemorySize);
  	printf("Main Memory Size \n");
  	scanf("%d", &mainMemorySize);
  	printf("Block Size: \n");
  	scanf("%d", &blockMemorySize);
  	if(sequenceType==0)
	{
	  	numberOfBlocks = cacheMemorySize/blockMemorySize;
	  	printf("How many Addresses are going to be processed \n");
	  	scanf("%d",&data);
	}
  	else if(sequenceType==1)
	{
		printf("How Many Blocks are there in the cache \n");
  		scanf("%d",&numberOfBlocks); 
  		cacheMemorySize= numberOfBlocks*blockMemorySize;
  		printf("CMS: %d\n",cacheMemorySize);
  		printf("Input the size of data blocks you'd like to process \n");
    	scanf("%d", &data);
	}
	else if(sequenceType==2)
  	{
  		numberOfBlocks = cacheMemorySize/blockMemorySize;
  		data = InputWordAmount(blockMemorySize);
    	doesProgramLoop = InputProgramLoop();	
 	}
  	const int numOfData = data;
	  struct DataToProcess dataBlocks[numOfData];
    printf("Sequence Type %d\n",sequenceType);
    InputDataSequence(dataBlocks, numOfData, sequenceType);
	  const int numOfCacheBlocks = numberOfBlocks;
  	struct CacheBlock cacheBlocks[numOfCacheBlocks];
  	InitializeBlocks(cacheBlocks,numOfCacheBlocks);
  	if(doesProgramLoop==1)
  	{
  	  printf("How many nested loops are located in the program \n");
      scanf("%d", &numOfLoops);
      printf("The program is designed to only handle the specs in the problem set question number 6 and 7\n");
	  const int loopArraySize = numOfLoops;
      struct LoopStructure loopStructures[loopArraySize];
      LoopInputs(loopStructures, blockMemorySize, loopArraySize,blockMemorySize);
      NestedLoops(cacheBlocks, dataBlocks, loopStructures, numOfData, numOfLoops, numOfCacheBlocks, 0);
	}
	else if(doesProgramLoop==0&&sequenceType==1)
	{
		FullAssociative(cacheBlocks,dataBlocks,numOfCacheBlocks,0,numOfData);
	}
  else if(doesProgramLoop==0&&sequenceType==0)
  {
      FullAssociative(cacheBlocks, dataBlocks, numOfCacheBlocks, 0,numOfData);
  }
  	const char *filename = "output.txt";
  	outputProgram(data,blockMemorySize,cacheAccessTime,memAccessTime,numOfCacheBlocks,mainMemorySize,cacheBlocks,filename);
  	return 0;
}


// End of Program// 
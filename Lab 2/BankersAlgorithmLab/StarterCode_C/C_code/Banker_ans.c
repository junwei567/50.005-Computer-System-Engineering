/**
 * CSE lab project 2 -- C version
 * 
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numberOfCustomers = 0; // the number of customers
int numberOfResources = 0; // the number of resources

int *available;	  // the available amount of each resource
int **maximum;	  // the maximum demand of each customer
int **allocation; // the amount currently allocated
int **need;		  // the remaining needs of each customer

int BUDGE = 0;


// Utility function to allocate an int vector.
int *mallocIntVector(int size)
{
	int i, *a = malloc(sizeof(int) * size);
	for (i = 0; i < size; i++)
		a[i] = 0;
	return a;
}

// Utility function to free an int vector.
void freeIntVector(int *a)
{
	free(a);
}

// Utility function to allocate an int matrix.
int **mallocIntMatrix(int numRows, int numColumns)
{
	int i, j, **a = malloc(sizeof(int *) * (numRows + 1));
	for (i = 0; i < numRows; i++)
	{
		a[i] = malloc(sizeof(int) * numColumns);
		for (j = 0; j < numColumns; j++)
			a[i][j] = 0;
	}
	a[numRows] = 0;
	return a;
}

// Utility function to free an int matrix.
void freeIntMatrix(int **a)
{
	int i;
	for (i = 0; a[i] != 0; i++)
		free(a[i]);
	free(a);
}

/**
 * Initializes the state of the bank.
 * @param resources  An array of the available count for each resource.
 * @param m          The number of resources.
 * @param n          The number of customers.
 */
void initBank(int *resources, int m, int n)
{
	// Allocate memory for the vector and matrices
	available = mallocIntVector(m);
	need = mallocIntMatrix(n, m);
	allocation = mallocIntMatrix(n, m);
	maximum = mallocIntMatrix(n, m);

	// TODO: initialize the numberOfCustomers and numberOfResources
	numberOfCustomers = n;
	numberOfResources = m;

	// TODO: initialize the available vector
	for (int i = 0; i < m; i++) {
		available[i] = resources[i];
	}
}

/**
 * Frees the memory used to store the state of the bank.
 */
void freeBank()
{
	freeIntVector(available);
	freeIntMatrix(need);
	freeIntMatrix(allocation);
	free(maximum);
}

/**
 * Prints the state of the bank.
 */
void printState()
{
	int i;
	int j;
	// TODO: print the current state with a tidy format
	printf("\nCurrent state:\n");

	// TODO: print available
	printf("Available:\n");
	for (i = 0; i < numberOfResources; i++)
	{
		printf("%d ", available[i]); // leave a space between each print
	}
	printf("\n"); // Leave a line after printing available

	// TODO: print maximum
	printf("Maximum:\n");
	for (i = 0; i < numberOfCustomers; i++)
	{
		for (j = 0; j < numberOfResources; j++)
		{
			printf("%d ", maximum[i][j]); // Leave a space between each print
		}
		printf("\n"); // Leave a line after each customer
	}
	printf("\n"); // Leave a line after printing maximum

	// TODO: print allocation
	printf("Allocation:\n");
	for (i = 0; i < numberOfCustomers; i++)
	{
		for (j = 0; j < numberOfResources; j++)
		{
			printf("%d ", allocation[i][j]); // Leave a space between each print
		}
		printf("\n"); // Leave a line after each customer
	}
	printf("\n"); // Leave a line after printing Allcoation

	// TODO: print need
	printf("Need:\n");
	for (i = 0; i < numberOfCustomers; i++)
	{
		for (j = 0; j < numberOfResources; j++)
		{
			printf("%d ", need[i][j]); // Leave a space bettern each print
		}
		printf("\n"); // Leave a line after each customer
	}
	printf("\n"); // Leave a line after printing need
}

/**
 * Sets the maximum number of demand of each resource for a customer.
 * @param customerIndex  The customer's index (0-indexed).
 * @param maximumDemand  An array of the maximum demanded count for each resource.
 */
void setMaximumDemand(int customerIndex, int *maximumDemand)
{
	// TODO: add customer, update maximum and need
	for (int i = 0; i < numberOfResources; i++) {
		maximum[customerIndex][i] = maximumDemand[i];
		need[customerIndex][i] = maximumDemand[i] - allocation[customerIndex][i];
	}
}

/**
 * Checks if the request will leave the bank in a safe state.
 * @param customerIndex  The customer's index (0-indexed).
 * @param request        An array of the requested count for each resource.
 * @return 1 if the requested resources will leave the bank in a
 *         safe state, else 0
 */
int checkSafe(int customerIndex, int *request)
{
	// Allocate temporary memory to copy the bank state.
	int *work = mallocIntVector(numberOfResources);
	int **tempNeed = mallocIntMatrix(numberOfCustomers, numberOfResources);
	int **tempAllocation = mallocIntMatrix(numberOfCustomers, numberOfResources);

	int i;
	int j;

	// TODO: copy the bank's state to the temporary memory and update it with the request.
	for (i = 0; i < numberOfResources; i++) {
		work[i] = available[i] - request[i];

		for (j = 0; j < numberOfCustomers; j++) {
			tempNeed[j][i] = need[j][i];
			tempAllocation[j][i] = allocation[j][i];
		}

		// For the particular customer, edit need and allocation
		tempNeed[customerIndex][i] -= request[i];
		tempAllocation[customerIndex][i] += request[i];
	}

	// Init finish array with '0'
	int *finish = mallocIntVector(numberOfCustomers);
	int done = 0;

	while (done == 0) {

		done = 1;

		int didSomething = 0;
		for (i = 0; i < numberOfCustomers; i++) {

			// Task i is not finished yet
			if (finish[i] == 0) {
				// Check if Need[i] <= Work for all resources
				int exceededResources = 0;
				for (j = 0; j < numberOfResources; j++) {
					if (tempNeed[i][j] > work[j]) {
						exceededResources = 1;
						break;
					}
				}

				// If resources have not been exceeded, then job has been done and release resources back to work
				if (!exceededResources) {
					didSomething = 1;
					for (j = 0; j < numberOfResources; j++) {
						work[j] += tempAllocation[i][j];
					}
					finish[i] = 1;
					break;
				} 
			}
		}

		// HELPER TO PRINT FOR DEBUG

		if (BUDGE) {
			// TODO: print the current state with a tidy format
			printf("\nIn safe Checker Mode:\n");
			printf("didSomething: %d\n", didSomething);

			printf("\nFinish:\n");
			for (i = 0; i < numberOfCustomers; i++)
			{
				printf("%d ", finish[i]); // leave a space between each print
			}
			printf("\n"); // Leave a line after printing finish

			// TODO: print available
			printf("Available:\n");
			for (i = 0; i < numberOfResources; i++)
			{
				printf("%d ", work[i]); // leave a space between each print
			}
			printf("\n"); // Leave a line after printing available

			// TODO: print allocation
			printf("Allocation:\n");
			for (i = 0; i < numberOfCustomers; i++)
			{
				for (j = 0; j < numberOfResources; j++)
				{
					printf("%d ", tempAllocation[i][j]); // Leave a space between each print
				}
				printf("\n"); // Leave a line after each customer
			}
			printf("\n"); // Leave a line after printing Allcoation

			// TODO: print need
			printf("Need:\n");
			for (i = 0; i < numberOfCustomers; i++)
			{
				for (j = 0; j < numberOfResources; j++)
				{
					printf("%d ", tempNeed[i][j]); // Leave a space bettern each print
				}
				printf("\n"); // Leave a line after each customer
			}
			printf("\n"); // Leave a line after printing need
		}

		if (didSomething == 0) {
			// We could not find any job to proceed, thus the system is not safe
			freeIntVector(work);
			freeIntMatrix(tempNeed);
			freeIntMatrix(tempAllocation);

			return 0;
		}

		// Else, check if all finish are 1, if so then it means that we have completed and the system is safe
		for (i = 0; i < numberOfCustomers; i++) {
			if (finish[i] == 0) {
				done = 0;
				break;
			}
		}


	}


	// TODO: check if the new state is safe
	// If not safe, then return state back to that of the temporary memory

	freeIntVector(work);
	freeIntMatrix(tempNeed);
	freeIntMatrix(tempAllocation);

	return 1;
}

/**
 * Requests resources for a customer loan.
 * If the request leave the bank in a safe state, it is carried out.
 * @param customerIndex  The customer's index (0-indexed).
 * @param request        An array of the requested count for each resource.
 * @return 1 if the requested resources can be loaned, else 0.
 */
int requestResources(int customerIndex, int *request)
{
	// TODO: print the request
	printf("Customer %d requesting\n", customerIndex);
	for (int i = 0; i < numberOfResources; i++)
	{
		printf("%d ", request[i]); // Leave a space between each request
	}
	printf("\n"); // Leave a line after each customer

	// TODO: judge if request larger than need
	// bool bad_request = false;
	for (int i = 0; i < numberOfResources; i++) {
		if (request[i] > need[customerIndex][i]) {
			return 0;
		}
	}

	// TODO: judge if request larger than available

	for (int i = 0; i < numberOfResources; i++) {
		if (request[i] > available[i]) {
			return 0;
		}
	}

	// TODO: judge if the new state is safe if grants this request (for question 2)

	if (!checkSafe(customerIndex, request)) {
		if (BUDGE) printf("NOT SAFE\n");
		return 0;
	}

	// TODO: request is granted, update state

	for (int i = 0; i < numberOfResources; i++) {
		if (BUDGE) printf("i is %d, available is %d, request is %d\n", i, available[i], request[i]);
		available[i] -= request[i];
		allocation[customerIndex][i] += request[i];
		need[customerIndex][i] -= request[i];
		if (BUDGE) printf("i is %d, available is %d, request is %d\n", i, available[i], request[i]);
	}
	
	return 1;
}

/**
 * Releases resources borrowed by a customer. Assume release is valid for simplicity.
 * @param customerIndex  The customer's index (0-indexed).
 * @param release        An array of the release count for each resource.
 */
void releaseResources(int customerIndex, int *release)
{
	// TODO: print the release
	printf("Customer %d releasing\n", customerIndex);
	for (int i = 0; i < numberOfResources; i++)
	{
		printf("%d ", release[i]); // Leave a space between each request
	}

	printf("\n"); // Leave a line after each customer

	// TODO: deal with release (:For simplicity, we do not judge the release request, just update directly)
	for (int i = 0; i < numberOfResources; i++) {
		available[i] += release[i];
		allocation[customerIndex][i] -= release[i];
		need[customerIndex][i] += release[i];
	}
}

/**
 * Parses and runs the file simulating a series of resource request and releases.
 * Provided for your convenience.
 * @param filename  The name of the file.
 */
void runFile(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	int c = 0, i = 0, j = 0, m = 0, n = 0, bankInited = 0,
		lineLen = 0, prevLineEnd = 0, maxLineLen = 0;
	do
	{
		if (c == '\n' || c == EOF)
		{
			lineLen = i - prevLineEnd;
			prevLineEnd = i;
			if (lineLen > maxLineLen)
				maxLineLen = lineLen;
		}
		i++;
	} while ((c = fgetc(fp)) != EOF);

	rewind(fp);
	lineLen++;
	char *line = malloc(lineLen), *token;
	i = 0;
	while (fgets(line, lineLen, fp) != NULL)
	{
		for (j = 0; j < lineLen - 1; j++)
			if (line[j] == '\n')
				line[j] = '\0';
		if (i == 0)
		{
			token = strtok(line, ",");
			token = strtok(NULL, ",");
			n = atoi(token);
		}
		else if (i == 1)
		{
			token = strtok(line, ",");
			token = strtok(NULL, ",");
			m = atoi(token);
		}
		else if (i == 2)
		{
			token = strtok(line, ",");
			token = strtok(NULL, ",");
			int *resources = malloc(sizeof(int) * m);
			for (j = 0; j < m; j++)
			{
				resources[j] = atoi(strtok(j == 0 ? token : NULL, " "));
			}
			initBank(resources, m, n);
			bankInited = 1;
			free(resources);
		}
		else
		{
			int *resources = malloc(sizeof(int) * m);
			token = strtok(line, ",");
			if (strcmp(token, "c") == 0)
			{
				int customerIndex = atoi(strtok(NULL, ","));
				int *resources = malloc(sizeof(int) * m);
				token = strtok(NULL, ",");
				for (j = 0; j < m; j++)
				{
					resources[j] = atoi(strtok(j == 0 ? token : NULL, " "));
				}
				setMaximumDemand(customerIndex, resources);
				free(resources);
			}
			else if (strcmp(token, "r") == 0)
			{
				int customerIndex = atoi(strtok(NULL, ","));
				int *resources = malloc(sizeof(int) * m);
				token = strtok(NULL, ",");
				for (j = 0; j < m; j++)
				{
					resources[j] = atoi(strtok(j == 0 ? token : NULL, " "));
				}
				requestResources(customerIndex, resources);
				free(resources);
			}
			else if (strcmp(token, "f") == 0)
			{
				int customerIndex = atoi(strtok(NULL, ","));
				int *resources = malloc(sizeof(int) * m);
				token = strtok(NULL, ",");
				for (j = 0; j < m; j++)
				{
					resources[j] = atoi(strtok(j == 0 ? token : NULL, " "));
				}
				releaseResources(customerIndex, resources);
				free(resources);
			}
			else if (strcmp(token, "p") == 0)
			{
				printState();
			}
		}
		i++;
	}
	if (bankInited)
		freeBank();
	free(line);
	fclose(fp);
}

/**
 * Main function
 * @param args  The command line arguments
 */
int main(int argc, const char **argv)
{
	if (argc > 1)
	{
		runFile(argv[1]);
	}
	return 0;
}

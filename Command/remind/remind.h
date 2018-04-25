#ifndef REMIND_H_
#define REMIND_H_
struct reminder{
	char *date;
	char *memo[10];
	int current;
};

FILE *fp;

struct reminder memobook[20];
int used;
// Allocate Memory
void initialize();
// Destroy Memory
void cleanup();
// Search for date;
int search(char *date);
// Add entry
void addentry(char *date, char *rem);
// Delete Entry
void deleteentry(char *date);
// Display
void displayAll(char *date);
#endif
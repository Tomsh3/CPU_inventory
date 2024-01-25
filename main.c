#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 200
#define MAX_STR 400

// define cpu struct
typedef struct cpu
{
	char name[NAME_LENGTH];
	int copies;
	struct cpu *next;
}CPU;

// Declaration of functions:
int check_arg(int num_of_arg_rec, char *argv[]);
void free_list(CPU *head);
CPU* new_cpu(char* name, int copies, CPU *head);
CPU* sort_list(CPU *head, CPU *new_cpu);
CPU *create_list(FILE *file);
CPU *remove_cpu(CPU *head, char *curr_name);
CPU *rename_cpu(CPU *head, char *curr_name, char *new_name);
CPU *returned_cpu(CPU *head, char *curr_name, int add_copies);
void malfunc_cpu(CPU *head, char *curr_name, int add_copies);

int main(int argc, char *argv[])
{
	FILE *f_act = NULL, *fuptd_cpus = NULL;
	CPU *head = NULL;
	char line[MAX_STR];
	int act_num = 0;
	if (check_arg(argc, argv) != 0) return 1;
	f_act = fopen(argv[2], "r");
	fgets(line, MAX_STR, f_act);
	if (create_list(argv[1]) != 1) head = create_list(argv[1]);
	else return 1;

	//work with actions:
	while (!feof(f_act))
	{
		fgets(line, MAX_STR, f_act);
		char curr_name[MAX_STR], new_name[MAX_STR], act_line[MAX_STR], temp[MAX_STR];
		int copies = 0;
		char *act1 = "Rename ";
		char *act2 = "Returned_from_customer ";
		char *act3 = "Fatal_malfunction ";
		char *act4 = "Finalize";
		strcpy(act_line, strtok(line, "$"));
		if (strcmp(act_line, act1) == 0) act_num = 1;
		if (strcmp(act_line, act2) == 0) act_num = 2;
		if (strcmp(act_line, act3) == 0) act_num = 3;
		if (strcmp(act_line, act4) == 0) act_num = 4;
		switch (act_num)
		{
		case 1:
			strcpy(curr_name, strtok(NULL, "$"));
			curr_name[strlen(curr_name) - 1] = '\0';
			strcpy(new_name, strtok(NULL, "$"));
			new_name[strlen(new_name) - 1] = '\0';
			head = rename_cpu(head, curr_name, new_name);
			break;

		case 2:
			strcpy(curr_name, strtok(NULL, "$"));
			curr_name[strlen(curr_name) - 1] = '\0';
			strcpy(temp, strtok(NULL, "$"));
			temp[strlen(temp) - 1] = '\0';
			copies = atoi(temp);
			head = returned_cpu(head, curr_name, copies);
			break;
		case 3:
			strcpy(curr_name, strtok(NULL, "$"));
			curr_name[strlen(curr_name) - 1] = '\0';
			strcpy(temp, strtok(NULL, "$"));
			temp[strlen(temp) - 1] = '\0';
			copies = atoi(temp);
			malfunc_cpu(head, curr_name, copies);
			break;
		case 4:
			fuptd_cpus = fopen(argv[3], "w");
			CPU *curr_cpu = NULL;
			curr_cpu = head;
			if (head == NULL) return 0; // Empty list, no CPU to print.
			while (curr_cpu != NULL)
			{
				fprintf(fuptd_cpus, "%s $$$ %d\n", curr_cpu->name, curr_cpu->copies);
				curr_cpu = curr_cpu->next;
			}
			fclose(fuptd_cpus);
			free_list(head);
			fclose(f_act);
			return 0;
		}
	}
}

//Input: number of arguments received by the user & list of pointers to relevent files. 
//return parameter: 1 if the file opening has failed and 0 if everything is okay. 
//Function functionality : Check if the files inserted by the user are valid and can be open.
int check_arg(int num_of_arg_rec, char *argv[])
{
	FILE *fptr1 = NULL, *fptr2 = NULL, *fptr3 = NULL;
	if (num_of_arg_rec != 4)
	{
		// Bad number of arguments
		printf("Error: invalid number of arguments (<%d> instead of 3)\n", (num_of_arg_rec - 1));
		return 1;
	}
	if (NULL == (fptr1 = fopen(argv[1], "r")))
	{
		// Error with opening a file
		printf("Error: opening %s failed\n", argv[1]);
		return 1;
	}
	if (NULL == (fptr2 = fopen(argv[2], "r")))
	{
		// Error with opening a file
		printf("Error: opening %s failed\n", argv[2]);
		return 1;
	}
	if (NULL == (fptr3 = fopen(argv[3], "w")))
	{
		// Error with opening a file
		printf("Error: opening %s failed\n", argv[3]);
		return 1;
	}
	fclose(fptr1);
	fclose(fptr2);
	fclose(fptr3);
	return 0;
}

//Input: A pointer to the head of a linked list.
//return parameter: None.
//Function functionality : The function free the memory allocation that has been made.
void free_list(CPU *head)
{
	if (head == NULL) return;

	free_list(head->next);
	free(head);
}

//Input: A pointer to the head of a linked list, name of new cpu and number of copies.
//return parameter: pointer to new cpu.
//Function functionality : The function allocate memory and define new cpu.
CPU* new_cpu(char* name, int copies, CPU *head)
{
	CPU *ncpu = (CPU*)malloc(sizeof(CPU));
	if (ncpu == NULL) // memory allocation failed
	{
		printf("Error: memory allocation failed\n");
		free_list(head);
		return 1;
	}
	if (ncpu != NULL) // memory allocation succeed
	{
		strcpy(ncpu->name, name);
		ncpu->copies = copies;
		ncpu->next = NULL;
	}
	return ncpu;
}

//Input: A pointer to the head of a linked list and pointer the the new cpu created.
//return parameter: The head of the new linked list.
//Function functionality : Sort list based on cpu's name (order define by ascii table).
CPU* sort_list(CPU *head, CPU *new_cpu)
{
	CPU *curr_cpu, *prev_cpu = NULL;

	if (new_cpu = 1) return new_cpu; // error in creating new CPU
	if (head == NULL) return new_cpu; // empty list
	if (strcmp(new_cpu->name, head->name) < 0)
	{
		new_cpu->next = head;
		return new_cpu;
	}
	curr_cpu = head;
	while (curr_cpu != NULL && (strcmp(curr_cpu->name, new_cpu->name) < 0))
	{
		prev_cpu = curr_cpu;
		curr_cpu = curr_cpu->next;
	}
	prev_cpu->next = new_cpu;
	new_cpu->next = curr_cpu;

	return head;
}

//Input: File that contain's list of cpus and the number of copies for each.
//return parameter: The head of the new linked list.
//Function functionality : Create sorted linked list of cpus, based on data from file (cpus.txt)
CPU *create_list(FILE *file)
{
	FILE *fcpuread = NULL;
	CPU *head = NULL;
	char name[NAME_LENGTH], line[MAX_STR], temp[MAX_STR];
	int copies = 0;
	fcpuread = fopen(file, "r");
	while (fgets(line, MAX_STR, fcpuread))
	{
		strcpy(name, strtok(line, "$"));
		name[strlen(name) - 1] = '\0'; // replace the space char at the end with '\0'
		strcpy(temp, strtok(NULL, "$"));
		if (temp[-1] == '\n') temp[strlen(temp) - 1] = '\0';
		copies = atoi(temp); // convert str to int
		CPU *a = new_cpu(name, copies, head);
		if (sort_list(head, a) != 1) head = sort_list(head, a); // check for error, if working - sort list.
		else // in case of error
		{
			fclose(fcpuread);
			return 1;
		}
	}
	fclose(fcpuread);
	return head;
}

//Input: Head of a linked list, the name of the requested cpu, the new name of the relevant cpu.
//return parameter: The head of the new linked list.
//Function functionality : Change the name of specific cpu (choose by the user) and sort the linked list after the changes.
CPU *rename_cpu(CPU *head, char *curr_name, char *new_name)
{
	CPU *curr_cpu = NULL;
	int exist_copies = 0;
	char *cur_name = curr_name + 1; // Trim leading space.
	new_name++; // Trim leading space.
	curr_cpu = head;
	if (head == NULL) return head; // Empty list, no CPU to rename.
	while (curr_cpu != NULL) // Not an empty list.
		if (strcmp(curr_cpu->name, cur_name) == 0) // A match has found.
		{
			exist_copies = curr_cpu->copies;
			CPU *a = new_cpu(new_name, exist_copies, head);
			head = remove_cpu(head, cur_name);
			head = sort_list(head, a);
			return head;
		}
		else curr_cpu = curr_cpu->next; // No match - Move to the next CPU.
	return head;
}

//Input: Head of a linked list, the name of the requested cpu.
//return parameter: The head of the new linked list.
//Function functionality : remove specific cpu from the list.
CPU *remove_cpu(CPU *head, char *curr_name)
{
	if (head == NULL) return head; // In case of empty list.

	CPU *cur = NULL, *prev = NULL;
	cur = head;
	if (strcmp(cur->name, curr_name) == 0)// If there is a match for the first CPU in the list.
	{
		head = head->next;
		free(cur);
		return head;
	}

	while (cur != NULL && strcmp(cur->name, curr_name) != 0)
	{
		prev = cur;
		cur = cur->next;
	}
	if (cur != NULL) // in case of match
	{
		prev->next = cur->next;
		free(cur);
	}
	return head;
}

//Input: A pointer to the head of a linked list, name of specific cpu and the number of copies to return.
//return parameter: The head of the linked list.
//Function functionality : Check if requested cpu is available, if it does - adding number of copies, if not - create new cpu and sort the linked list.
CPU *returned_cpu(CPU *head, char *curr_name, int add_copies)
{
	CPU *curr_cpu = NULL;
	curr_cpu = head;
	char *cur_name = curr_name + 1; // Trim leading space.
	while (curr_cpu != NULL)
		if (strcmp(curr_cpu->name, cur_name) == 0)
		{
			curr_cpu->copies = (curr_cpu->copies + add_copies);
			break;
		}
		else curr_cpu = curr_cpu->next;

	/* add cpu to list */
	if (curr_cpu == NULL)
	{
		CPU *a = new_cpu(cur_name, add_copies, head);
		head = sort_list(head, a);
	}
	return head;

}

//Input: A pointer to the head of a linked list, name of specific cpu and number of malfunction cpus.
//return parameter: None.
//Function functionality : Updating the number of spcific cpu, based on malfunction cpu's copies received by the user.
void malfunc_cpu(CPU *head, char *curr_name, int mal_copies)
{
	CPU *curr_cpu = NULL;
	curr_cpu = head;
	char *cur_name = curr_name + 1; // Trim leading space.
	while (curr_cpu != NULL)
		if (strcmp(curr_cpu->name, cur_name) == 0)
		{
			if (mal_copies >= curr_cpu->copies) curr_cpu->copies = 0;
			else curr_cpu->copies = ((curr_cpu->copies) - mal_copies);
			break;
		}
		else curr_cpu = curr_cpu->next;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Line {
	char label[10];
	char opcode[10];
	char operand[10];
	struct Line* nextLine;
	int ad;
} Line;

typedef struct Table {
	int adSymbol;
	struct Table* nextTable;
	struct Line* symbol;
} Table;

typedef struct Midfile {
	int adLine;
	struct Midfile* nextMidfile;
	struct Line* ptrLabel;
	struct Line* ptrOpcode;
	struct Line* ptrOperand;
} Midfile;

int main(int argc, char* argv[]) {
	int opt = 0;
	int address = 0;
	int end = 0;
	char askii[10] = { '\0' };
	char fromhexa[10] = { '\0' };
	char* filename = NULL;
	char* for16 = NULL;
	Line* head = NULL;
	Line* cur = NULL;
	Line* tail = NULL;
	Table* head2 = NULL;
	Table* cur2 = NULL;
	Table* tail2 = NULL;
	Midfile* head3 = NULL;
	Midfile* cur3 = NULL;
	Midfile* tail3 = NULL;
	Midfile* forOB = NULL;
	Midfile* forOB2 = NULL;

	while ((opt = getopt(argc, argv, "i:")) != -1) {
		if (opt == 'i') {
			filename = optarg;
		}
	}

	FILE* fp = fopen(filename, "rt");

	if (fp == NULL) {
		printf("no File");
		exit(-1);
	}

	while (feof(fp) == 0) {
		Line* line = (Line*) malloc(sizeof(Line));
		line->nextLine = NULL;

		if (head == NULL) {
			head = line;
			cur = head;
		} else {
			tail->nextLine = line;
		}

		tail = line;
		fscanf(fp, "%s %s %s", line->label, line->opcode, line->operand);

		if (head->nextLine == NULL) {
			address = atoi(line->operand);
		} else {
			if (strcmp(line->label, "**") == 0) {
				line->ad = address;
				address += 3;
			} else {
				if (strcmp(line->opcode, "RESW") == 0) {
					line->ad = address;
					address += atoi(line->operand) * 3;
				} else if (strcmp(line->opcode, "WORD") == 0) {
					line->ad = address;
					address += 3;
				} else if (strcmp(line->opcode, "BYTE") == 0) {
					line->ad = address;
					address += 1;
				} else if (strcmp(line->opcode, "RESB") == 0) {
					line->ad = address;
					address += atoi(line->operand);
				}

				Table* table = (Table*) malloc(sizeof(Table));
				table->nextTable = NULL;

				if (head2 == NULL) {
					head2 = table;
					cur2 = head2;
				} else {
					tail2->nextTable = table;
				}

				tail2 = table;
				table->symbol = line;
				table->adSymbol = line->ad;
			}
		}

		if (strcmp(tail->label, "**") == 0 && strcmp(tail->opcode, "END") == 0) {
			break;
		}
	}

	fclose(fp);

	while (cur != NULL) {
		Midfile* midfile = (Midfile*) malloc(sizeof(Midfile));
		midfile->nextMidfile = NULL;

		if (head3 == NULL) {
			head3 = midfile;
			forOB = head3;
			forOB2 = head3;
			cur3 = head3;
		} else {
			tail3->nextMidfile = midfile;
		}

		tail3 = midfile;
		midfile->adLine = cur->ad;
		midfile->ptrLabel = cur;
		midfile->ptrOpcode = cur;
		midfile->ptrOperand = cur;
		cur = cur->nextLine;
	}

	cur = head;
	printf("The contents of Input file\n\n");

	while (cur != NULL) {
		printf("%s %s %s\n", cur->label, cur->opcode, cur->operand);
		cur = cur->nextLine;
	}

	printf("\n\n");
	printf("Length of the input program is %d.\n\n", tail->ad - head->nextLine->ad);
	printf("The contents of Symbol Table:\n\n");

	while (cur2 != NULL) {
		printf("%s %d\n", cur2->symbol->label, cur2->adSymbol);
		cur2 = cur2->nextTable;
	}

	printf("\n\n");
	printf("The contents of Intermed File:\n\n");

	while (cur3 != NULL) {
		if (cur3 == head3) {
			printf("%s %s %s\n", cur3->ptrLabel->label, cur3->ptrOpcode->opcode, cur3->ptrOperand->operand);
			cur3 = cur3->nextMidfile;
		}

		printf("%d %s %s %s\n", cur3->adLine, cur3->ptrLabel->label, cur3->ptrOpcode->opcode, cur3->ptrOperand->operand);
		cur3 = cur3->nextMidfile;
	}

	printf("\n");
	printf("Object Program has been generated.\n\n");
	printf("Object Program:\n\n");
	printf("H^COPY^%06d^%06d\n", head3->nextMidfile->adLine, tail3->adLine);
	printf("T^%06d^%d", head3->nextMidfile->adLine, tail3->adLine - head3->nextMidfile->adLine - 1);
	forOB = head3->nextMidfile;

	while (forOB != tail3) {
		if (strcmp(forOB->ptrLabel->label, "**") == 0) {
			if (strcmp(forOB->ptrOpcode->opcode, "LDA") == 0) {
				while (forOB2 != NULL) {
					if (strcmp(forOB->ptrOperand->operand, forOB2->ptrLabel->label) == 0) {
						printf("^33");
						printf("%d", forOB2->adLine);
						break;
					}

					forOB2 = forOB2->nextMidfile;
				}

				forOB2 = head3;
			} else if (strcmp(forOB->ptrOpcode->opcode, "STA") == 0) {
				while (forOB2 != NULL) {
					if (strcmp(forOB->ptrOperand->operand, forOB2->ptrLabel->label) == 0) {
						printf("^44");
						printf("%d", forOB2->adLine);
						break;
					}

					forOB2 = forOB2->nextMidfile;
				}

				forOB2 = head3;
			} else if (strcmp(forOB->ptrOpcode->opcode, "LDCH") == 0) {
				while (forOB2 != NULL) {
					if (strcmp(forOB->ptrOperand->operand, forOB2->ptrLabel->label) == 0) {
						printf("^53");
						printf("%d", forOB2->adLine);
						break;
					}

					forOB2 = forOB2->nextMidfile;
				}

				forOB2 = head3;
			} else if (strcmp(forOB->ptrOpcode->opcode, "STCH") == 0) {
				while (forOB2 != NULL) {
					if (strcmp(forOB->ptrOperand->operand, forOB2->ptrLabel->label) == 0) {
						printf("^57");
						printf("%d", forOB2->adLine);
						break;
					}

					forOB2 = forOB2->nextMidfile;
				}

				forOB2 = head3;
			}
		} else {
			if (strcmp(forOB->ptrOpcode->opcode, "WORD") == 0) {
				printf("^%06d", atoi(forOB->ptrOperand->operand));
			} else if (strcmp(forOB->ptrOpcode->opcode, "BYTE") == 0) {
				printf("^");

				if (forOB->ptrOperand->operand[0] == 'C') {
					for16 = strtok(forOB->ptrOperand->operand, "\'");
					for16 = strtok(NULL, "\'");

					for (int i = 0; i < 10; i++) {
						askii[i] = for16[i];

						if (askii[i] == '\0') {
							end = i;
							break;
						}
					}

					for (int i = 0; i < end; i++) {
						printf("%x", askii[i]);
					}
				} else if (forOB->ptrOperand->operand[0] == 'X') {
					for16 = strtok(forOB->ptrOperand->operand, "\'");
					for16 = strtok(NULL, "\'");

					for (int i = 0; i < 10; i++) {
						fromhexa[i] = for16[i];

						if (fromhexa[i] == '\0') {
							end = i;
							break;
						}
					}

					for (int i = 0; i < end; i++) {
						printf("%c", fromhexa[i]);
					}
				}
			}
		}

		forOB = forOB->nextMidfile;
	}

	printf("\n");
	printf("E^%06d\n", head3->nextMidfile->adLine);
	return 0;
}
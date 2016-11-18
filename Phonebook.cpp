/*
 * Phonebook.c
 *
 *  Created on: Mar 18, 2016
 *      Author: Ahmed Lotfey Siam && Abdelrahman Abdelnabi
 */

//Phonebook PROJECT :D "matshelsh el smile face ya re5em"
/****** UPDATES *******
 * (1) MODIFY now takes an integer , which is the index in data[] of the contact to modify
 * (2) MODIFY now doesn't ask the user for the name of the contact to modify, it goes directly to modification
 * (3) MODIFY now puts the contact in its correct position if the first/last name has been modified by the user
 * (4) MODIFY algorithm is now updated to delete the original record in case the user changes the first/last name
 *     and inserts the contact again in its correct position
 * (5) NEW FUNCTION ADDED "insertRecord()". it inserts the record in its correct position in the list
 * (6) NEW FUNCTION ADDED "isExit()". checks if a pervious contact already exists with a given first and last names
 */

/************************* INCLUDES ***************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/************************* DEFINITIONS *************************/
#define flush fflush(stdin);
#define NAME 30
#define STREET 50
#define CITY 30
#define EMAIL 50
#define NUMBER 14
#define MAXSIZE 10000 //the number of contacts in the phonebook
#define ChoiceCondition choice == 'Y' || choice == 'y' || choice == 'N' || choice == 'n'

/******************** STRUCTURE DEFINITIONS ********************/

typedef struct address {
    char street[STREET];
    int flat;
    char city[CITY];
} address;

typedef struct contact {
    long long pn; //I CHANGED THE DATA TYPE OF THE PHONE NUMBER TO LONG LONG !!!
    char firstName[NAME], lastName[NAME]; //"firstName" was mis-spelled, spelling was corrected
    char email[EMAIL];
    address location;
} contact;

/********************* FUNCTION PROTOTYPES *********************/

int save(void);

int load(void);

int search(char *, int);

int addRecord(void);

int modifyRecord(int);

void insertRecord(contact);

int control(void);

void printEntries(int);

void printRecord(contact *);

int isExist(char *, char *);

//getters
void getFirstName(contact *);

void getLastName(contact *);

void getAddress(contact *);

void getFlatNumber(contact *);

void getCity(contact *);

void getPhoneNumber(contact *);

void getEmail(contact *);

int getChoice(void);

int validateName(char *, int);

int validateAddress(char *);

int validateNumber(char *, int);

int validateEmail(char *);

/********************** GLOBAL VARIABLES **********************/

int searchResults[1000]; // This stored contacts index for each matched element found when searching
int records, searchNum = 0; // search num  = number of matched elements returned from search function
int saveFlag = 0;
FILE *f;
contact data[MAXSIZE];

/********************* BEGIN MAIN ****************************/

int main() {
    setbuf(stdout, NULL);
    load();
    control();

    return EXIT_SUCCESS;
}

/**********************************END MAIN*******************/

/************* CONTROL ************/
int control() {
    char selection[2];
    menu:
    printf(""
                   "\n********* MAIN MENU *************\n"
                   "**  1 - Show all contacts.     **\n"
                   "**  2 - Search.                **\n"
                   "**  3 - New contact.           **\n"
                   "**  4 - Modify an entry.       **\n"
                   "**  5 - Delete an entry.       **\n"
                   "**  6 - Save.                  **\n"
                   "**  7 - Exit.                  **\n"
                   "*********************************\n");
    menuRechoose:
    printf("Pick from above -->\n");
    flush;
    scanf("%c", &selection[0]);
    selection[1] = '\0';
    flush;
// option 1 ****************************************************************************************************************

    if (selection[0] == '1') {
        printEntries(records);
        printf("Press any key to go main menu...");
        getchar();
        system("cls");
        goto menu;
    }

        /**option 2 ****************************************************************************************************************/

    else if (selection[0] == '2') {
        research:
        printf("\n");
        char lok[30]; /* Start */
        int s, i = 0;
        printf("Enter 1st name:");
        flush;
        /*	Takes an string from user to search for it*/
        gets(lok);
        int x = strlen(lok);
        lok[x] = '\0';
        flush; /* End */
        s = search(lok, records);
        // search for entered string && s refers to number of matches found
        if (s == 0 || x == 0) {
            //if nothing found
            char z;   // checks if user want to re-enter
            printf("\n\nError 404 : Entry not found !\n"
                           " \"Y\" To retry.\n"
                           " Anykey to main menu.\n");
            scanf("%c", &z);
            if (z == 'Y' || z == 'y')
                goto research;
            else {
                system("cls");
                goto menu;
            }
        }   // if matches found ...
        for (i = 0; i < s; i++)  //printing search results.
            printf("%-4d -->  %s,%s,%s,%d,%s,%lld,%s.\n", searchResults[i],
                   data[searchResults[i]].firstName,
                   data[searchResults[i]].lastName,
                   data[searchResults[i]].location.street,
                   data[searchResults[i]].location.flat,
                   data[searchResults[i]].location.city,
                   data[searchResults[i]].pn, data[searchResults[i]].email);
        printf("Press anykey to go main menu...\n");
        getchar();
        system("cls");
        goto menu;

    }

        /**option 3 ****************************************************************************************************************/

    else if (selection[0] == '3') {
        addRecord();
        printf("Press any key to go main menu...\n");
        getchar();
        system("cls");
        goto menu;
    }

        /**option 4 ****************************************************************************************************************/

    else if (selection[0] == '4') {
        Modify:
        printf("\n");
        char lok[30];
        int s, i = 0;
        printf("Enter 1st name of the contact to modify:\n");
        flush;

        gets(lok);
        int x = strlen(lok);
        lok[x] = '\0';
        flush;
        s = search(lok, records);
        if (s == 0) {
            char z;

            printf("\n\nError 404 : Entry not found !\n"
                           " Retry ? \"Y\" To retry.\n"
                           " Anykey to main menu.\n");
            fflush(stdin);
            scanf("%c", &z);
            fflush(stdin);
            if (z == 'Y' || z == 'y')
                goto Modify;
            else
                goto menu;
        }
        for (i = 0; i < s; i++)  //printing search results.
            printf("%-4d -->  %s,%s,%s,%d,%s,%lld,%s.\n", searchResults[i],
                   data[searchResults[i]].firstName,
                   data[searchResults[i]].lastName,
                   data[searchResults[i]].location.street,
                   data[searchResults[i]].location.flat,
                   data[searchResults[i]].location.city,
                   data[searchResults[i]].pn, data[searchResults[i]].email);
        Re_Enter_index:
        printf("Choose Entry Num to modify\n"); // takes index of an entry from the user to be modified
        int o, found = 0;
        fflush(stdin);
        scanf("%d", &o);
        fflush(stdin);
        for (i = 0; i < searchNum; i++) { // check if the selected index is found in the array of search results
            if (o == searchResults[i]) {
                found = 1;
                break;
            }
        }
        if (found == 1) { // if found confirm that this entry will be modified...
            printf("Are you sure u want to modify this entry ?  ( Y/N ) \n");
            printf("%-4d -->  %s,%s,%s,%d,%s,%lld,%s.\n", o, data[o].firstName,
                   data[o].lastName, data[o].location.street,
                   data[o].location.flat, data[o].location.city, data[o].pn,
                   data[o].email);
            char chec;
            fflush(stdin);
            scanf("%c", &chec);
            fflush(stdin);
            if (chec == 'Y' || chec == 'y') {
                /**************************************************************************
                 * ***********************************************************************
                 * ***********************************************************************/

                modifyRecord(o); // o refers to index of an item in structure " data " array

                /**************************************************************************
                 * ***********************************************************************
                 * ***********************************************************************/
            }
        } else {
            printf("Entry was not found !\nRe-Enter ? ( Y - N )\n");
            char check;
            fflush(stdin);
            scanf("%c", &check);
            fflush(stdin);
            {
                if (check == 'y' || check == 'Y') {
                    goto Re_Enter_index;
                }
            }
        }
        system("cls");
        goto menu;
    }

        /**option 5 ****************************************************************************************************************/

    else if (selection[0] == '5') {
        iFedUP:
        printf("\n");
        char lok[30];
        int s, i = 0;
        printf("Enter 1st name:");
        flush;
        gets(lok);
        int x = strlen(lok);
        lok[x] = '\0';
        flush;
        s = search(lok, records);
        if (s == 0) {
            char z;

            printf("\n\nError 404 : Entry not found !\n"
                           " Retry ? \"Y\" To retry.\n"
                           " Anykey to main menu.\n");
            fflush(stdin);
            scanf("%c", &z);
            fflush(stdin);
            if (z == 'Y' || z == 'y')
                goto iFedUP;
            else {
                system("cls");
                goto menu;
            }
        }
        for (i = 0; i < s; i++)  //printing search results.
            printf("%-4d -->  %s,%s,%s,%d,%s,%lld,%s.\n", searchResults[i],
                   data[searchResults[i]].firstName,
                   data[searchResults[i]].lastName,
                   data[searchResults[i]].location.street,
                   data[searchResults[i]].location.flat,
                   data[searchResults[i]].location.city,
                   data[searchResults[i]].pn, data[searchResults[i]].email);
        Re_Enter_Delete:
        printf("Choose Entry Num to delete\n"); // takes index of an entry from the user to be deleted
        int o, found = 0;
        fflush(stdin);
        scanf("%d", &o);
        fflush(stdin);
        for (i = 0; i < searchNum; i++) { // check if the selected index is found in the array of search results
            if (o == searchResults[i]) {
                found = 1;
                break;
            }
        }
        if (found == 1) { // if found confirm that this entry will be removed...
            printf("Are you sure u want to delete this entry ?  ( Y - N ) \n");
            printf("%-4d -->  %s,%s,%s,%d,%s,%lld,%s.\n", o, data[o].firstName,
                   data[o].lastName, data[o].location.street,
                   data[o].location.flat, data[o].location.city, data[o].pn,
                   data[o].email);
            char chec;
            fflush(stdin);
            scanf("%c", &chec);
            fflush(stdin);
            if (chec == 'Y' || chec == 'y') { // delete value !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                for (; o < records; o++) {
                    data[o] = data[o + 1];
                }
                records -= 1; // records reduction
                saveFlag = 1; // save flag
                printf("Contact Deleted Successfully !\n"
                               "Enter any key to go main menu...");
                getchar();
                system("cls");
                goto menu;
            } else {
                printf(
                        "Contact was not removed !\nPress anykey to go main menu\n");
                getchar();
                system("cls");
                goto menu;
            }
        } else {
            printf("Wrong Entry !   Re-try ? ( Y - N )");
            char ok1;
            scanf("%c", &ok1);
            if (ok1 == 'Y' || ok1 == 'y') {
                goto Re_Enter_Delete;
            }
            system("cls");
            goto menu;
        }
    }
        /** option 6 ****************************************************************************************************************/
    else if (selection[0] == '6') {
        save();
        printf("\nPress any key to go main menu...\n");
        getchar();
        system("cls");
        goto menu;
    }
        /** option 7 ****************************************************************************************************************/

    else if (selection[0] == '7') {
        if (saveFlag) {
            printf("You did NOT save your work !!\n"
                           "Exit phonebook?\n");
            printf("( Y ) to Confirm , Anykey to continue...\n");
            char s;
            int x = 0;
            flush;
            scanf("%c", &s);
            flush;
            x = ((s == 'Y' || s == 'y') ? 0 : 1);
            if (x) {
                system("cls");
                goto menu;
            }
        }
        system("cls");
        printf("\n\n\n******** TERMINATED ********\n\n\n");
        return EXIT_SUCCESS;

    }

        /** defualt. ****************************************************************************************************************/
    else
        printf("\nInvalid ! , retry.\n\n");
    goto menuRechoose;
}

/************* SAVE ****************/
int save() {
    int i = 0;
    {
        f = fopen("contacts.txt", "w");
        if (f == NULL) {
            printf("Error 404 : File not found.\n");
            return 0;
        }
        for (i = 0; i < records; i++)
            fprintf(f, "%s,%s,%s,%d,%s,%lld,%s\n", data[i].firstName,
                    data[i].lastName, data[i].location.street,
                    data[i].location.flat, data[i].location.city, data[i].pn,
                    data[i].email);

        fclose(f);
        saveFlag = 0;
        printf("All contacts saved successfully !\n");
    }

    return 1;
}

/*************** LOAD ***************/
int load() {
    f = fopen("contacts.txt", "r+");
    if (f == NULL) {
        printf("Error 404 : File not found.\n");
        f = fopen("contacts.txt", "w+");
        if (f == NULL) {
            printf("Can't Creat new file");
            return 0;
        } else {
            printf("New Clear File created\n"
                           "File directory Project Folder\n");
            return 1;
        }
    }
    int i = 0;
    while (fscanf(f,
                  "%30[^,],%30[^,],%50[^,],%d,%30[^,],%lld,%s\n", // change was made here.
                  data[i].firstName, data[i].lastName, data[i].location.street,
                  &data[i].location.flat, data[i].location.city, &data[i].pn,
                  data[i].email) != EOF) {
        i++;           // i refers to number of elements read from the file.
    }
    records = i;
    return i;                                       // change was made here.
}

/************** PRINT **************/
void printEntries(int j) {
    int i = 0;                             // change was made here.
    while (i < j) {
        printf("%-4d -->  %s,%s,%s,%d,%s,%lld,%s.\n", i + 1, data[i].firstName,
               data[i].lastName, data[i].location.street,
               data[i].location.flat, data[i].location.city, data[i].pn,
               data[i].email);
        i++;
    }
}

/************* SEARCH **************/

int search(char a[], int i) {
    searchNum = 0;
    int j = 0;
    while (i >= j) {
        if (!strcmp(a, data[j].firstName)) {
            searchResults[searchNum] = j;
            searchNum++;
        }
        j++;
    }
    return searchNum;
}

/************** ADD ****************/
int addRecord(void) {
    contact newrecord;
    int exist = 0;

    do {
        if (exist == 1)
            printf(
                    "a previous contact already exist with the same first and last name\n");
        getFirstName(&newrecord);
        getLastName(&newrecord);
        exist = isExist(newrecord.firstName, newrecord.lastName);
    } while (exist == 1);

    getAddress(&newrecord);
    getFlatNumber(&newrecord);
    getCity(&newrecord);
    getPhoneNumber(&newrecord);
    getEmail(&newrecord);

    insertRecord(newrecord); //insert the newrecord in the correct place

    printf("the following contact has been created:\n");
    printRecord(&newrecord);
    saveFlag = 1;
    return EXIT_SUCCESS;
}

/************** MODIFY *************/
int modifyRecord(int index) {
    int nameModFlag = 0;
    /*a flag indicating if the first or last name has been modified
     *which might cause a contact's alphabetical position in the list to change
     */
    contact tempcontact = data[index];
    printf("do you want to modify the contact's first name? (Y/N)\n");
    if (getChoice()) {
        getFirstName(&tempcontact);
        nameModFlag = 1;
    }
    printf("do you want to modify the contact's last name? (Y/N)\n");
    if (getChoice()) {
        getLastName(&tempcontact);
        nameModFlag = 1;
        saveFlag = 1;
    }

    if (isExist(tempcontact.firstName, tempcontact.lastName)
        && nameModFlag == 1) {
        printf(
                "a previous contact already exist with the same first and last name\n\
               Aborting modification\nPress any key to go to main menu\n");
        getchar();
        return 0;
    }

    printf("do you want to modify the contact's address? (Y/N)\n");
    if (getChoice()) {
        getAddress(&tempcontact);
        saveFlag = 1;
    }
    printf("do you want to modify the contact's flat number? (Y/N)\n");
    if (getChoice()) {
        getFlatNumber(&tempcontact);
        saveFlag = 1;
    }
    printf("do you want to modify the contact's city? (Y/N)\n");
    if (getChoice()) {
        getCity(&tempcontact);
        saveFlag = 1;
    }
    printf("do you want to modify the contact's Phone Number? (Y/N)\n");
    if (getChoice()) {
        getPhoneNumber(&tempcontact);
        saveFlag = 1;
    }
    printf("do you want to modify the contact's E-mail? (Y/N)\n");
    if (getChoice()) {
        getEmail(&tempcontact);
        saveFlag = 1;
    }
    printf("modification complete:\nupdated data:\n");
    printRecord(&tempcontact);
    if (nameModFlag == 1) {
        //delete the original contact
        int i;
        for (i = index; i < records - 1; i++) {
            data[i] = data[i + 1];
        }
        records--; //reduce the number of records
        //insert the modified contact in its correct place
        insertRecord(tempcontact);
    }
//   else if (nameModFlag == 0) //in case the user didn't change the contact's original name
    {
        //just update the contact's data by equating structures of original contact and temp contact
        data[index] = tempcontact;
    }

    return EXIT_SUCCESS; //modification complete!!
}

/******************** HELPING FUNCTIONS ********************/
int validateName(char *str, int maxlength) {
//this function checks if the name entered contains only alphabetical characters and has suitable length
    int i;
    int length = strlen(str);

    if (length >= maxlength) {
        printf("input is too long\n");
        return 1; // 1 for unsuitable length
    }

    for (i = 0; i < length; i++) {
        if (!isalpha(str[i])) {
            printf("input is invalid\n");
            return 2; //2 for invalid name
        }

    }

    printf("input accepted\n");
    return 0; //valid
}

int validateAddress(char *str) {
//address should be of suitable length and should start with a number
    int length = strlen(str);

    if (length >= STREET) {
        printf("Address is too long\n");
        return 1;
    }

    printf("Address accepted\n");
    return 0; //valid
}

int validateNumber(char *str, int maxlength) {
    int length = strlen(str);
    int i;

    if (length > maxlength) {
        printf("flat number is too long\n");
        return 1; //1 for unsuitable length
    }

    for (i = 0; i < length; i++) //checks if all the charaters are digits
    {
        if (!isdigit(str[i])) {
            printf("invalid input\n");
            return 2; //2 for invalid number
        }
    }

    printf("number accepted\n");
    return 0; //valid
}

void printRecord(contact *record) {
    printf("%s,", record->firstName);
    printf("%s,", record->lastName);
    printf("%s,", record->location.street);
    printf("%d,", record->location.flat);
    printf("%s,", record->location.city);
    printf("%lld,", record->pn);
    printf("%s.\n\n", record->email);
}

int validateEmail(char *str) {
    int length = strlen(str);
    int i, j;

    if (length >= EMAIL) {
        printf("E-mail is too long\n");
        return 1;
    }
    int spaceFlag = 0; // space in emial adress causes   OVERFLOW    T_T
    for (i = 0; i < length; i++) {
        if (str[i] == ' ') {
            spaceFlag = 1;
        }
        if (spaceFlag == 1) {
            return 2;
        }
    }
//look for '@' and '.' somewhere after it
    int dotflag = 0, atflag = 0;
    for (i = 0; i < length; i++) {
        if (str[i] == '@') {
            atflag = 1;
        }
        if (atflag == 1) {
            for (j = i + 2; j < length; j++) {
                if (str[j] == '.')
                    dotflag = 1;
            }
        }
    }

    if (!dotflag || !atflag) {
        printf("Invalid E-mail address format\n");
        return 2;
    }

    printf("E-mail address accepted\n");
    return 0;
}

void getFirstName(contact *pointer) {
    int validation;
    char buffer[40];
    do {
        printf("enter the new contact's first name:\n");
        scanf("%39s", buffer);
        fflush(stdin);

        validation = validateName(buffer, NAME);
        if (validation == 0)
            strcpy(pointer->firstName, buffer);
    } while (validation != 0);
}

void getLastName(contact *pointer) {
    int validation;
    char buffer[40];
    do {
        printf("enter the new contact's last name:\n");
        scanf("%39s", buffer);
        fflush(stdin);

        validation = validateName(buffer, NAME);
        if (validation == 0)
            strcpy(pointer->lastName, buffer);
    } while (validation != 0);
}

void getAddress(contact *pointer) {
    int validation;
    char buffer[100];
    do {
        printf("enter the new contact's street address:\n");
        scanf("%99[^\n]", buffer);
        fflush(stdin);

        validation = validateAddress(buffer);
        if (validation == 0)
            strcpy(pointer->location.street, buffer);
    } while (validation != 0);
}

void getFlatNumber(contact *pointer) {
    int validation;
    char buffer[100];
    do {
        printf("enter the contact's flat number:\n");
        scanf("%99s", buffer);
        fflush(stdin);

        validation = validateNumber(buffer, 3);
        if (validation == 0)
            pointer->location.flat = atoi(buffer);
    } while (validation != 0);
}

void getCity(contact *pointer) {
    int validation;
    char buffer[50];
    do {
        printf("enter the contact's city:\n");
        scanf("%49s", buffer);
        fflush(stdin);

        validation = validateName(buffer, CITY);
        if (validation == 0)
            strcpy(pointer->location.city, buffer);
    } while (validation != 0);
}

void getPhoneNumber(contact *pointer) {
    int validation;
    char buffer[30];
    do {
        printf("enter the contact's phone number:\n");
        scanf("%29s", buffer);
        fflush(stdin);

        validation = validateNumber(buffer, NUMBER);
        if (validation == 0)
            pointer->pn = atoll(buffer);
    } while (validation != 0);
}

void getEmail(contact *pointer) {
    int validation;
    char buffer[60];
    do {
        printf("enter the contact's E-mial address:\n");
        scanf("%59s", buffer);
        fflush(stdin);

        validation = validateEmail(buffer);
        if (validation == 0)
            strcpy(pointer->email, buffer);
    } while (validation != 0);
}

int getChoice(void) {
    char choice;
    do {
        choice = (char) getchar();
        fflush(stdin);
        if (!(ChoiceCondition))
            printf("Invalid input\n(Y/N)?\n");
    } while (!(ChoiceCondition));

    if (choice == 'Y' || choice == 'y')
        return 1;
    else
        return 0;
}

void insertRecord(contact newContact) {
    char tempName[60]; //string to hold the name of the current contact to compare (first + last)
    char tempRecord[60]; //string to hold the name of the new record (first + last)
    int i, found = 0, location;
    strcpy(tempRecord, newContact.firstName);
    strcat(tempRecord, " ");
    strcat(tempRecord, newContact.lastName); //the new record's name is now ready

    for (i = 0; i < records; i++) {
        strcpy(tempName, data[i].firstName);
        strcat(tempName, " ");
        strcat(tempName, data[i].lastName);

        if (strcmp(tempRecord, tempName) < 0) {
            //the correct location for the new record has been found
            found = 1;
            //shift all the following contacts by one place
            //then place the new record in the empty place
            location = i; //remember the correct location of the new contact

            for (i = records - 1; i >= location; i--) //shift
                data[i + 1] = data[i];

            data[location] = newContact;
            records++; //increment the number of records in the phone book
            break;
        }
    }
    if (!found) //no correct location has been found for the new record, then place it at the end of the list
    {
        location = records; //the first empty location after the phone book
        data[location] = newContact;
        records++;
    }
}

int isExist(char *firstname, char *lastname) {
    int i;
    for (i = 0; i < records; i++) {
        if ((strcmp(data[i].firstName, firstname) == 0)
            && (strcmp(data[i].lastName, lastname) == 0))
            return 1; // a contact already exist with the same first and last name
    }
    return 0; //no previous contact already exist with the same first and last name
}

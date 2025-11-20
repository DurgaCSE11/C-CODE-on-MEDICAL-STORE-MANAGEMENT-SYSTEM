#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // For generating timestamps

// ======== STRUCT DEFINITION ========
struct Medicine {
    int id;
    char name[50];
    char company[50];
    float price;
    int quantity;
    char mfgDate[20];
    char expDate[20];
    char info[100];
};

// ======== FUNCTION PROTOTYPES ========
void addMedicine();
void displayMedicines();
void searchMedicine();
void updateMedicine();
void deleteMedicine();
void sellMedicine();
void displaySales();       // Shows the sales_records.csv log
void viewSavedBill();      // NEW: Shows a .txt bill on the console
void printSavedBill();     // NEW: Prints a .txt bill to a printer
void recordSale(struct Medicine med, int quantitySold, float totalAmount, char* customerName, char* customerPhone);

// Helper functions
void consumeNewline();
void readString(char *buffer, int size);
void ensureHeader(FILE *fp, const char* filename, const char* header);

// ======== FILENAMES & DIRECTORIES ========
const char *MEDICINE_FILE = "medicine_data.csv";
const char *SALES_FILE = "sales_records.csv";
const char *BILL_DIRECTORY = "E:\\0. VSSUT ACADEMICS\\1ST YEAR\\1ST SEM PROJECTS\\CDS\\BILLS";


// ======== MAIN FUNCTION ========
int main() {
    int choice;
    while (1) {
        printf("\n==== MEDICAL STORE MANAGEMENT SYSTEM ====\n");
        printf("1. Add Medicine\n");
        printf("2. Display All Medicines\n");
        printf("3. Search Medicine by ID\n");
        printf("4. Update Medicine\n");
        printf("5. Delete Medicine\n");
        printf("6. Sell Medicine\n");
        printf("7. View Sales Records (CSV)\n");
        printf("8. View Saved Bill (TXT)\n");      // NEW
        printf("9. Print Saved Bill (TXT)\n");     // NEW
        printf("10. Exit\n");                     // Updated
        printf("========================================\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            consumeNewline(); // Clear bad input
            continue;
        }
        consumeNewline(); // FIXED: Consume the newline left by scanf

        switch (choice) {
            case 1: addMedicine(); break;
            case 2: displayMedicines(); break;
            case 3: searchMedicine(); break;
            case 4: updateMedicine(); break;
            case 5: deleteMedicine(); break;
            case 6: sellMedicine(); break;
            case 7: displaySales(); break;
            case 8: viewSavedBill(); break;      // NEW
            case 9: printSavedBill(); break;     // NEW
            case 10:                          // Updated
                printf("\nThank you for using the Medical Store Management System!\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// ======== HELPER FUNCTIONS ========

void consumeNewline() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void readString(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void ensureHeader(FILE *fp, const char* filename, const char* header) {
    FILE *check = fopen(filename, "r");
    if (check == NULL) {
        fprintf(fp, "%s\n", header);
        return;
    }
    fseek(check, 0, SEEK_END);
    long size = ftell(check);
    fclose(check);
    if (size == 0) {
        fprintf(fp, "%s\n", header);
    }
}


// ======== MEDICINE FUNCTIONS (CRUD) ========

//  Add Medicine
void addMedicine() {
    struct Medicine med;
    FILE *fp = fopen(MEDICINE_FILE, "a");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    
    const char *header = "ID,Name,Company,Price,Quantity,MfgDate,ExpDate,Info";
    ensureHeader(fp, MEDICINE_FILE, header);

    printf("\n--- Enter New Medicine Details ---\n");
    printf("ID: ");
    scanf("%d", &med.id); consumeNewline();
    printf("Name: ");
    readString(med.name, 50);
    printf("Company: ");
    readString(med.company, 50);
    printf("Price: ");
    scanf("%f", &med.price); consumeNewline();
    printf("Quantity: ");
    scanf("%d", &med.quantity); consumeNewline();
    printf("Mfg Date (dd-mm-yyyy): ");
    readString(med.mfgDate, 20);
    printf("Exp Date (dd-mm-yyyy): ");
    readString(med.expDate, 20);
    printf("Info: ");
    readString(med.info, 100);

    fprintf(fp, "%d,%s,%s,%.2f,%d,%s,%s,%s\n",
            med.id, med.name, med.company, med.price,
            med.quantity, med.mfgDate, med.expDate, med.info);
    fclose(fp);
    printf("\n Medicine added successfully!\n");
}

//  Display All Medicines
void displayMedicines() {
    FILE *fp = fopen(MEDICINE_FILE, "r");
    if (!fp) {
        printf("No records found! (File: %s)\n", MEDICINE_FILE);
        return;
    }
    char line[300];
    printf("\n=========== MEDICINE LIST ===========\n");
    if(fgets(line, sizeof(line), fp)) {
        printf("** %s **", line);
    }
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    printf("=====================================\n");
    fclose(fp);
}

//  Search Medicine by ID
void searchMedicine() {
    FILE *fp = fopen(MEDICINE_FILE, "r");
    if (!fp) {
        printf("No records found!\n");
        return;
    }
    int searchID, found = 0;
    struct Medicine med;
    char line[300];
    printf("\nEnter Medicine ID to search: ");
    scanf("%d", &searchID);
    consumeNewline();

    fgets(line, sizeof(line), fp); // skip header
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%49[^,],%49[^,],%f,%d,%19[^,],%19[^,],%99[^\n]",
               &med.id, med.name, med.company, &med.price, &med.quantity,
               &med.mfgDate, &med.expDate, &med.info);

        if (med.id == searchID) {
            printf("\n--- Medicine Found ---\n");
            printf("ID: %d\nName: %s\nCompany: %s\nPrice: %.2f\nQuantity: %d\nMfg: %s\nExp: %s\nInfo: %s\n",
                   med.id, med.name, med.company, med.price,
                   med.quantity, med.mfgDate, med.expDate, med.info);
            found = 1;
            break;
        }
    }
    if (!found) printf("\n Medicine not found!\n");
    fclose(fp);
}

//  Update Medicine
void updateMedicine() {
    FILE *fp = fopen(MEDICINE_FILE, "r");
    if (!fp) { printf("No records found!\n"); return; }
    FILE *temp = fopen("temp.csv", "w");
    if (!temp) { printf("Error creating temp file!\n"); fclose(fp); return; }

    int updateID, found = 0;
    struct Medicine med;
    char line[300];
    printf("\nEnter Medicine ID to update: ");
    scanf("%d", &updateID); consumeNewline();

    fgets(line, sizeof(line), fp);
    fprintf(temp, "%s", line); 

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%49[^,],%49[^,],%f,%d,%19[^,],%19[^,],%99[^\n]",
               &med.id, med.name, med.company, &med.price, &med.quantity,
               med.mfgDate, med.expDate, med.info);

        if (med.id == updateID) {
            found = 1;
            printf("--- Enter new details for Medicine ID %d (Current: %s) ---\n", med.id, med.name);
            printf("New Name: "); readString(med.name, 50);
            printf("New Company: "); readString(med.company, 50);
            printf("New Price: "); scanf("%f", &med.price); consumeNewline();
            printf("New Quantity: "); scanf("%d", &med.quantity); consumeNewline();
            printf("New Mfg Date: "); readString(med.mfgDate, 20);
            printf("New Exp Date: "); readString(med.expDate, 20);
            printf("New Info: "); readString(med.info, 100);
            printf("--------------------------------------------------\n");
        }
        fprintf(temp, "%d,%s,%s,%.2f,%d,%s,%s,%s\n",
                med.id, med.name, med.company, med.price,
                med.quantity, med.mfgDate, med.expDate, med.info);
    }
    fclose(fp); fclose(temp);
    remove(MEDICINE_FILE);
    rename("temp.csv", MEDICINE_FILE);
    if (found) printf("\n Medicine updated successfully!\n");
    else printf("\n Medicine ID not found!\n");
}

//  Delete Medicine
void deleteMedicine() {
    FILE *fp = fopen(MEDICINE_FILE, "r");
    if (!fp) { printf("No records found!\n"); return; }
    FILE *temp = fopen("temp.csv", "w");
    if (!temp) { printf("Error creating temp file!\n"); fclose(fp); return; }

    int deleteID, found = 0;
    struct Medicine med;
    char line[300];
    printf("\nEnter Medicine ID to delete: ");
    scanf("%d", &deleteID); consumeNewline();

    fgets(line, sizeof(line), fp);
    fprintf(temp, "%s", line); 

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%49[^,],%49[^,],%f,%d,%19[^,],%19[^,],%99[^\n]",
               &med.id, med.name, med.company, &med.price, &med.quantity,
               med.mfgDate, med.expDate, med.info);

        if (med.id != deleteID) {
            fprintf(temp, "%d,%s,%s,%.2f,%d,%s,%s,%s\n",
                    med.id, med.name, med.company, med.price,
                    med.quantity, med.mfgDate, med.expDate, med.info);
        } else {
            found = 1;
        }
    }
    fclose(fp); fclose(temp);
    remove(MEDICINE_FILE);
    rename("temp.csv", MEDICINE_FILE);
    if (found) printf("\n Medicine (ID: %d) deleted successfully!\n", deleteID);
    else printf("\n Medicine ID not found!\n");
}

// ======== SALES & BILLING FUNCTIONS ========

/**
 * @brief Records a sale to the CSV log AND saves a unique, formatted
 * .txt bill file inside the "BILLS" folder.
 */
void recordSale(struct Medicine med, int quantitySold, float totalAmount, char* customerName, char* customerPhone) {
    
    // --- 1. Save to the main CSV log (as before) ---
    FILE *fp = fopen(SALES_FILE, "a");
    if (!fp) {
        printf("CRITICAL ERROR: Could not open sales file (%s)!\n", SALES_FILE);
    } else {
        const char *header = "CustomerName,CustomerPhone,MedicineID,MedicineName,Quantity,TotalAmount";
        ensureHeader(fp, SALES_FILE, header);
        fprintf(fp, "%s,%s,%d,%s,%d,%.2f\n",
                customerName, customerPhone, med.id, med.name, quantitySold, totalAmount);
        fclose(fp);
    }
    
    // --- 2. Generate a unique filename ---
    char billFilename[500]; // Increased size for long path
    char timestamp[20];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", t);
    
    // Create the full filename, e.g., "E:\...BILLS\John Doe_20251102_183000.txt"
    sprintf(billFilename, "%s\\%s_%s.txt", BILL_DIRECTORY, customerName, timestamp);

    // --- 3. Save the formatted bill to the unique file ---
    FILE *billFile = fopen(billFilename, "w");
    if (billFile == NULL) {
        printf("\nCRITICAL ERROR: Could not create bill file at %s\n", billFilename);
        printf("Please ensure the directory exists and you have write permissions.\n");
    } else {
        fprintf(billFile, "   *** MEDICAL STORE BILL ***\n\n");
        fprintf(billFile, "==================================\n");
        fprintf(billFile, "Customer: %s\n", customerName);
        fprintf(billFile, "Phone:    %s\n", customerPhone);
        fprintf(billFile, "==================================\n\n");
        fprintf(billFile, "Item:       %s (ID: %d)\n", med.name, med.id);
        fprintf(billFile, "Quantity:   %d\n", quantitySold);
        fprintf(billFile, "Price/unit: %.2f\n", med.price);
        fprintf(billFile, "----------------------------------\n");
        fprintf(billFile, "TOTAL AMOUNT: %.2f\n\n", totalAmount);
        fprintf(billFile, "   Thank you for your purchase!\n");
        fclose(billFile);
        printf("\n Bill saved successfully to: %s\n", billFilename);
    }
    
    // --- 4. Print bill to console (as before) ---
    printf("\n--- BILL GENERATED (Console Copy) ---\n");
    printf("Customer: %s (%s)\n", customerName, customerPhone);
    printf("Item: %s (ID: %d)\n", med.name, med.id);
    printf("Quantity: %d\n", quantitySold);
    printf("Price per unit: %.2f\n", med.price);
    printf("---------------------------------------\n");
    printf("TOTAL AMOUNT: %.2f\n", totalAmount);
    printf("---------------------------------------\n");
}

/**
 * @brief Main function to sell a medicine.
 */
void sellMedicine() {
    FILE *fp = fopen(MEDICINE_FILE, "r");
    if (!fp) { printf("No medicine records found! Cannot perform sale.\n"); return; }
    FILE *temp = fopen("temp.csv", "w");
    if (!temp) { printf("Error creating temp file!\n"); fclose(fp); return; }

    int sellID, sellQuantity, found = 0, stockOK = 0, saleMade = 0;
    struct Medicine med;
    char line[300], customerName[50], customerPhone[20];

    printf("\n--- SELL MEDICINE ---\n");
    printf("Enter Medicine ID to sell: ");
    scanf("%d", &sellID); consumeNewline();
    printf("Enter Quantity to sell: ");
    scanf("%d", &sellQuantity); consumeNewline();

    if (sellQuantity <= 0) {
        printf("Invalid quantity. Sale cancelled.\n");
        fclose(fp); fclose(temp); remove("temp.csv");
        return;
    }

    fgets(line, sizeof(line), fp); // Copy header
    fprintf(temp, "%s", line); 

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%49[^,],%49[^,],%f,%d,%19[^,],%19[^,],%99[^\n]",
               &med.id, med.name, med.company, &med.price, &med.quantity,
               med.mfgDate, med.expDate, med.info);

        if (med.id == sellID) {
            found = 1;
            if (med.quantity >= sellQuantity) {
                stockOK = 1;
                printf("Stock available (%d units). Enter customer details:\n", med.quantity);
                printf("Customer Name: "); readString(customerName, 50);
                printf("Customer Phone: "); readString(customerPhone, 20);

                med.quantity -= sellQuantity;
                float totalAmount = med.price * sellQuantity;
                recordSale(med, sellQuantity, totalAmount, customerName, customerPhone);
                saleMade = 1;

            } else {
                printf("Error: Insufficient stock for %s. Only %d units available.\n", med.name, med.quantity);
            }
        }
        fprintf(temp, "%d,%s,%s,%.2f,%d,%s,%s,%s\n",
                med.id, med.name, med.company, med.price,
                med.quantity, med.mfgDate, med.expDate, med.info);
    }

    fclose(fp); fclose(temp);
    remove(MEDICINE_FILE);
    rename("temp.csv", MEDICINE_FILE);

    if (!found) printf("\n Medicine ID not found! Sale cancelled.\n");
    else if (found && stockOK && saleMade) printf("\n Sale successful! Inventory updated.\n");
    else printf("\n Sale cancelled.\n");
}

/**
 * @brief Displays all records from the sales_records.csv file.
 */
void displaySales() {
    FILE *fp = fopen(SALES_FILE, "r");
    if (!fp) {
        printf("No sales records found! (File: %s)\n", SALES_FILE);
        return;
    }
    char line[300];
    printf("\n=========== SALES RECORDS (CSV LOG) ===========\n");
    if(fgets(line, sizeof(line), fp)) {
        printf("** %s **", line);
    }
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    printf("===============================================\n");
    fclose(fp);
}


// ======== NEW FUNCTIONS (VIEW & PRINT) ========

/**
 * @brief Lists bills, then shows the content of a selected .txt bill on the console.
 */
void viewSavedBill() {
    char command[500];
    char filename[100];
    char fullPath[600];
    
    // 1. List all available .txt bills in the directory
    printf("\n--- Available Bills in %s ---\n", BILL_DIRECTORY);
    // Use 'dir /b' for a bare list of files. Quotes handle spaces in the path.
    sprintf(command, "dir \"%s\\*.txt\" /b", BILL_DIRECTORY);
    system(command);
    printf("--------------------------------------------------\n");

    // 2. Ask user for the filename
    printf("Enter the full filename to VIEW (e.g., John_Doe_...txt): ");
    readString(filename, 100);

    // 3. Construct the full file path
    sprintf(fullPath, "%s\\%s", BILL_DIRECTORY, filename);

    // 4. Open, read, and print the file content to console
    FILE *fp = fopen(fullPath, "r");
    if (fp == NULL) {
        printf("\n Error: Cannot open file %s\n", fullPath);
        printf("Check if the filename is correct and the file exists.\n");
        return;
    }

    printf("\n--- Displaying Bill: %s ---\n", filename);
    char c;
    while ((c = fgetc(fp)) != EOF) {
        putchar(c); // Print one character at a time
    }
    printf("\n--- End of Bill ---\n");
    fclose(fp);
}

/**
 * @brief Lists bills, then sends a selected .txt bill to the default printer.
 */
void printSavedBill() {
    char listCommand[500];
    char printCommand[600];
    char filename[100];
    char fullPath[600];
    
    // 1. List all available .txt bills in the directory
    printf("\n--- Available Bills in %s ---\n", BILL_DIRECTORY);
    sprintf(listCommand, "dir \"%s\\*.txt\" /b", BILL_DIRECTORY);
    system(listCommand);
    printf("--------------------------------------------------\n");

    // 2. Ask user for the filename
    printf("Enter the full filename to PRINT (e.g., John_Doe_...txt): ");
    readString(filename, 100);

    // 3. Construct the full file path
    sprintf(fullPath, "%s\\%s", BILL_DIRECTORY, filename);

    // 4. Check if file exists before trying to print
    FILE *fp = fopen(fullPath, "r");
    if (fp == NULL) {
        printf("\n Error: Cannot find file %s\n", fullPath);
        printf("Printing cancelled.\n");
        return;
    }
    fclose(fp); // Just checking for existence, so close it.
    // 5. Construct and execute the print command
    // "notepad /p [filename]" is the Windows command to print a text file
    // We add quotes ( \" ) around the path to handle any spaces.
    sprintf(printCommand, "notepad /p \"%s\"", fullPath);
    printf("\nSending %s to default printer...\n", filename);
    system(printCommand);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Product
{
    int productID;
    char *productName;
    float productPrice;
    int productQuantity;
} Product;

void printMenu()
{
    printf("\n=========== Inventory Menu ===========\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
}

Product getProductDetails()
{
    Product tempProduct;
    char scannerBuffer[100];
    int scannedItems;
    char bufferChar;

    do {
        printf("Enter Product ID (1-10000): ");
        fgets(scannerBuffer, sizeof(scannerBuffer), stdin);
        scannedItems = sscanf(scannerBuffer, "%d %c", &tempProduct.productID, &bufferChar);

        if ((scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n')) && tempProduct.productID >= 1 && tempProduct.productID <= 10000) {
            break;
        } else {
            printf("Please enter only a number between 1 and 10000.\n");
        }
    } while (1);

    do {
        printf("Enter Product Name (1-50 characters): ");
        fgets(scannerBuffer, sizeof(scannerBuffer), stdin);
        scannerBuffer[strcspn(scannerBuffer, "\n")] = '\0';
        if (strlen(scannerBuffer) >= 1 && strlen(scannerBuffer) <= 50) {
            tempProduct.productName = (char*)malloc(strlen(scannerBuffer) + 1);
            strcpy(tempProduct.productName, scannerBuffer);
            break;
        } else {
            printf("Product name must be between 1 and 50 characters long.\n");
        }
    } while (1);

    do {
        printf("Enter Product Price (0-100000): ");
        fgets(scannerBuffer, sizeof(scannerBuffer), stdin);
        scannedItems = sscanf(scannerBuffer, "%f %c", &tempProduct.productPrice, &bufferChar);
    
        if ((scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n')) && tempProduct.productPrice >= 0 && tempProduct.productPrice <= 100000) {
            break;
        } else {
            printf("Please enter only a number between 0 and 100000.\n");
        }
    } while (1);

    do {
        printf("Enter Product Quantity (0-1000000): ");
        fgets(scannerBuffer, sizeof(scannerBuffer), stdin);
        scannedItems = sscanf(scannerBuffer, "%d %c", &tempProduct.productQuantity, &bufferChar);
        if ((scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n')) && tempProduct.productQuantity >= 0 && tempProduct.productQuantity <= 1000000) {
            break;
        } else {
            printf("Please enter only a number between 0 and 1000000.\n");
        }
    } while (1);
    
    return tempProduct;
}

void printProductDetails(Product currentProduct){
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", currentProduct.productID, currentProduct.productName, currentProduct.productPrice, currentProduct.productQuantity);
}

void viewAllProducts(Product* productsArray, int productArraySize){
    printf("\n--- Product List ---\n");
    if (productArraySize == 0) {
        printf("Inventory is empty.\n");
        return;
    }
    for(int productIndex = 0; productIndex < productArraySize ; productIndex++){
        printProductDetails(productsArray[productIndex]);
    }
}

Product* searchProductByID(Product* productsArray, int productsArraysize){
    char inputBuffer[20];
    int targetProductID;
    char bufferChar;
    
    if (productsArraysize == 0) {
        return NULL;
    }

    printf("Enter the Product ID to search: ");
    do{
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        int scannedItems = sscanf(inputBuffer, "%d %c", &targetProductID, &bufferChar);
        if((scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n')) && targetProductID >= 1 && targetProductID <= 10000){
            break;
        } else {
            fprintf(stderr , "Please enter a valid product ID: ");
        }
    } while (1);

    for (int productIndex = 0; productIndex < productsArraysize; productIndex++) {
        if(productsArray[productIndex].productID == targetProductID){
            return &productsArray[productIndex];
        }
    }
    return NULL;
}

void updateProductQuantity(Product* productsArray, int productsArraysize){
    Product* productToUpdate = searchProductByID(productsArray, productsArraysize);

    if (productToUpdate != NULL) {
        char inputBuffer[20];
        int newQuantity;
        char bufferChar;
        do {
            printf("Enter the new product quantity (0-1000000): ");
            fgets(inputBuffer, sizeof(inputBuffer), stdin);
            int scannedItems = sscanf(inputBuffer, "%d %c", &newQuantity, &bufferChar);
            if ((scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n')) && newQuantity >= 0 && newQuantity <= 1000000) {
                productToUpdate->productQuantity = newQuantity;
                printf("Quantity updated successfully \n");
                printProductDetails(*productToUpdate);
                return;
            } else {
                fprintf(stderr, "Invalid quantity.\n");
            }
        } while (1);
    } else {
        printf("Product with that ID was not found.\n");
    }
}

int deleteProduct(Product** productsArray, int* productsArraysize) {
    char inputBuffer[20];
    int targetProductID;
    char bufferChar;
    int productFound = 0; 
    int productIndexToDelete = -1;

    if (*productsArraysize == 0) {
        printf("Inventory is empty.\n");
        return 0;
    }

    do {
        printf("\nEnter Product ID to Delete: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        int scannedItems = sscanf(inputBuffer, "%d %c", &targetProductID, &bufferChar);
        if ((scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n')) && targetProductID >= 1 && targetProductID <= 10000) {
            break;
        } else {
            fprintf(stderr, "Please enter a valid product ID.\n");
        }
    } while (1);

    for (int i = 0; i < *productsArraysize; i++) {
        if ((*productsArray)[i].productID == targetProductID) {
            productFound = 1;
            productIndexToDelete = i;
            break;
        }
    }

    if (productFound) {
        free((*productsArray)[productIndexToDelete].productName);
        for (int i = productIndexToDelete; i < (*productsArraysize - 1); i++) {
            (*productsArray)[i] = (*productsArray)[i + 1];
        }

        if (*productsArraysize == 1) {
            free(*productsArray);
            *productsArray = NULL;
        } else {
            Product* tempArray = realloc(*productsArray, (*productsArraysize - 1) * sizeof(Product));
            if (tempArray == NULL && (*productsArraysize - 1) > 0) {
                printf("Memory reallocation failed during delete\n");
                return 0;
            }
            *productsArray = tempArray;
        }
        (*productsArraysize)--;
        printf("Product deleted successfully\n");
        return 1;
    }
    
    printf("Product with ID %d not found.\n", targetProductID);
    return 0;
}

void addNewProduct(Product** productsArray, int* productsArraysize){
    printf("\n--- Add New Product ---\n");
    Product tempProduct = getProductDetails();
    Product* tempArray = realloc(*productsArray, (*productsArraysize + 1) * sizeof(Product));

    if (tempArray == NULL) {
        printf("Memory reallocation failed\n");
        free(tempProduct.productName);
        return;
    }
    *productsArray = tempArray;
    (*productsArray)[*productsArraysize] = tempProduct;
    (*productsArraysize)++;
    printf("Product added successfully\n");
}

void searchProductByName(Product* productsArray, int productsArraysize){
    char inputBuffer[52];
    int foundCount = 0;

    printf("\nEnter name to search (partial match allowed): ");
    fgets(inputBuffer,sizeof(inputBuffer),stdin);
    inputBuffer[strcspn(inputBuffer,"\n")] = '\0';

    if(strlen(inputBuffer) == 0){
        fprintf(stderr,"Name cannot be empty.\n");
        return;
    }
    
    printf("\n--- Products Found ---\n");
    for (int productIndex = 0; productIndex < productsArraysize; productIndex++) {
        if (strstr(productsArray[productIndex].productName, inputBuffer) != NULL) {
            printProductDetails(productsArray[productIndex]);
            foundCount++;
        }
    }
    if (foundCount == 0) {
        printf("No products found with the name containing \"%s\".\n", inputBuffer);
    }
}

void searchProductByPriceRange(Product* productsArray, int productArraySize){
    char inputbuffer[50];
    char bufferChar;
    float startingRange;
    float endingRange;
    int productsFound = 0;

    do {
        printf("Enter minimum price (0-100000): ");
        fgets(inputbuffer,sizeof(inputbuffer),stdin);
        int scannedItem = sscanf(inputbuffer , "%f %c", &startingRange , &bufferChar);
        if((scannedItem == 1 || (scannedItem == 2 && bufferChar == '\n')) && startingRange >= 0 && startingRange <= 100000){
            break;
        }else{
            fprintf(stderr ,"Please Enter a valid Minimum Price.\n");
        }
    } while (1);

    do {
        printf("Enter maximum price (0-100000): ");
        fgets(inputbuffer,sizeof(inputbuffer),stdin);
        int scannedItem = sscanf(inputbuffer , "%f %c", &endingRange , &bufferChar);
        if((scannedItem == 1 || (scannedItem == 2 && bufferChar == '\n')) && endingRange >= 0 && endingRange <= 100000 && startingRange < endingRange){
            break;
        }else{
            fprintf(stderr ,"Please Enter a valid Maximum Price (must not be less than minimum).\n");
        }
    } while (1);
    
    printf("\n--- Products in Price Range ---\n");
    for(int productIndex=0; productIndex < productArraySize; productIndex++){
        if(productsArray[productIndex].productPrice >= startingRange && productsArray[productIndex].productPrice <= endingRange){
            printProductDetails(productsArray[productIndex]);
            productsFound++;
        }
    }
    if(productsFound == 0){
        printf("No Products found in the given range %.2f to %.2f\n", startingRange, endingRange);
    }
}

int main()
{
    printf("Enter the initial number of products (1-100): ");
    int initialProductCount;
    char inputBuffer[20];
    char bufferChar;
    int productArraySize = 0;

    fgets(inputBuffer , sizeof(inputBuffer) , stdin);
    int scannedItems = sscanf(inputBuffer ,"%d %c" , &initialProductCount , &bufferChar);
    
    if (!((scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n')) && initialProductCount >= 1 && initialProductCount <= 100))
    {
        fprintf(stderr, "Please enter a number between 1 and 100.\n");
        return 1;
    }
    
    Product *productArray = calloc(initialProductCount, sizeof(Product));
    if (productArray == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed\n");
        return 1;
    }
    productArraySize = initialProductCount;

    for (int productIndex = 0; productIndex < initialProductCount; productIndex++)
    {
        printf("\nEnter details for product %d:\n", productIndex + 1);
        productArray[productIndex] = getProductDetails();
    }
    
    while (1)
    {
        int selectedOption;
        char menuInputBuffer[10];
        printMenu();
        printf("Enter your choice: ");
        fgets(menuInputBuffer, sizeof(menuInputBuffer), stdin);
        int menuScannedItems = sscanf(menuInputBuffer, "%d %c", &selectedOption, &bufferChar);

        if (!((menuScannedItems == 1 || (menuScannedItems == 2 && bufferChar == '\n')))) {
            selectedOption = -1;
        }

        if (selectedOption == 8)
        {
            printf("Releasing memory and exiting...\n");
            for (int i = 0; i < productArraySize; i++) {
                free(productArray[i].productName);
            }
            free(productArray);
            break;
        }
        else if (selectedOption == 1)
        {
            addNewProduct(&productArray, &productArraySize);
        }
        else if(selectedOption == 2){
            viewAllProducts(productArray, productArraySize);
        }
        else if(selectedOption == 3){
            updateProductQuantity(productArray, productArraySize);
        }
        else if(selectedOption == 4){
            Product* foundProduct = searchProductByID(productArray, productArraySize);
            if (foundProduct == NULL) {
                printf("Product with that ID was not found.\n");
            }
        }
        else if(selectedOption == 5){
            searchProductByName(productArray, productArraySize);
        }
        else if(selectedOption == 6){
            searchProductByPriceRange(productArray, productArraySize);
        }
        else if(selectedOption == 7){
            deleteProduct(&productArray, &productArraySize);
        }
        else
        {
            fprintf(stderr, "Invalid option. Please try again.\n");
        }
        printf("\n");
    }
    return 0;
}
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
    printf("Dynamic Inventory Management System\n");
    printf("=========== Inventory Menu ===========\n");
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
            printf("Error: Invalid ID. Please enter a number between 1 and 10000.\n");
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
            printf("Error: Product name must be between 1 and 50 characters long.\n");
        }
    } while (1);

    do {
        printf("Enter Product Price (0-100000): ");
        fgets(scannerBuffer, sizeof(scannerBuffer), stdin);
        scannedItems = sscanf(scannerBuffer, "%f %c", &tempProduct.productPrice, &bufferChar);
    
        if ((scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n')) && tempProduct.productPrice >= 0 && tempProduct.productPrice <= 100000) {
            break;
        } else {
            printf("Error: Invalid price. Please enter a number between 0 and 100000.\n");
        }
    } while (1);

    do {
        printf("Enter Product Quantity (0-1000000): ");
        fgets(scannerBuffer, sizeof(scannerBuffer), stdin);
        scannedItems = sscanf(scannerBuffer, "%d %c", &tempProduct.productQuantity, &bufferChar);
        if ((scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n')) && tempProduct.productQuantity >= 0 && tempProduct.productQuantity <= 1000000) {
            break;
        } else {
            printf("Error: Invalid quantity. Please enter a number between 0 and 1000000.\n");
        }
    } while (1);
    printf("\n");
    return tempProduct;
}

void printProductDetails(Product currentProduct){
    printf("Product ID: %d | Name: %s | Price: %0.2f | Quantity: %d \n",currentProduct.productID,currentProduct.productName,currentProduct.productPrice,currentProduct.productQuantity);
}

void viewAllProducts(Product* productsArray,int productArraySize){
    for(int productIndex = 0;productIndex < productArraySize ; productIndex++){
        printProductDetails(productsArray[productIndex]);
    }
}

Product* searchProductByID(Product* productsArray,int productsArraysize){
    char inputBuffer[10];
    int targetProductID;
    char bufferchar;
    printf("Enter the Product ID to search : \n");
    do{
        fgets(inputBuffer,sizeof(inputBuffer),stdin);
        int scannedItems = sscanf(inputBuffer,"%d %c",&targetProductID,&bufferchar);
        if(scannedItems == 1 && targetProductID >= 1 && targetProductID <= 10000){
            break;
        }
        else if(scannedItems == 2 && bufferchar == '\n' && targetProductID >= 1 && targetProductID <= 10000){
            break;
        }else{
            fprintf(stderr , "enter the valid and correct product ID \n");
        }
    }
    while (1);
    Product tempProduct;
    for (int productIndex = 0; productIndex < productsArraysize; productIndex++)
    {
        if(productsArray[productIndex].productID == targetProductID){
            tempProduct = productsArray[productIndex];
            printProductDetails(tempProduct);
            return &(productsArray[productIndex]);
        }
    }   

    return NULL;
}

int updateProductQuantity(Product* productsArray,int* productsArraysize){
    char inputBuffer[10];
    int targetProductID;
    char bufferchar;
    int updatedQuantity;
    do{
        printf("Enter the Product ID to Update : \n");
        fgets(inputBuffer,sizeof(inputBuffer),stdin);
        int scannedItems = sscanf(inputBuffer,"%d %c",&targetProductID,&bufferchar);
        if(scannedItems == 1 && targetProductID >= 1 && targetProductID <= 10000){
            break;
        }
        else if(scannedItems == 2 && bufferchar == '\n' && targetProductID >= 1 && targetProductID <= 10000){
            break;
        }else{
            fprintf(stderr , "enter the valid and correct product ID \n");
        }
    }
    while (1);
    do
    {
        printf("Enter the new product quantity");
        fgets(inputBuffer,sizeof(inputBuffer),stdin);
        int scannedItems = sscanf(inputBuffer,"%d %c",&updatedQuantity,&bufferchar);
        if(scannedItems == 1 && updatedQuantity >= 0 && updatedQuantity <= 1000000){
            break;
        }
        else if(scannedItems == 2 && bufferchar == '\n' && updatedQuantity >= 0 && updatedQuantity <= 1000000){
            break;
        }else{
            fprintf(stderr , "enter the valid and correct product ID \n");
        }
    } while (1);
    
    for (int productIndex = 0; productIndex < *productsArraysize; productIndex++)
    {
        if(productsArray[productIndex].productID == targetProductID){
            productsArray[productIndex].productQuantity = updatedQuantity;
            printf("Product Quantity Updated :\n");
            printProductDetails(productsArray[productIndex]);
            return 1;
        }
    } 
    return 0;  
}

int deleteProduct(Product** productsArray, int* productsArraysize) {
    char inputBuffer[20];
    int targetProductID;
    char bufferChar;
    int productFound = 0; 
    int productIndexToDelete = -1;

    do {
        printf("\nEnter Product ID to Delete: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        int scannedItems = sscanf(inputBuffer, "%d %c", &targetProductID, &bufferChar);
        if ((scannedItems == 1 || (scannedItems == 2 && bufferChar == '\n')) && targetProductID >= 1 && targetProductID <= 10000) {
            break;
        } else {
            fprintf(stderr, "Error: Please enter a valid product ID.\n");
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
        for (int productIndex = productIndexToDelete; productIndex < (*productsArraysize - 1); productIndex++) {
            (*productsArray)[productIndex] = (*productsArray)[productIndex + 1];
        }
        if (*productsArraysize == 1) {
            free(*productsArray);
            *productsArray = NULL;
        } else {
            Product* tempArray = realloc(*productsArray, (*productsArraysize - 1) * sizeof(Product));
            if (tempArray == NULL) {
                printf("Error: Memory reallocation failed during delete\n");
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

void addNewProduct(Product** productsArray,int* productsArraysize){
    Product tempProduct = getProductDetails();
   Product* tempArray = realloc(*productsArray, (*productsArraysize + 1) * sizeof(Product));
    if (tempArray == NULL) {
        printf("Error: Memory reallocation failed! Cannot add new product.\n");
        free(tempProduct.productName);
        return;
    }
   *productsArray = tempArray;
    (*productsArray)[*productsArraysize] = tempProduct;
    (*productsArraysize)++;  
    printf("Product added successfully!\n");
}

void searchProductByName(Product* productsArray,int productsArraysize){
    char inputBuffer[52];
    int foundCount = 0;
    do
    {
        fgets(inputBuffer,sizeof(inputBuffer),stdin);
        inputBuffer[strcspn(inputBuffer,'\n')] = '\0';
        if(inputBuffer == 0){
            fprintf(stderr,"Name cannot be empty");
            return;
        }
    } while (1);
    
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

void searchProductByPriceRange(Product* productArray , int productArraySize){
    char inputbuffer[50];
    char bufferChar;
    float startingRange;
    float endingRange;
    do
    {
        printf("Enter minimum price: (0 -100000) \n");
        fgets(inputbuffer,sizeof(inputbuffer),stdin);
        int scannedItem = sscanf(inputbuffer , "%f %c", &startingRange , &bufferChar);
        if((scannedItem == 1 || (scannedItem == 2 && bufferChar == '\n')) && startingRange >= 0 && startingRange <= 100000){
            break;
        }else{
            fprintf(stderr ,"Please Enter a valid Minimum Price \n");
        }
        
    } while (1);

    do
    {
        printf("Enter maximum price: (0 -100000) \n");
        fgets(inputbuffer,sizeof(inputbuffer),stdin);
        int scannedItem = sscanf(inputbuffer , "%f %c", &endingRange , &bufferChar);
        if((scannedItem == 1 || (scannedItem == 2 && bufferChar == '\n')) && endingRange >= 0 && endingRange <= 100000 && startingRange < endingRange){
            break;
        }else{
            fprintf(stderr ,"Please Enter a valid Maximum Price \n");
        }
        
    } while (1);
    int productsFound = 0;
    for(int productIndex=0;productIndex < productArraySize ;productIndex++){
        if(productArray[productIndex].productPrice >= startingRange && productArray[productIndex].productPrice <= endingRange){
            printProductDetails(productArray[productIndex]);
            productsFound++;
        }
    }
    if(productsFound == 0){
        printf("No Products found in the given range %f to %f",startingRange,endingRange);
    }
}

int main()
{
    printf("Enter the number of products (1 - 100): ");
    int initialProductCount;
    char inputBuffer[10];
    char bufferChar;
    static int productArraySize;
    fgets(inputBuffer , sizeof(inputBuffer) , stdin);
    int scannedItems = sscanf(inputBuffer ,"%d %c" , &initialProductCount ,&bufferChar);
    if ( scannedItems != 1 || initialProductCount < 1 || initialProductCount > 100)
    {
        fprintf(stderr, "Please enter the valid initial product count within the specified range(1 -100)");
        return 1;
    }
    Product *productArray = calloc(initialProductCount, sizeof(Product));
    productArraySize = initialProductCount;
    if (productArray == NULL)
    {
        fprintf(stderr, "Memory Allocation Failed");
        return 1;
    }
    for (int productIndex = 0; productIndex < initialProductCount; productIndex++)
    {
        printf("Enter the product details for product %d :\n", productIndex);
        Product tempProduct = getProductDetails();
        productArray[productIndex] = tempProduct;
        
    }
    
    while (1)
    {
        int selectedOption;
        char menuInputBuffer[10];
        printMenu();
        printf("Enter your choice: ");
        fgets(menuInputBuffer, sizeof(menuInputBuffer), stdin);
        int menuScannedItems = sscanf(menuInputBuffer, "%d", &selectedOption);

        if (menuScannedItems != 1) {
            selectedOption = -1;
        }
        if (selectedOption == 8)
        {
            printf("Free memory and exiting the menu");
            for (int productIndex = 0; productIndex < productArraySize; productIndex++)
            {
                free(productArray[productIndex].productName);
            }           
            free(productArray);
            productArray = NULL;
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
            if(updateProductQuantity(productArray,&productArraySize) == 0){
                printf("Product with ID not found \n");
            }
        }
        else if(selectedOption == 4){
           Product* foundProduct = searchProductByID(productArray, productArraySize);
            if (foundProduct == NULL) {
                printf("Product with that ID was not found.\n");
            }
        }
        else if(selectedOption == 5){
            searchProductByName(productArray , productArraySize);
        }
        else if(selectedOption == 6){
            searchProductByPriceRange(productArray , productArraySize);
        }   
        else if(selectedOption == 7){
            deleteProduct(&productArray,&productArraySize);
        }
        else
        {
            fprintf(stderr, "Please select a valid option.\n");
        }
    }
}
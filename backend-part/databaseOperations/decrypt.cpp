#include <iostream>
#include <stdlib.h>
using namespace std;
int main()
{
    FILE *filePointer;
    char ch;
    
    filePointer = fopen("encrypt.txt", "r");

  
    if (filePointer == NULL)
        cout << "File is not available \n";
    
    else
    {
        
        while ((ch = fgetc(filePointer)) != EOF)
        {
            //printf("%c", ch);
            ch -= 100;
            printf("%c", ch);
            
        }
    }

   
    fclose(filePointer);

    return 0;
}

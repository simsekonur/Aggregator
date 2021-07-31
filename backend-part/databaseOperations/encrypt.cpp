#include <iostream>
#include <stdlib.h>
using namespace std;
int main()
{
    FILE *filePointer;
    FILE *filePointer2;
    char ch;

    filePointer = fopen("config.txt", "r");
    filePointer2 = fopen("encrypt.txt", "w");

    if (filePointer == NULL || filePointer2 == NULL)
        cout << "Files are not available \n";

    
    else
    {

        while ((ch = fgetc(filePointer)) != EOF)
        {
            //printf("%c", ch);
            ch += 100;
            //printf("%c", ch);
            fputc(ch,filePointer2);
            
        }
    }

    fclose(filePointer);

    return 0;
}

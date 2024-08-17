#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover [name of file to recover]\n");
        return 1;
    }

    char *infile = argv[1];

    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }

    const int size = 512;
    const int max_numbers_of_filename = 8; // длина строки наименования файла с символом окончания

    uint8_t buffer[size];

    int jpeg_number = 0;
    int flag = 0;
    char *filename = malloc(sizeof(char)  * max_numbers_of_filename);
    FILE *new_jpeg = NULL;

    while (fread(&buffer, 1, 512, inptr) == 512)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            flag = 1;
            if (jpeg_number == 0)
            {
                sprintf(filename, "%03i.jpg", jpeg_number);

                new_jpeg = fopen(filename, "w");
                if (new_jpeg == NULL)
                {
                    printf("Could not open %s.\n", filename);
                    return 1;
                }
                fwrite(&buffer, 1, 512, new_jpeg);
            }
            else
            {
                fclose(new_jpeg);
                sprintf(filename, "%03i.jpg", jpeg_number);
                new_jpeg = fopen(filename, "w");
                if (new_jpeg == NULL)
                {
                    printf("Could not open %s.\n", filename);
                    return 1;
                }
                fwrite(&buffer, 1, 512, new_jpeg);
            }
            jpeg_number++;
        }
        else if (flag == 1)
        {
            fwrite(&buffer, 1, 512, new_jpeg);
        }
    }
    fclose(new_jpeg);
    fclose(inptr);
    free(filename);
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

bool header(BYTE buffer[512]);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *inptr = fopen(argv[1], "r");

    if (inptr == NULL)
    {
        printf("Couldn't find the %s file", argv[1]);
        return 2;
    }

    BYTE buffer[512];
    int cnt = 0;
    int num = 0;

    while (1)
    {
        fread(buffer, 512, 1, inptr);
        cnt++;

        if (header(buffer) == true)
        {
            char img[8];
            sprintf(img, "%03i.jpg", num);
            FILE *outptr = fopen(img, "a");
            do
            {
                fwrite(buffer, 512, 1, outptr);
                fread(buffer, 512, 1, inptr);
                cnt++;

                if (feof(inptr))
                {
                break;
                }
            }
            while ((header(buffer)) == false);

            fclose(outptr);
            fseek(inptr, -512, SEEK_CUR);
            num++;
        }

        else if (feof(inptr))
        {
            break;
        }
    }
    fclose(inptr);
    return 0;
}

bool header(BYTE buffer[512])
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
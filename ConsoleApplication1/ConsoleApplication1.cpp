// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <C:\Users\P15S-ENG5\Downloads\libmseed-main\libmseed-main\libmseed.h>

int main()
{
	//char filename[] = "C:/Users/P15S-ENG5/Downloads/libmseed-main/libmseed-main/test/data/Int16-encoded.mseed";
	char filename[] = "C:/Users/P15S-ENG5/Work/MiniSEED/Data/OO-HYEA1--YDH-2023-05-17T00 00 00.000015.mseed";

    MS3Record* msr = NULL;
    uint32_t flags = 0;

    static int8_t verbose = 0;
    static int8_t ppackets = 0;

    int64_t totalrecs = 0;
    int64_t totalsamps = 0;
    int retcode;


    /* Set flag to validate CRCs when reading */
    flags |= MSF_VALIDATECRC;

    /* Parse byte range from file/URL path name if present */
    flags |= MSF_PNAMERANGE;

    /* Set flag to unpack data if printing samples */
    flags |= MSF_UNPACKDATA;

    /* Enable accumulation of up to 10 error and warning messages */
    ms_rloginit(NULL, NULL, NULL, NULL, 10);

    /* Loop over the input file record by record */
    while ((retcode = ms3_readmsr(&msr, filename, NULL, NULL,
        flags, verbose)) == MS_NOERROR)
    {
        totalrecs++;
        totalsamps += msr->samplecnt;

        msr3_print(msr, ppackets);

        /* Print data samples */
        if (msr->numsamples > 0)
        {
            int line, col, cnt, samplesize;
            int lines = (msr->numsamples / 6) + 1;
            void* sptr;

            if ((samplesize = ms_samplesize(msr->sampletype)) == 0)
            {
                ms_log(2, "Unrecognized sample type: '%c'\n", msr->sampletype);
            }

            for (cnt = 0, line = 0; line < lines; line++)
            {
                for (col = 0; col < 6; col++)
                {
                    if (cnt < msr->numsamples)
                    {
                        sptr = (char*)msr->datasamples + (cnt * samplesize);

                        if (msr->sampletype == 'i')
                            ms_log(0, "%10d  ", *(int32_t*)sptr);

                        else if (msr->sampletype == 'f')
                            ms_log(0, "%10.8g  ", *(float*)sptr);

                        else if (msr->sampletype == 'd')
                            ms_log(0, "%10.10g  ", *(double*)sptr);

                        cnt++;
                    }
                }
                ms_log(0, "\n");

            }
        }
    }

    /* Emit all accumulated warning and error messages */
    ms_rlog_emit(NULL, 0, verbose);

    /* Make sure everything is cleaned up */
    ms3_readmsr(&msr, NULL, NULL, NULL, 0, 0);


    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

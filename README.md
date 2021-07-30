# MICROCHIP INTERNSHIP SUBMISSION:

# testlogs-enumeration
This c console application enumerates the testlogs and prepares a csv report based  on the passed and failed hardware tests.

# Algorithem
STEP 1:  Enumerate TEST LOGS

STEP 2: If the TEST LOGS directory is empty, end the program with a negative return. Else iterate and enumerate the contents of the timestamp directories 
               with the loop variable ‘i’ till the ‘l’ number of folders get parsed.

STEP 3:  If timestamp[i] is empty, continue proceeding with the next iteration of i. Else iterate and enumerate the contents of module directories with the loop 
              variable ‘j’ till the ‘m’ number of folders get parsed.

STEP 4:  If the module[j]  is empty, continue with the next iteration of j. Else iterate the log files which are named after their test IDs with the loop variable ‘k’ 
               till the ‘n’ number of log files get parsed.

STEP 5:  Open the file TEST_ID[k] in read mode and check if any of the search strings is present in the file.

STEP 6:  if it contains “PASS” or “PASSED” or “pass” or “passed”, in TEST_ID[k] set status=’PASS’. Else if the file contains “FAIL” or “FAILED” or 
              “fail” or “failed” in TEST_ID[K], set the status=’FAIL’;
                    
STEP 7:  For every iteration in k:
              [Serial No, (char)time stamp[i], (char)Module[j], (char)log[k], status]  gets updated to ouput.csv (ROW WISE) separated by comma.
               SerialNo is incremented and the iteration of k continues till all the files in the processing module folder get parsed.               


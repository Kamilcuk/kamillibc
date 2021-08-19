set(CTEST_SUBMIT_URL "http://cdash.karta.dyzio.pl//submit.php?project=kamillibc")
ctest_submit(HTTP_HEADER "Authorization: Bearer $ENV{CDASH_KARTA_DYZIO_PL_TOKEN}")

